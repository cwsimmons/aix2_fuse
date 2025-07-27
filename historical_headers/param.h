/* @(#)param.h  7.3 87/11/11 19:51:46 */
/*
 * BSD style param constants for use in AIX
 *      NOTE: includes AIX param.h
 */
/* COPYRIGHT 1987 IBM CORPORATION */
/*      NOTE: includes AIX param.h */
/* BEGIN Include of AIX /usr/include/sys/param.h */
/* @(#)param.h  7.2 87/07/08 10:37:27 */
#ifndef _h_PARAM
#define _h_PARAM
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/seg.h>
#include <sys/pri.h>
#include <sys/low.h>
/*
 * fundamental variables
 * don't change too often
 */
#define NOFILE  200             /* max open files per process */
#define MAXPID  30000           /* max process id */
#define MAXUID  60000           /* max user id */
#define MAXLINK 32767           /* max links (max signed short value) */
#define PAGESIZE 2048           /** KIMONO memory page in bytes **/
#define MAXBLK  500             /* max blocks possible for phys IO */
#define MAXSEG  (128*1024)      /* max seg size (in clicks) */
/* default max data seg size is linear fn of avail pg space at boot time */
#define DEFSTACK 512            /* default max stack size (in clicks) */
#define SSIZE   8               /* initial stack size (in clicks) */
#define SINCR   1               /* increment of stack (in clicks) */
#define USIZE   3               /* size of user block (in clicks) */
#define USRSTACK (1+TOP(STACKSEG)-ctob(USIZE))       /* Start of user stack */
#define CANBSIZ 256             /* max size of typewriter line  */
#define HZ      60              /* Ticks/second of the clock */
#define NCARGS  16384           /* # characters in exec arglist */
#define GRPMAP  (128)           /* multiple group bit map; power of 2 */
/*
 * fundamental constants of the implementation--
 * cannot be changed easily
 */
#define NBPW    sizeof(int)     /* number of bytes in an integer */
#define NBPB     (8)            /* number of bits in a byte      */
#define BPBSHIFT (3)            /* log2( NBPB)                   */
#ifndef FsTYPE
#define FsTYPE  4               /** kimono hybrid **/
#endif
#if FsTYPE==1
        /* Original 512 byte file system */
#define BSIZE   512             /* size of file system block (bytes) */
#define SBUFSIZE        BSIZE   /* system buffer size */
#define BSHIFT  9               /* LOG2(BSIZE) */
#define NINDIR  (BSIZE/sizeof(daddr_t))
#define BMASK   0777            /* BSIZE-1 */
#define INOPB   8               /* inodes per block */
#define INOSHIFT        3       /* LOG2(INOPB) if exact */
#define NMASK   0177            /* NINDIR-1 */
#define NSHIFT  7               /* LOG2(NINDIR) */
#define FsBSIZE(dev)    BSIZE
#define FsBSHIFT(dev)   BSHIFT
#define FsNINDIR(dev)   NINDIR
#define FsBMASK(dev)    BMASK
#define FsINOPB(dev)    INOPB
#define FsLTOP(dev, b)  b
#define FsPTOL(dev, b)  b
#define FsNMASK(dev)    NMASK
#define FsNSHIFT(dev)   NSHIFT
#define FsITOD(dev, x)  itod(x)
#define FsITOO(dev, x)  itoo(x)
#endif
#if FsTYPE==2
        /* New 1024 byte file system */
/** TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP **/
#define Fs2BLK  0x2000          /** added here because mount won't compile **/
                                /** without it. formerly only              **/
                                /** in FsTYPE==3                           **/
#define BSIZE   1024            /* size of file system block (bytes) */
#define SBUFSIZE        BSIZE   /* system buffer size */
#define BSHIFT  10              /* LOG2(BSIZE) */
#define NINDIR  (BSIZE/sizeof(daddr_t))
#define BMASK   01777           /* BSIZE-1 */
#define INOPB   16              /* inodes per block */
#define INOSHIFT        4       /* LOG2(INOPB) if exact */
#define NMASK   0377            /* NINDIR-1 */
#define NSHIFT  8               /* LOG2(NINDIR) */
#define FsBSIZE(dev)    BSIZE
#define FsBSHIFT(dev)   BSHIFT
#define FsNINDIR(dev)   NINDIR
#define FsBMASK(dev)    BMASK
#define FsINOPB(dev)    INOPB
#define FsLTOP(dev, b)  (b<<1)
#define FsPTOL(dev, b)  (b>>1)
#define FsNMASK(dev)    NMASK
#define FsNSHIFT(dev)   NSHIFT
#define FsITOD(dev, x)  itod(x)
#define FsITOO(dev, x)  itoo(x)
#define FsINOS(dev, x)  (((x)&~07)+1)
#define FsBOFF(dev, x)  ((x)&01777)
#define FsBNO(dev, x)   ((x)>>10)
#endif
#if FsTYPE==3
        /* Dual system */
