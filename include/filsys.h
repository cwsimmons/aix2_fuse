/* @(#)filsys.h	7.1 - 87/06/15 - 23:35:53 */
/*
 *
 * NAME:        filsys.h
 *
 * PURPOSE:     This file defines the format of a file system superblock.
 *              This is the block, at the start of each file system, that
 *              defines the size and parameters of the file system.  All
 *              allocation and deallocation of files and blocks is done with
 *              the aid of tables within the superblock.
 *
 *              The superblock is laid out so as to make it possible to
 *              write blocksize independent file system algorithms.  The
 *              superblock includes all size specific parameters (including
 *              the lengths of the variable length lists).  Also, the
 *              variable length lists occur after all of the other fixed
 *              size parms.
 */

#ifndef _h_FSYS
#define _h_FSYS

#include "param.h"  /* definitions of basic system constants           */

#include <stdint.h>

#define FSfixsz 112     /* Fixed-format region is 112 bytes long           */

typedef struct  filsys
{       /* basic file system parameters - initialized when FS is created   */

	char    s_magic[4];     /* magic number:   FSmagic = 0xdf817eb2    */
	char    s_flag[4];      /* flag word (see below)                   */
	int32_t s_fsize;        /* size (in blocks) of entire file system  */
	uint16_t  s_bsize;        /* block size (in bytes) for this filsys   */
	uint16_t  s_isize;        /* size (in blocks) of i-list & overhead   */
	int16_t   s_cyl;          /* number of blocks per cylinder           */
	int16_t   s_skip;         /* block interleaving factor               */
	int16_t   s_nicfree;      /* number of slots in block free list      */
	int16_t   s_nicino;       /* number of slots in free i-node list     */
	int16_t   s_sicfree;      /* byte offset to start of block free list */
	int16_t   s_sicino;       /* byte offset to start of free i-node lst */
	char    s_fname[6];     /* name of this file system                */
	char    s_fpack[6];     /* name of this volume                     */
	int16_t   s_nicfrag;      /* number of slots in fragment table       */
	int16_t   s_sicfrag;      /* byte offset to start of frag table      */
	int32_t	s_swaplo;	/* start of swap area */
	int32_t	s_nswap;	/* number of block of swap area */

	union {			/* Union contrived here so that no mods    */
				/* would be required to mkfs(1).  jar      */
		struct {	/* Boot count per superblock		    */
			int32_t bc_cnt;
			char bc_rsvd[32];
		}s_bc;
		char    unused[36];   
	} s_bcun;

#define s_bootcnt	s_bcun.s_bc.bc_cnt
#define s_rsvd		s_bcun.unused

	/* current file system state information, values change over time  */

	uint16_t  s_tffrag;       /* number of fragmented files              */
	uint16_t  s_tbfrag;       /* number of fragmented blocks             */
	int16_t   s_findex;       /* fragment allocation index               */
	char    s_fmod;         /* flag: set when file system is mounted   */
                                /* see below                               */
	char    s_ronly;        /* flag: file system is read only          */
	int32_t s_tfree;        /* total number of available free blocks   */
	char    s_flock;        /* flag: free list lock                    */
	char    s_ilock;        /* flag: free i-node list lock             */
	int16_t   s_nfree;        /* number of valid blocks in free-list     */
	uint16_t   s_tinode;       /* total number of available i-nodes       */
	int16_t   s_ninode;       /* number of valid i-nodes in free-list    */
	int32_t  s_time;         /* time of last superblock update          */

	/*
	 *  TOTAL LENGTH OF FIXED-FORMAT REGION: 112 bytes
	 *
	 *  All variable length fields appear beyond this point, and are
	 *      described and pointed to by information in the fixed format
	 *      portion of the superblock:
	 *
	 *      int32_t s_free[s_nicfree];      <free block list>
	 *      ino_t   s_inode[s_nicino];      <free I-node list>
	 *      frag_t  s_frag[s_nicfrag];      <fragment table>
	 *
	 *      Macros defined below allow access to these tables.
	 */

	union                   /* Variable-format region                  */
	{  char    	su_var[BSIZE-FSfixsz];
	   struct
	   {	int32_t	su_free[NICFREE];
		uint16_t	su_inode[NICINOD];
	   } su_ovly;
	} s_u;
} filsys_t;

#define s_free  s_u.su_ovly.su_free     /* #define away the union          */
#define s_inode s_u.su_ovly.su_inode
#define s_var   s_u.su_var

#define s_n     s_cyl           /* for compatibilty with old systems       */
#define s_m     s_skip

/*
 *  Notes on "s_magic" and "s_flag" fields:
 *      The superblock's magic number is encoded as a 4-byte character
 *      string to make it possible to validate the superblock without
 *      knowing the byte order of the remaining fields.  To check for
 *      a valid superblock, use something like:
 *
 *              if (strncmp(sp->s_magic,FSmagic,4) == 0)
 *
 *      The last byte of the file system type code ("s_flag[3]") gives
 *      the blocksize-dependent information -- see <sys/fstypes.h> for
 *      the list of possible values and their meanings.  The first byte
 *      of this field ("s_flag[0]") gives the CPU code for the host
 *      system (with the byte order encoded in the low order bits) --
 *      see <a.out.h> for the legal CPU codes.
 */

#define FSmagic "\337\201\176\262"      /* magic number for file systems    */

#define s_cpu   s_flag[0]               /* Target cpu type code             */
#define s_type  s_flag[3]               /* File system type code            */

#define Fs1b    1               /* 512-byte blocksize file system          */
#define Fs2b    2               /* 1024-byte blocksize                     */
#define Fs4b    3               /* 2048-byte blocksize                     */
#define Fs8b    4               /* 4096-byte blocksize                     */

/*
 * Notes on s_fmod field:
 *	This field is intended to be a three state flag with the third
 *	state being a sticky state.  The three states are:
 *
 *	0 = file system is clean and unmounted
 *	1 = file system is mounted
 *	2 = file system was mounted when dirty
 *
 *	If you merely mount and unmount the filesystem, the flag
 *	toggles back and forth between states 0 and 1.  If you ever
 *	mount the filesystem while the flag is in stat 1 then it
 *	goes to state 2 and stays there until you run fsck.
 *	The only way to clean up a corrupted file system (and change
 *	the flag from state 2 back to state 0) is to run fsck.
 *	The bit above this tri-state (i.e. 04, FM_SDIRTY) is only used
 *	in core.  It is never written to disk.
 */
#define FM_CLEAN   00   /* File system is clean and unmounted */
#define FM_MOUNT   01   /* File system is mounted cleanly */
#define FM_MDIRTY  02   /* File system was dirty when last mounted */
#define FM_SDIRTY  04   /* Super block is dirty; this bit is not written out */

#define	FMOD(x)		((x)==0?1:2)
#define	FCLEAN(x)	((x)==2?2:0)

/*
 * Macros for accessing elements in the variable-format region of the
 *      superblock.  "sbp" is a pointer to the superblock, and "n" gives
 *      the index of the element to be fetched.
 */

/*  FREEino() -- Finds the nth element in the free I-node list.  Each      */
/*      element of the free I-node list is of type "ino_t".                */

#define FREEino(sbp,n) \
	(((uint16_t *)((char *)(sbp)+(sbp)->s_sicino))[n])

/*  FREEblk() -- Finds the nth element in the free block list.  Each       */
/*      element of the free block list is of type "int32_t".               */

#define FREEblk(sbp,n) \
	(((int32_t *)((char *)(sbp)+(sbp)->s_sicfree))[n])

/* we have a NEW Format superblock */
#define	_s_NEWF

#endif

