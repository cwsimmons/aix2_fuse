/* @(#)types.h  7.4 87/12/07 10:06:52 */
/*
 * BSD to AIX <sys/types.h> compatibility include file
 * COPYRIGHT 1987 IBM CORP.
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *      @(#)types.h     7.1 (Berkeley) 6/4/86
 */

#ifndef _TYPES_
#define _TYPES_
/* typedefs for BSD unsigned things */
/* NOTE: please also include <sys/types.h> to pick up AIX types */
typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;
/*** DELETED some types that are in AIX <sys/types.h> */
typedef struct  _quad { long val[2]; } quad;
typedef long    swblk_t;
typedef long    size_t;
typedef unsigned short  uid_t;
typedef unsigned short  gid_t;
#define NBBY    8               /* number of bits in a byte */
/*
 * Select uses bit masks of file descriptors in longs.
 * These macros manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here
 * should be >= NOFILE (param.h).
 */
#ifndef FD_SETSIZE
#define FD_SETSIZE      256
#endif
typedef long    fd_mask;
#define NFDBITS (sizeof(fd_mask) * NBBY)        /* bits per mask */
#ifndef howmany
#define howmany(x, y)   (((x)+((y)-1))/(y))
#endif
typedef struct fd_set {
        fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;
#define FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)      ((void)memset((char *)(p), 0, sizeof(*(p))))
/*
 * added for general collecting of stuff
 * interrupt level controls
 */
#define splnet  spl6
#define splimp  spl6
/* XXX WARNING -- including AIX sys/types.h */
/* BEGIN Include of AIX /usr/include/sys/types.h */
/* @(#)types.h  7.1 87/06/15 23:40:18 */
#ifndef _h_TYPES
#define _h_TYPES
typedef struct { int r[1]; } *  physadr;
typedef long            level_t;
typedef long            daddr_t;
typedef char *          caddr_t;
typedef unsigned int    uint;
typedef unsigned short  ushort;
typedef unsigned long   ulong;
typedef ushort          ino_t;
typedef short           cnt_t;
typedef long            time_t;
typedef int             label_t[11];  /** regs r5 - r15 **/
typedef int             dev_t;
typedef long            off_t;
typedef long            paddr_t;
typedef long            key_t;
/*
 * Types of vnodes supported by
 * Distributed Services.
 */
enum vtype      { VDIR, VCHR, VBLK, VREG, VMPC, VFIFO, VBAD, VUNDEF } ;
typedef enum vtype      vtype;
/*
 * Possible results of Distributed
 * Services uid/gid reverse mapping.
 */
enum tagtype    { CALLER, OTHER, SOMEONE, NO_ONE };
typedef enum tagtype    tagtype;
#ifndef NULL
#define NULL    0
#endif
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif
#endif
/* END Include of AIX /usr/include/sys/types.h */
#endif