#define BSIZE   512             /* size of file system block (bytes) */
#define SBUFSIZE        (BSIZE*2)       /* system buffer size */
#define BSHIFT  9               /* LOG2(BSIZE) */
#define NINDIR  (BSIZE/sizeof(daddr_t))
#define BMASK   0777            /* BSIZE-1 */
#define INOPB   8               /* inodes per block */
#define INOSHIFT        3       /* LOG2(INOPB) if exact */
#define NMASK   0177            /* NINDIR-1 */
#define NSHIFT  7               /* LOG2(NINDIR) */
#define Fs2BLK  0x2000
#define FsLRG(dev)      (dev&Fs2BLK)
#define FsBSIZE(dev)    (FsLRG(dev) ? (BSIZE*2) : BSIZE)
#define FsBSHIFT(dev)   (FsLRG(dev) ? 10 : 9)
#define FsNINDIR(dev)   (FsLRG(dev) ? 256 : 128)
#define FsBMASK(dev)    (FsLRG(dev) ? 01777 : 0777)
#define FsBOFF(dev, x)  (FsLRG(dev) ? ((x)&01777) : ((x)&0777))
#define FsBNO(dev, x)   (FsLRG(dev) ? ((x)>>10) : ((x)>>9))
#define FsINOPB(dev)    (FsLRG(dev) ? 16 : 8)
#define FsLTOP(dev, b)  (FsLRG(dev) ? b<<1 : b)
#define FsPTOL(dev, b)  (FsLRG(dev) ? b>>1 : b)
#define FsNMASK(dev)    (FsLRG(dev) ? 0377 : 0177)
#define FsNSHIFT(dev)   (FsLRG(dev) ? 8 : 7)
#define FsITOD(dev, x)  (daddr_t)(FsLRG(dev) ? \
        ((unsigned)x+(2*16-1))>>4 : ((unsigned)x+(2*8-1))>>3)
#define FsITOO(dev, x)  (daddr_t)(FsLRG(dev) ? \
        ((unsigned)x+(2*16-1))&017 : ((unsigned)x+(2*8-1))&07)
#define FsINOS(dev, x)  (FsLRG(dev) ? \
        ((x&~017)+1) : ((x&~07)+1))
#endif
#if FsTYPE==4
        /* Dual system with 512/2048 sizes and clustering done by the VRM */
#define BSIZE   2048            /* size of file system block (bytes) */
#define SBUFSIZE        (BSIZE)       /* system buffer size */
#define BSHIFT  11               /* LOG2(BSIZE) */
#define NINDIR  (BSIZE/sizeof(daddr_t))
#define BMASK   03777            /* BSIZE-1 */
#define INOPB   32              /* inodes per block */
#define INOSHIFT        5       /* LOG2(INOPB) if exact */
#define NMASK   0777            /* NINDIR-1 */
#define NSHIFT  9               /* LOG2(NINDIR) */
#define Fs1BLK  0x4000          /* 2 bits for Fs info, 6 for major */
#define Fsi86   0x8000          /* Is it a PC/IX (i.e. byte-swapped) Fs */
#define FsLRG(dev)      (!(dev&Fs1BLK))
#define FsSWAP(dev)     ((dev)&Fsi86)
#define FsBSIZE(dev)    (FsLRG(dev) ? 2048 : 512)
#define FsBSHIFT(dev)   (FsLRG(dev) ? 11 : 9)
#define FsNINDIR(dev)   (FsLRG(dev) ? 512 : 128)
#define FsBMASK(dev)    (FsLRG(dev) ? 03777 : 0777)
#define FsBOFF(dev, x)  (FsLRG(dev) ? ((x)&03777) : ((x)&0777))
#define FsBNO(dev, x)   (FsLRG(dev) ? ((x)>>11) : ((x)>>9))
#define FsINOPB(dev)    (FsLRG(dev) ? 32 : 8)
#define FsLTOP(dev, b)  (FsLRG(dev) ? b<<2 : b)
#define FsPTOL(dev, b)  (FsLRG(dev) ? b>>2 : b)
#define FsNMASK(dev)    (FsLRG(dev) ? 0777 : 0177)
#define FsNSHIFT(dev)   (FsLRG(dev) ? 9 : 7)
#define FsITOD(dev, x)  (daddr_t)(FsLRG(dev) ? \
        ((unsigned)x+(2*32-1))>>5 : ((unsigned)x+(2*8-1))>>3)
