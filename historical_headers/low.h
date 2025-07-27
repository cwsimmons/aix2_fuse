/* @(#)low.h	7.1 87/06/15 23:42:24 */
#ifndef _h_LOW
#define _h_LOW

#include <sys/types.h>

/*
 * VM page 0 data - these definitions should be used only if defining
 * in low.s and in a .h (tracechan is an example) runs too slow.
 * For a more complete map of page 0 see VRM Tech Ref.
 */

#define STATUS_SAVE (*(uint *)  0xB4)  /* FPA1/2 status register */
#define FREESLTS (*(uint *)     0xB8)   /* free paging disk slots */
#define PGCYCLES (*(uint *)     0xBC)   /* page replacement cycles */
#define PGFAULTS (*(uint *)     0xC0)   /* page faults w/ preemption */
#define NONPDIOS (*(uint *)     0xC4)   /* non-paging disk IOs */

#define FPTYPE  (*(char *)      0xCC)   /* FPA type, set by VRM */
#define IRP     (*(ushort *)    0xD4)

#define PS      (*(ushort *)    0xE2)
#define ELEV    (*(short *)     0xE4)   /* execution level */
#define VMID    (*(short *)     0xE6)   /* virtual machine id */

#define RTIMER  (*(time_t *)    0xF0)

#define TIMEINV (*(long  *)     0xF8)

#define CTABPTR (*(long  *)    0xC80)   /* ptr to default collate table */
#define CAAAPTR (*(long  *)    0xC84)   /* ptr to spare table 1         */
#define CBBBPTR (*(long  *)    0xC88)   /* ptr to spare table 2         */
#define CCCCPTR (*(long  *)    0xC8c)   /* ptr to spare table 3         */
#define CDDDPTR (*(long  *)    0xC90)   /* ptr to spare table 4         */

#endif
