
#include <stdint.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include <arpa/inet.h>

void usage(char* progname) {
    printf("Usage:\n");
    printf("  %s <command> [args]\n\n", progname);
    printf("Available Commands:\n");
    printf("  show\n");
}

#define MINIDISK_BLOCK	3	/* starting block number of minidisk */
#define MAXDISKS 47		/* actual minidisk size */

struct miniheader {
	int32_t number;		/* number of entries */
	int32_t level;		/* revision level */
	int16_t unused;		/* index of next unused entry */
	int16_t lastused;		/* index of entry most recently used */
	int16_t first;		/* first entry */
	int16_t last;		/* last entry index */
	int32_t bad_block;		/* first bad block */
	int32_t bad_size;		/* number of bad blocks */
	int32_t unused2;		/* reserved */
	int32_t unused3;		/* reserved */
};

struct minidisk {
	int16_t previous;		/* 0 link to previous entry */
	int16_t next;		/* 2 line to next entry */
	char name[4];		/* 4 name of minidisk */
	unsigned :32;		/* 8 */
	int32_t date;		/* 12 the date of creation */
	int16_t iodn;		/* 16 the IODN used */
#define nextfree iodn		/* 	hokey way of doing it! */
	char blocksize;		/* 18 filesystem block size (bits 0-3) */
#define BLOCK_512	0
#define BLOCK_1024	1
#define BLOCK_2048	3
	char type;		/* 19 type flags */
#define TYPE_RESERVED	0x80	/* reserved */
#define TYPE_NOBADBLOCK	0x40	/* no bad block forwarding on this disk */
#define TYPE_PAGE	0x20	/* pageing space */
#define TYPE_FILE	0x10	/* AIX file system */
#define TYPE_AIX	0x08	/* AIX system minidisk */
#define TYPE_PC		0x04	/* coprocessor disk */
#define TYPE_VRM	0x02	/* VRM */
#define TYPE_IPL	0x01	/* IPL'able */
	int32_t unused;		/* 20 */
	int32_t start;		/* 24 start block of partition */
	int32_t size;		/* 28 size of partition */
				/* 32 total length */
};

struct minidirectory {
    struct miniheader header;
    struct minidisk minidisk[MAXDISKS];
};

void do_show(FILE* image_file) {

    fseek(image_file, MINIDISK_BLOCK * 512, SEEK_SET);

    struct minidirectory table;

    int count = fread(&table, 512, 3, image_file);

    if (count != 3) {
        printf("Could not read entire minidisk directory\n");
        return;
    }

    short index = ntohs(table.header.first);

    printf("Index  IODN Name    Start    Size           Date\n");
    printf("----------------------------------------------------------\n");

    while (index >= 0) {

        if (index >= MAXDISKS) {
            printf("Invalid minidisk index, stopping\n");
            return;
        }

        struct minidisk* entry = &table.minidisk[index];

        char name[5];
        strncpy(name, entry->name, 4);
        name[4] = '\0';

        short iodn = ntohs(entry->iodn);
        time_t creation = (time_t) ntohl(entry->date);
        int start = ntohl(entry->start);
        int size = ntohl(entry->size);

        printf("%5d %5d %4s  %7d %7d %s", index, iodn, name, start, size, ctime(&creation));

        index = ntohs(entry->next);

    }

}

int main(int argc, char** argv) {

    if (argc < 2) {
        usage(argv[0]);
        return 0;
    }

    char* command = argv[1];

    argv++;
    argc--;
    
    int c;
    int option_index = 0;

    char image_filename[FILENAME_MAX] = "";

    static struct option long_options[] = {
        {"image",           required_argument, 0, 'i'},
        {0,                 0,                 0,  0 }
    };

    while((c = getopt_long(argc, argv, "i:", long_options, &option_index)) != -1) {
        switch(c) {
            case 'i':
                strcpy(image_filename, optarg);
                break;
            default:
                break;
        }
    }

    if (strlen(image_filename) == 0) {
        printf("An image file must be specified.\n");
        return -1;
    }

    FILE* image_file = fopen(image_filename, "rb");

    if (image_file == NULL) {
        printf("Could not open image file.\n");
        return -1;
    }

    if (strcasecmp(command, "show") == 0) {
        do_show(image_file);
    } else {
        printf("Unrecognized command: %s\n", command);
    }
    
    return 0;
}