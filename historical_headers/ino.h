/* @(#)ino.h	7.1 - 87/06/15 - 23:36:16 */
#ifndef _h_INO
#define _h_INO

#include "sys/inode.h"
#define	di_gen	di_addr[NADDR * 3]	/* Should be 39 */

	/* Inode structure as it appears on a disk block. */
struct dinode
{
	ushort di_mode;		/* mode and type of file */
	short	di_nlink;    	/* number of links to file */
	ushort	di_uid;      	/* owner's user id */
	ushort	di_gid;      	/* owner's group id */
	off_t	di_size;     	/* number of bytes in file */
	char  	di_addr[40];	/* disk block addresses */
	time_t	di_atime;   	/* time last accessed */
	time_t	di_mtime;   	/* time last modified */
	time_t	di_ctime;   	/* time created */
};
/*
 * the 40 address bytes:
 *	39 used; 13 addresses
 *	of 3 bytes each.
 */

#endif
