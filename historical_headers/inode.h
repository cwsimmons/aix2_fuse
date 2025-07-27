/* @(#)inode.h	7.4 87/12/04 18:12:59 */

/*
 * The I node is the focus of all
 * file activity in unix. There is a unique
 * inode allocated for each active file,
 * each current directory, each mounted-on
 * file, text file, and the root. An inode is 'named'
 * by its dev/inumber pair. (iget/iget.c)
 * Data, from mode on, is read in
 * from permanent inode on volume.
 */

#ifndef _h_INODE
#define _h_INODE

#include <sys/types.h>
#include <sys/seg.h>

#define	NADDR	13
#define	NSADDR	(NADDR*sizeof(daddr_t)/sizeof(short))
#define	NFADDR	8
#define	PIPSIZ	NFADDR*BSIZE

#define	NHINO	128	/* must be power of 2 */
#define	IHASH(X)	(&hinode[(int)(X) & (NHINO-1)])

struct	hinode {
	union
	{
		struct inode  *ri_forwd;
		struct sinode *si_forwd;
	} unino;
};

#define hri_forw	unino.ri_forwd
#define hsi_forw	unino.si_forwd

/* file handle */

#define FHSIZE  32
typedef struct
{
	char x[FHSIZE];			/* allows structure assignments	*/
} fhandle;


struct fileh
{
	/*
	 * This MUST be the same size as the above fhandle!!! 
	 */

	long    fh_type;         /* file system type */
	dev_t   fh_dev;          /* device where inode resides */
	ino_t   fh_number;       /* i number, 1-to-1 with device address */
	char    fh_gen;          /* inode generation number */
	char    fh_x[21];        /* reserved */
} ;

struct	inode
{
	struct  gnode
	{
		struct inode *g_forw;   /* hash chain forw */
		struct inode *g_back;   /* hash chain back */
		short   g_flag;
		short   g_sflag;        /* s-mode and cache flags */
		ushort 	g_mode;		/* permission bits and file type */
		cnt_t   g_count;        /* reference count */
		off_t   g_size;         /* size of file */
		time_t  g_mtime;        /* server modification time */
		struct filock *g_filocks;  /* locked region list */
		struct mapnode *g_mapn; /* for mapped file */
	} gi;

#define i_forw gi.g_forw
#define i_back gi.g_back
#define i_flag gi.g_flag
#define i_sflag gi.g_sflag
#define i_count gi.g_count
#define i_size  gi.g_size
#define i_mtime gi.g_mtime
#define i_filocks gi.g_filocks
#define i_mapn  gi.g_mapn
#define i_mode  gi.g_mode

	dev_t   i_dev;          /* device where inode resides */
	ino_t   i_number;       /* i number, 1-to-1 with device address */
	char    i_gen;          /* inode generation number */
	short   i_nlink;        /* directory entries */
	ushort  i_uid;          /* owner */
	ushort  i_gid;          /* group of owner */
	struct {
		union {
			daddr_t i_a[NADDR];	/* if normal file/directory */
						/* for pipes, with select */
			struct {
			daddr_t	i__faddr[NFADDR];	/* fifo block ptrs */
			struct proc *i__frsel;	/* ptr for read select */
			struct proc *i__fwsel;	/* ptr for write select */
			/* except select NOT implemented, takes up space! */
			short	i__frptr;
			short	i__fwptr;
			short	i__frcnt;
			short	i__fwcnt;
			short	i__fflag;
			short	i__fpad;	/* nothing */
			} i__f;			
		} i_p;
		daddr_t	i_l;		/* last logical block read (for read-ahead) */
	} i_blks;
	struct  file_access *i_fa;  /* file access structure pointer */
};

struct  sinode
{
	struct  gnode gi;

#define si_forw		gi.g_forw
#define si_back		gi.g_back
#define si_flag		gi.g_flag
#define si_sflag	gi.g_sflag
#define si_mode 	gi.g_mode
#define si_count	gi.g_count
#define si_size 	gi.g_size
#define si_mtime	gi.g_mtime
#define si_filocks	gi.g_filocks
#define si_mapn 	gi.g_mapn

	fhandle si_fh;           /* file handle of real file */
	unsigned long si_nid;    /* node id of machine who holds real file */
	struct  node *si_node;   /* node structure pointer */
	daddr_t	si_lastr;	 /* last block read */
	int    	si_lastra;	 /* last readahead block */
	int     si_dwqcnt;       /* count of blocks in delayed write queue */
};

