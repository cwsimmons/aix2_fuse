/* @(#)ino.h	7.1 - 87/06/15 - 23:36:16 */
#ifndef _h_INO
#define _h_INO

#include "inode.h"

#include <stdint.h>

#define	di_gen	di_addr[NADDR * 3]	/* Should be 39 */

	/* Inode structure as it appears on a disk block. */
struct dinode
{
	uint16_t di_mode;		/* mode and type of file */
	int16_t	di_nlink;    	/* number of links to file */
	uint16_t	di_uid;      	/* owner's user id */
	uint16_t	di_gid;      	/* owner's group id */
	int32_t	di_size;     	/* number of bytes in file */
	uint8_t  	di_addr[40];	/* disk block addresses */
	int32_t	di_atime;   	/* time last accessed */
	int32_t	di_mtime;   	/* time last modified */
	int32_t	di_ctime;   	/* time created */
};
/*
 * the 40 address bytes:
 *	39 used; 13 addresses
 *	of 3 bytes each.
 */

#endif
