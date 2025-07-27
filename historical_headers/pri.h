/* @(#)pri.h	7.2 87/09/30 14:16:40 */
#ifndef _h_PRI
#define _h_PRI
#include        <sys/proc.h>

#define NZERO           20      /* default nice value */
#define PMASK   0177
#define PCATCH  0400            /* must not be equal to SWAKEONSIG */

/* upwardly compatible sleep macro: */
#define SLPDEF(C,D) (sleepx((C),(D)&PMASK,(D)&PCATCH|(((D)&PMASK)>PZERO?SWAKEONSIG:0)))
#ifdef KPROCS
/* There is an excuse for this hack. */
#define sleep(C, D)     SLPDEF(C, D)
#endif

/*
 * priorities
 * should not be altered too much
 * changing a priority's relationship (< = >) to
 * PSWP, PINOD, PRIBIO, PZERO, PUSER, or PIDLE is "risky".
 */

#define PSWP            0       /* "swapping" */
#define PGKILLPRI       1       /* disaster recovery from pg space shortage */
#define PINOD           10      /* inode locks */
#define HFTNPRI         11      /* locator overrun */
#define PRIPGFLT        16      /* page fault */
#define PRIDDMA         19
#define PRIBIO          20
#define PRIPBIO         20      /* physio */
#define TAPEPRI         20
#define FDPRI           20      /* floppy */
#define PRIDMA          20      /* "buffer bypass" */
#define SYNCPRI         20
#define PRIFBIO         21
#define PRIFDMA         21
#define FSYNCPRI        21
#define TRACEXPRI       24
#define PDELAY          24      /* delay() in clock.c */
#define ERRXPRI         24
#define PTRACEPRI       25
#define PZERO           25      /* if >PZERO signals interrupt the sleep */
				/* above comment no longer true! see
				   SWAKEONSIG and sleepx() for details   */
#define PPIPE           26
#define ERRPRI          26      /* error logging */
#define PSHM            27      /* shared memory */
#define PMSG            27      /* ipc messages */
#define PSEMZ           27      /* ipc semaphores */
#define PSEMN           28
#define HFTPRI          29      /* high fn terminal */
#define TTIPRI          30      /* tty input */
#define RSIPRI          30      /* rs232 input */
#define TTOPRI          31      /* tty output */
#define RSOPRI          31      /* rs232 output */
#define TOKPRI          32      /* token ring - AIX block I/O device driver */
#define PWAIT           33
#define TRACEPRI        35
#define LPPRI           36      /* printer */
#define OSMPRI          37      /* os messages */
#define PSLEP           39      /* obsolete? */
#define PAUSEPRI        39
#define SPAUSEPRI       39      /* sigpause */
#define LOCKFPRI        39
#define ENPRI           60      /* ethernet */
#define PUSER           60
#define PIDLE           127     /* == PMASK */

#endif
