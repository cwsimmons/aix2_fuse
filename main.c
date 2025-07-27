
/* 

    Copyright 2025 Christopher Simmons

  This program is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation, either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along
  with this program. If not, see <https://www.gnu.org/licenses/>.

*/

#define FUSE_USE_VERSION 31

#include "dir.h"
#include "ino.h"
#include "filsys.h"

#include <unistd.h>
#include <fuse3/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <sys/stat.h>

static int image_fd = -1;
filsys_t super;

struct aix2_fuse_options {
    const char* image_file;
    long unsigned int offset;
};

static struct aix2_fuse_options options = {
    .image_file = NULL,
    .offset = 0
};

static struct fuse_opt option_description[] = {
    {"--filename %s", offsetof(struct aix2_fuse_options, image_file), FUSE_OPT_KEY_DISCARD},
    {"--offset %lu", offsetof(struct aix2_fuse_options, offset), FUSE_OPT_KEY_DISCARD},
    FUSE_OPT_END
};

int min(int x, int y) {
    return (x < y) ? x : y;
}

int max(int x, int y) {
    return (x > y) ? x : y;
}

void hex_print(uint8_t* buffer, int length) {
    for (int i = 0; i < length; i++) {
        printf("%.2x ", buffer[i]);
        if ((i % 16) == 15) {
            printf("\n");
        }
    }
    if ((length % 16)) {
        printf("\n");
    }
}

ssize_t my_pread(int fd, void* buf, size_t count, off_t offset) {

    size_t bytes_read = 0;
    while (bytes_read < count) {
        ssize_t n = pread(fd, (char*)buf + bytes_read, count - bytes_read, offset + bytes_read);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (n == 0)
            break;
        bytes_read += n;
    }

    return bytes_read;
}

int read_inode(uint16_t inum, struct dinode* inode) {
    ssize_t bytes_read = my_pread(image_fd, inode, sizeof(struct dinode), options.offset + (2 * BSIZE) + ((inum - 1) * 64));
    if (bytes_read < sizeof(struct dinode))
        return -1;

    inode->di_mode = ntohs(inode->di_mode);
    inode->di_nlink = ntohs(inode->di_nlink);
    inode->di_uid = ntohs(inode->di_uid);
    inode->di_gid = ntohs(inode->di_gid);
    inode->di_size = ntohl(inode->di_size);
    inode->di_atime = ntohl(inode->di_atime);
    inode->di_mtime = ntohl(inode->di_mtime);
    inode->di_ctime = ntohl(inode->di_ctime);

    return 0;
}

uint32_t get_inode_address(struct dinode* inode, int addr_number) {
    int x = addr_number * 3;
    return (((uint32_t) inode->di_addr[x]) << 16) |
            (((uint32_t) inode->di_addr[x+1]) << 8) |
            (((uint32_t) inode->di_addr[x+2]));
}

int read_inode_data(struct dinode* inode, void* buf, int offset, int length) {

    if (offset >= inode->di_size) {
        return 0;
    }

    length = min(length, inode->di_size - offset);

    int bytes_read = 0;
    uint32_t data_block_address;

    while(bytes_read < length) {
        int block = (offset + bytes_read) / BSIZE;
        if (block < 10) {
            data_block_address = get_inode_address(inode, block);
        } else {
            break;
        }

        int pos_in_block = (offset + bytes_read) % BSIZE;
        int grab = min(length - bytes_read, BSIZE - pos_in_block);

        int count;
        if ((count = my_pread(image_fd, (char*)buf + bytes_read, grab, options.offset + (BSIZE * data_block_address) + pos_in_block)) < 0) {
            break;
        }
        bytes_read += count;
    }

    return bytes_read;

}

int lookup_entry_by_name(struct dinode* inode, const char* name, uint16_t* inumber) {
    char buff[BSIZE];
    struct direct* dir_buff = (struct direct*) buff;
    int bytes_read = 0;
    char name_buffer[DIRSIZ+1];

    if (!(inode->di_mode & IFDIR)) {
        return -1;
    }

    while (bytes_read < inode->di_size) {
        int grab = min(inode->di_size - bytes_read, BSIZE);
        int bytes_read_now = read_inode_data(inode, buff, bytes_read, grab);
        if (bytes_read_now < 0)
            break;
        for (int i = 0; i < (bytes_read_now / sizeof(struct direct)); i++) {
            strncpy(name_buffer, dir_buff[i].d_name, DIRSIZ);
            name_buffer[DIRSIZ] = '\0';
            if (strcmp(name_buffer, name) == 0) {
                *inumber = ntohs(dir_buff[i].d_ino);
                return (bytes_read / sizeof(struct direct)) + i;
            }
        }
        bytes_read += bytes_read_now;
    }

    return -1;
}

int path_to_inumber(const char *path, uint16_t* inumber) {

    if (strcmp(path, "/") == 0) {
        *inumber = 2;
        return 0;
    }

    char temp[1024];
    snprintf(temp, sizeof(temp), "%s", path);
    char* component_start = temp + 1;

    uint16_t ino_so_far = 2;
    struct dinode inode;

    for (char* p = component_start;; p++) {
        if (*p == '/') {
            *p = '\0';
            if (read_inode(ino_so_far, &inode))
                return -1;
            uint16_t next_ino;
            if (lookup_entry_by_name(&inode, component_start, &next_ino) < 0) {
                return -1;
            }
            ino_so_far = next_ino;
            //*p = '/';
            component_start = p + 1;

        } else if (*p == '\0') {
            if (read_inode(ino_so_far, &inode))
                return -1;
            uint16_t next_ino;
            if (lookup_entry_by_name(&inode, component_start, &next_ino) < 0) {
                return -1;
            }
            *inumber = next_ino;
            return 0;
        }
    }

    return -1;
}