extern struct inode inode[];	/* The inode table itself */
extern struct locklist locklist[]; /* The lock table itself */
#ifndef NFLOCKS
#define NFLOCKS NINODE
#endif

/* flags */
#define	ILOCK	01		/* inode is locked */
#define	IUPD	02		/* file has been modified */
#define	IACC	04		/* inode access time to be updated */
#define	IMOUNT	010		/* inode is mounted on */
#define	IWANT	020		/* some process waiting on lock */
#define	ITEXT	040		/* inode is pure text prototype */
#define	ICHG	0100		/* inode has been changed */
#define	ISYN	0200		/* do synchronous write for iupdate */
#define	INAMI	0400		/* set when namei() calls iput() */
#define	IZOMB	01000		/* this inode is unavailable. */
#define IDMA    02000           /* use DMA when possible */
#define IMAP    04000           /* mapped file */
#define ISUR    010000          /* surrogate inode */

/* sflags */
#define IFSWARN 0x0001          /* client sync for near-full filesystem */
#define IFSFULL 0x0002          /* async write failed: full filesystem */
#define ISASYNC 0x0004          /* inode s_mode async */
#define ISFSYNC 0x0008          /* inode s_mode fullsync */
#define ISRONLY 0x0010          /* inode s_mode read-only */
#define ISSMODE 0x001C          /* mask for smodes */
#define FALOCK  0x0040          /* file access structure list is locked */
#define FAWANT  0x0080          /* file access structure list is wanted */

/* modes */
#define	IFMT	0170000		/* type of file */
#define		IFDIR	0040000	/* directory */
#define		IFCHR	0020000	/* character special */
#define		IFBLK	0060000	/* block special */
#define		IFREG	0100000	/* regular */
#define         IFMPC   0030000 /* multiplexed char special - obsolete */
#define         IFMPB   0070000 /* multiplexed block special - obsolete */
#define		IFIFO	0010000	/* fifo special */
#define	ISUID	04000		/* set user id on execution */
#define	ISGID	02000		/* set group id on execution */
#define ISVTX	01000		/* save swapped text even after use */
#define	IREAD	0400		/* read, write, execute permissions */
#define	IWRITE	0200
#define	IEXEC	0100

/*
 * iblock is used in bmap()/iclear() to determine where a block index
 * is stored in aindirect block
 */

struct iblock
{
	daddr_t	ibk_bn;		/* logical block number */
	int	ibk_off;	/* index into indirect block */
};

#define	i_addr	i_blks.i_p.i_a
#define	i_lastr	i_blks.i_l
#define	i_rdev	i_blks.i_p.i_a[0]

#define	i_faddr	i_blks.i_p.i_a
#define	i_frptr	i_blks.i_p.i__f.i__frptr
#define	i_fwptr	i_blks.i_p.i__f.i__fwptr
#define	i_frcnt	i_blks.i_p.i__f.i__frcnt
#define	i_fwcnt	i_blks.i_p.i__f.i__fwcnt
#define	i_fflag	i_blks.i_p.i__f.i__fflag
#define i_frsel	i_blks.i_p.i__f.i__frsel
#define i_fwsel	i_blks.i_p.i__f.i__fwsel

#define	IFIR	01
#define	IFIW	02
/* define select collision flags */
#define IFRCOL	010		/* read select collision */
#define IFWCOL	020		/* write select collision */
				/* exception select NOT implemented */

/*
 * file access structure, linked to server real inode,
 * used to tie together real inode and client surrogate inode
 */

struct file_access
{
    struct file_access *fa_next;  /* file access structure pointer */
    short fa_flag;                /* flags */
    short fa_count;               /* use count */
    short fa_orocnt;              /* open read/only count */
    short fa_orwcnt;              /* open read/write count */
    short fa_txtcnt;		  /* executing process count */
    struct node *fa_node;         /* node structure pointer */
    int fa_nid;                   /* node id */
    struct inode *fa_sip;         /* surrogate inode pointer */
};

/* flags */
#define FACCINV 01                /* client cache invalid */

/* return code from faput */
#define DOSMODEPROC 1             /* indicates that smode processing needs */
				  /* to be done */

#endif _h_INODE
