/* @(#)dir.h	7.1 87/06/15 23:34:48 */
#ifndef _h_DIR
#define _h_DIR

#include <stdint.h>

#ifndef	DIRSIZ
#define	DIRSIZ	14
#endif
struct	direct
{
	uint16_t	d_ino;
	char	d_name[DIRSIZ];
};

#endif