static int myfs_getattr(const char *path, struct stat *stbuf,
                        struct fuse_file_info *fi) {
    (void) fi;
    memset(stbuf, 0, sizeof(struct stat));

    uint16_t inum;
    if (path_to_inumber(path, &inum))
        return -ENOENT;

    struct dinode inode;
    if (read_inode(inum, &inode))
        return -ENOENT;

    stbuf->st_ino = inum;
    stbuf->st_mode = inode.di_mode;
    stbuf->st_nlink = inode.di_nlink;
    stbuf->st_uid = inode.di_uid;
    stbuf->st_gid = inode.di_gid;
    stbuf->st_size = inode.di_size;
    stbuf->st_atime = inode.di_atime;
    stbuf->st_mtime = inode.di_mtime;
    stbuf->st_ctime = inode.di_ctime;
    
    return 0;
}

static int myfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi,
                        enum fuse_readdir_flags flags) {
    (void) offset;
    (void) fi;
    (void) flags;

    uint16_t inum;

    if (path_to_inumber(path, &inum))
        return -ENOENT;

    struct dinode inode;
    if (read_inode(inum, &inode))
        return -ENOENT;

    char buff[BSIZE];
    struct direct* dir_buff = (struct direct*) buff;
    int bytes_read = 0;
    char name_buffer[DIRSIZ+1];

    while (bytes_read < inode.di_size) {
        int grab = min(inode.di_size - bytes_read, BSIZE);
        int bytes_read_now = read_inode_data(&inode, buff, bytes_read, grab);
        if (bytes_read_now < 0)
            break;
        for (int i = 0; i < (bytes_read_now / sizeof(struct direct)); i++) {

            if (dir_buff[i].d_ino == 0)
                continue;

            strncpy(name_buffer, dir_buff[i].d_name, DIRSIZ);
            name_buffer[DIRSIZ] = '\0';

            struct stat entry_stat;
            struct stat* p_entry_stat = NULL;
            struct dinode entry_inode;
            if (!read_inode(ntohs(dir_buff[i].d_ino), &entry_inode)) {
                p_entry_stat = &entry_stat;
                
                memset(&entry_stat, 0, sizeof(struct stat));
                entry_stat.st_ino = ntohs(dir_buff[i].d_ino);
                entry_stat.st_mode = entry_inode.di_mode;
                entry_stat.st_nlink = entry_inode.di_nlink;
                entry_stat.st_uid = entry_inode.di_uid;
                entry_stat.st_gid = entry_inode.di_gid;
                entry_stat.st_size = entry_inode.di_size;
                entry_stat.st_atime = entry_inode.di_atime;
                entry_stat.st_mtime = entry_inode.di_mtime;
                entry_stat.st_ctime = entry_inode.di_ctime;

            }

            filler(buf, name_buffer, p_entry_stat, 0, 0);
        }
        bytes_read += bytes_read_now;
    }

    return 0;
}

static int myfs_open(const char *path, struct fuse_file_info *fi) {
    uint16_t inum;

    if (path_to_inumber(path, &inum))
        return -ENOENT;

    struct dinode inode;
    if (read_inode(inum, &inode))
        return -ENOENT;

    if ((fi->flags & O_ACCMODE) != O_RDONLY)
        return -EACCES;

    return 0;
}

static int myfs_read(const char *path, char *buf, size_t size, off_t offset,
                     struct fuse_file_info *fi) {
    (void) fi;
    
    uint16_t inum;

    if (path_to_inumber(path, &inum))
        return -ENOENT;

    struct dinode inode;
    if (read_inode(inum, &inode))
        return -ENOENT;

    size = read_inode_data(&inode, buf, offset, size);

    return size;
}

static struct fuse_operations myfs_oper = {
    .getattr = myfs_getattr,
    .readdir = myfs_readdir,
    .open    = myfs_open,
    .read    = myfs_read,
};

int main(int argc, char *argv[]) {

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    if (fuse_opt_parse(&args, &options, option_description, NULL) == -1) {
        fprintf(stderr, "Failed to parse options\n");
        return -1;
    }

    printf("filename = %s\n", options.image_file);
    printf("offset = %lu\n", options.offset);

    image_fd = open(options.image_file, O_RDONLY);

    if (image_fd < 0) {
        fprintf(stderr, "Could not open image file\n");
        return -1;
    }

    ssize_t bytes_read = my_pread(image_fd, &super, sizeof(filsys_t), options.offset + BSIZE);

    if (bytes_read != sizeof(filsys_t)) {
        fprintf(stderr, "Could not read superblock\n");
        return -1;
    }

    if (strncmp(super.s_magic, FSmagic, 4)) {
        fprintf(stderr, "Bad superblock magic\n");
        return -1;
    }

    printf("Filesystem Name = %s\n", super.s_fname);

    return fuse_main(args.argc, args.argv, &myfs_oper, NULL);
}