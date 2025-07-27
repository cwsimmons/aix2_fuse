/* @(#)proc.h	7.2 87/09/10 16:08:09 */

#ifndef _h_PROC
#define _h_PROC
#include <sys/seg.h>
/*
 * One structure allocated per active process. It contains all data needed
 * about the process while the process may be swapped out.
 * Other per process data (user.h) is swapped with the process.
 */

struct	proc {
	struct seg_list p_segs; /* segment ids in VRM form */
	int     p_flag;
	char    p_stat;
	char	p_pri;		/* priority, negative is high */
	char	p_time;		/* resident time for scheduling */
	char	p_cpu;		/* cpu usage for scheduling */
	char	p_nice;		/* nice for cpu usage */
	ushort	p_uid;		/* real user id */
	ushort	p_suid;		/* set (effective) user id */
	short	p_pgrp;		/* name of process group leader */
	short	p_pid;		/* unique process id */
	short	p_ppid;		/* process id of parent */
	long    p_addr;
	short	p_size;		/* size of image (clicks) */
	ushort  p_dpid;         /* debugger process id       */
	struct  user  *p_uptr;  /* location of ublock for this process */
	char	p_cursig;
	int	p_sig;		/* signals pending to this process */
	int	p_sigmask;	/* current signal mask */
	int	p_sigignore;	/* signals being ignored */
	int	p_sigcatch;	/* signals being caught by user */
	int     p_sigaction;    /* 1 == Bell style, 0 == 4.2 style */

	union {
		caddr_t	p_cad;
		int	p_int;
	} p_unw;
#define	p_wchan	p_unw.p_cad
#define	p_arg	p_unw.p_int
	struct text *p_textp;	/* pointer to text structure */
	struct proc *p_link;	/* linked list of running processes */
	int	p_clktim;	/* time to alarm clock signal */
	int	p_smbeg;	/* beginning pte entry for shared memory */
	int	p_smend;	/* ending pte entry for shared memory */
	caddr_t p_freg;         /* NULL, FPA regset or ptr to saved regset */
	ulong   p_pflt;         /* # page faults */
	int	p_epid;		/* id of the remote process (for locking) */
	int	p_sysid;	/* nid of the remote process (for locking) */
};

extern struct proc proc[];	/* the proc table itself */

/* stat codes */
#define SNONE   0               /* slot is available */
#define	SSLEEP	1		/* awaiting an event */
#define	SWAIT	2		/* (abandoned state) */
#define	SRUN	3		/* running */
#define	SIDL	4		/* intermediate state in process creation */
#define	SZOMB	5		/* intermediate state in process termination */
#define	SSTOP	6		/* process being traced */
#define SKPROC 10               /* process is an available kernel proc */

/* flag codes */
#define	SLOAD	01		/* in core */
#define	SSYS	02		/* scheduling process */
#define SRSVD1  04              /* unused */
#define SFORK   010		/* a forked process */
#define	STRC	020		/* process is being traced */
#define	SWTED	040		/* another tracing flag */
#define	STEXT	0100		/* text pointer valid */
#define SSEL    0200            /* selecting; wakeup/waiting danger */
#define SRSVD3  0x0000100       /* unused */
#define	SOMASK	0x0000200	/* restore old mask after taking signal */
#define SWAKEONSIG 0x00000400   /* signal will abort the sleep */
#define SRSVD4  0xffff0800      /* all these are unused */
				/* must not be equal to PCATCH! */
#define SMPTRACE  0x00001000    /*multi-processing debugging is active            */
#define STRACING  0x00002000    /*process is an dbx debugger                      */
#define SFWTED    0x00004000    /*child process has stopped while being forked    */
#define SEWTED    0x00008000    /*child process has stopped while exec'ing        */
/*
 * parallel proc structure
 * to replace part with times
 * to be passed to parent process
 * in ZOMBIE state.
 */
#ifndef NPROC
struct	xproc {
	struct seg_list xp_segs;
	int     xp_flag;
	char	xp_stat;
	char	xp_pri;		/* priority, negative is high */
	char	xp_time;	/* resident time for scheduling */
	char	xp_cpu;		/* cpu usage for scheduling */
	char	xp_nice;	/* nice for cpu usage */
	ushort	xp_uid;		/* real user id */
	ushort	xp_suid;	/* set (effective) user id */
	short	xp_pgrp;	/* name of process group leader */
	short	xp_pid;		/* unique process id */
	short	xp_ppid;	/* process id of parent */
	short	xp_xstat;	/* Exit status for wait */
	time_t	xp_utime;	/* user time, this proc */
	time_t	xp_stime;	/* system time, this proc */
};
#endif
#endif
