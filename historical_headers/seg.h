/* @(#)seg.h	7.1 87/06/15 23:35:23 */
#ifndef _h_SEG
#define _h_SEG

#include <sys/types.h>
/*
 * Memory management addresses and bits
 */

#define CR_URO      1   /** segment creation **/
#define CR_URW      2
#define CR_RO       3
#define READ_ACC    1   /** segment access   **/
#define WRITE_ACC   2

/* mapped file and load-seg-register defines */
#define MSTART 0
#define MHOLE 1
#define MDATA 2
#define MRANGE (NBPC/8 - 16) /* makes sizeof(struct mprsvc) = NBPC !! */
#define MF_CREAT 040000
#define MFS_WN		0x01	/* mapfile svc write-new */ 	
#define MFS_RW		0x02	/* mapfile svc r/w */ 	
#define MFS_CW		0x03	/* mapfile svc copy-on-write */ 	
#define MFS_ZCW		0x13
/*
 * mrdwri() turns MFS_ZCW into MFS_CW, when MFS_ZCW is used zero pages
 * are not read protected (for mapping text and data pages)
 */

#define LSR_PRIV 0		/* unprivilege bit clear */
#define LSR_UNPRIV 1		/* unprivilege bit set */
#define LSRPROT 3
#define UBprot  0       /** U block protection: user nothing, kernel r/w **/
/*
 * defines to be used as the third arguments to S_PP SVC
 */
#define PPKRWU__	0
#define PPKRWUR_	1
#define PPKRWURW	2
#define PPKR_UR_	3
#define PPK__U__	4

#define NSEGS   16
#define SEGSHIFT 28
/** BASE is lowest addr in seg, TOP is highest, 0x??FFFFFF      **/
#define BASE(seg) ((seg)<<SEGSHIFT)
#define TOP(seg)  ((BASE(seg+1))-1)
#define LASTLONG(seg)	((BASE(seg+1))-4)

#define KERNELSEG  0
#define TEXTSEG    1
#define DATASEG    2
#define STACKSEG   3
#define IOSEG      15

/** which way does the segment grow? **/
#define TOPDOWN         1
#define BOTTOMUP        0

/** grow stack in chunks less than this (arbitrary) amount **/
#define ABSURD     (16*1024*1024)

/** struct for create-segment VRM call **/
struct crseg {                  /** create segment      **/
	short   c_segid;        /** filled in by VRM    **/
	char    c_grow;
	char    c_access;
	long    c_length;
};

/** number of segs not reserved for the kernel or for VRM 9/83 **/
#define USERSEGS   14
#define NSEGS   16

/** struct for load-segment-register VRM call **/
struct seg_list {
	int	count;		    /* # entries in array actually used */
	struct seg_spec {
		short  seg_id;	    /* segment identifier */
		char   seg_reg;     /* segment register number */
		char   seg_prot;    /* segment protection, usually LSRPROT */
	} p_segids[16];
};

struct lps {            /* saved/restored part of psb */
	unsigned    lps_iar;
	unsigned    lps_ics;
	ushort	    lps_cs;
	ushort	    lps_f1;	    /** filler; see svctrap (low.s) **/
	unsigned    lps_niar;
	ushort      lps_nics;       /** new ics **/
	ushort      lps_stflags;    /** status flags **/
	unsigned    lps_stat;
};
#define lps_mq	lps_niar

struct sbound_u {               /* unshared segments 0-3, 14-15 */
	unsigned sbu_sfirst;
	unsigned sbu_slast;
	int sbu_srw;
};

struct segoffs {
	unsigned so_first;
	unsigned so_last;
	short so_segid;
};

struct sbound_s {              /* shared segments 4-13 */
	struct segoffs *sbs_segoffs;
	unsigned sbs_sbase;
	int sbs_srw;
};

struct segbound {               /* user access descriptor */
	union {
		struct sbound_u sbu;
		struct sbound_s sbs;
	} u_segbnds;
};

#define u_sfirst        u_segbnds.sbu.sbu_sfirst
#define u_slast         u_segbnds.sbu.sbu_slast
#define u_srw           u_segbnds.sbu.sbu_srw
#define u_segbase       u_segbnds.sbs.sbs_sbase
#define u_segoffs       u_segbnds.sbs.sbs_segoffs
#define u_ofirst        u_segbnds.sbs.sbs_segoffs->so_first
#define u_olast         u_segbnds.sbs.sbs_segoffs->so_last

struct cw_ds {			/* for copy on write files */
	short	cw_cnt;
	short	cw_segid;
	struct	inode *cw_ip;
	struct	cw_ds *cw_link;
	long	cw_length;	/* maybe don't need this */
};

/*
 * NCWSEGS to be accessed only in conf.c
 * use ncwsegs otherwise
 */
#define NCWSEGS (NFILE/4)
extern int ncwsegs;

struct segstate {
	ushort	segflag;
	ushort	segfileno;
	union {
		struct vnode *vptr;
		struct shmid_ds *shmptr;
		struct cw_ds *cwptr;
		struct shlib *shlib;
	} u_ptrs;
};
#define shm_ptr u_ptrs.shmptr
#define v_ptr   u_ptrs.vptr
#define cw_ptr	u_ptrs.cwptr
#define sl_ptr u_ptrs.shlib

#define SEG_AVAIL       0x00
#define SEG_KERNEL      0x01
#define SEG_PRIVATE     0x02
#define SEG_SHARED      0x04
#define SEG_MAPPED      0x08
#define SEG_COPYWRT     0x10
#define SEG_SHRTEXT     0x20
#define SEG_MRDWR	0x40
#define SEG_SHLIB       0x80

#define NOTMAPPED  (-1)
#define SHLIBMAP   (-2)

struct pfwchan {
	long	pf_hadr;
	long	pf_ladr;
	short	pf_clstat;	/* status of a page fault clear intr. */
	char	pf_flg;
	char	pf_cnt;
};
#endif