#define FsITOO(dev, x)  (daddr_t)(FsLRG(dev) ? \
        ((unsigned)x+(2*32-1))&037 : ((unsigned)x+(2*8-1))&07)
#define FsINOS(dev, x)  (FsLRG(dev) ? \
        ((x&~037)+1) : ((x&~07)+1))
#endif
/*
 * UBSIZE: Units for communication with user in ulimit, ustat.
 */
#define UBSIZE  512             /* bytes in a "user block" (<= BSIZE) */
#define UBSHIFT 9               /* LOG2(UBSIZE) */
#define NICFREE 50              /* number of superblock free blocks */
#define NCPS    1               /* Number of clicks per segment */
#define NBPC    2048            /* Number of bytes per click */
#define NCPD    1               /* Number of clicks per disk block */
#define BPCSHIFT       11       /* LOG2(NBPC) if exact */
#ifndef NULL
#define NULL    0
#endif
#define GOOD    0
#define BAD     -1
#define MAXPATH 512             /* maximum pathlength for mount args */
#define CMASK   0               /* default mask for file creation */
#define CDLIMIT (1L<<13)        /* default max write address */
#define NODEV   (dev_t)(-1)
#define ROOTINO ((ino_t)2)      /* i number of all roots */
#define SUPERB  ((daddr_t)1)    /* physical block number of the super block */
#define SUPERBOFF(dev)  (FsLRG(dev) ? BSIZE : SBUFSIZE)
    /* byte offset of the super block */
#ifndef DIRSIZ
#define DIRSIZ  14              /* max characters per directory */
#endif
#define NICINOD 100             /* number of superblock inodes */
#define CLKTICK 20408           /* microseconds in a clock tick (49 HZ) */
#define USERMODE(ics)    (ics & 0x04)
#define KERNELMODE(ics)  (!USERMODE(ics))
#define BASEPRI()       (PS == 7)       /** really ??????? **/
#define BUSOPEN(ics)       (ics & 0x18)
    /* true if user has /dev/bus open */
#define lobyte(X)       (((unsigned char *)&X)[1])
#define hibyte(X)       (((unsigned char *)&X)[0])
#define loword(X)       (((ushort *)&X)[1])
#define hiword(X)       (((ushort *)&X)[0])
#endif
/* END Include of AIX /usr/include/sys/param.h */
/* for AIX sockets */
/* MBUF CLUSTER defines, totally unlike the vax */
#define NBPG            PAGESIZE
#define CLSIZE          1                       /* one cluster per page */
#define CLBYTES         (CLSIZE*PAGESIZE)       /* PAGESIZE from AIX */
#define CLOFSET         (CLSIZE*PAGESIZE-1)     /* for clusters, like PGOFSET */
#define claligned(x)    ((((int)(x))&CLOFSET)==0)
#define CLOFF           CLOFSET
#define CLSIZELOG2      0                       /* CLSIZE == PAGESIZE */
#define CLSHIFT         (BPCSHIFT+CLSIZELOG2)   /* Bytes per Click shift AIX */
#if CLSIZE==1
#define clbase(i)       (i)
#define clrnd(i)        (i)
#else
/* give the base virtual address (first of CLSIZE) */
#define clbase(i)       ((i) &~ (CLSIZE-1))
/* round a number of clicks up to a whole cluster */
#define clrnd(i)        (((i) + (CLSIZE-1)) &~ (CLSIZE-1))
#endif
/*
 * bit map related macros
 */
#define setbit(a,i)     ((a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define clrbit(a,i)     ((a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define isset(a,i)      ((a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define isclr(a,i)      (((a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)
/*
 * Macros for fast min/max.
 */
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/*
 * Macros for counting and rounding.
 */
#ifndef howmany
#define howmany(x, y)   (((x)+((y)-1))/(y))
#endif
#define roundup(x, y)   ((((x)+((y)-1))/(y))*(y))
/*
 * Maximum size of hostname recognized and stored in the kernel.
 */
#define MAXHOSTNAMELEN  32      /* see AIX <sys/utsname.h> SYS_NMLN */
/* max length of a file name a la BSD */
#define MAXPATHLEN      1024
/* BSD param.h also includes signal.h and types.h */
#ifndef KERNEL
#include <sys/signal.h>
#include <sys/types.h>
#endif