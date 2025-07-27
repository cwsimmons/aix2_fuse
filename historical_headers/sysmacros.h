/* @(#)sysmacros.h	7.1 87/06/15 23:39:22 */

#ifndef _h_SYSMACROS
#define _h_SYSMACROS

#include <sys/param.h>

/*
 * Some macros for units conversion
 */

/* Core clicks to segments and vice versa */
#if NCPS == 1
#define ctos(x) (x)
#define stoc(x) (x)
#else
#define ctos(x) (((x)+(NCPS-1))/NCPS)
#define	stoc(x) ((x)*NCPS)
#endif

/* Core clicks to disk blocks */
#if NCPD == 1
#define ctod(x) (x)
#else
#define ctod(x) (((x)+(NCPD-1))/NCPD)
#endif
#define bltoc(x) ((x)*NCPD)

/* inumber to disk address */
#ifdef INOSHIFT
#define itod(x) (daddr_t)(((unsigned)(x)+(2*INOPB-1))>>INOSHIFT)
#else
#define itod(x) (daddr_t)(((unsigned)(x)+(2*INOPB-1))/INOPB)
#endif

/* inumber to disk offset */
#ifdef INOSHIFT
#define itoo(x) (int)(((unsigned)(x)+(2*INOPB-1))&(INOPB-1))
#else
#define itoo(x) (int)(((unsigned)(x)+(2*INOPB-1))%INOPB)
#endif

/* clicks to bytes */
#ifdef BPCSHIFT
#define	ctob(x)	((x)<<BPCSHIFT)
#else
#define	ctob(x)	((x)*NBPC)
#endif

/* bytes to clicks */
#ifdef BPCSHIFT
#define	btoc(x)	(((unsigned)(x)+(NBPC-1))>>BPCSHIFT)
#define	btoct(x)	((unsigned)(x)>>BPCSHIFT)
#else
#define	btoc(x)	(((unsigned)(x)+(NBPC-1))/NBPC)
#define	btoct(x)	((unsigned)(x)/NBPC)
#endif

/* major part of a device */
#define major(x)        (int)((unsigned)(x)>>8)
#define	bmajor(x)	(int)(((unsigned)x>>8)&037)
#define	brdev(x)	(x&0x1fff)

/* minor part of a device */
#define minor(x)        (int)((x)&0377)

/* make a device number */
#define	makedev(x,y)	(dev_t)(((x)<<8) | (y))

#endif
