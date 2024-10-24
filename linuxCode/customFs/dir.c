// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/isofs/dir.c
 *
 *  (C) 1992, 1993, 1994  Eric Youngdale Modified for ISO 9660 filesystem.
 *
 *  (C) 1991  Linus Torvalds - minix filesystem
 *
 *  Steve Beynon		       : Missing last directory entries fixed
 *  (stephen@askone.demon.co.uk)      : 21st June 1996
 *
 *  isofs directory handling functions
 */
#include <linux/gfp.h>
#include "isofs.h"

int isofs_name_translate(struct iso_directory_record *de, char *new, struct inode *inode)
{
	char * old = de->name;
	int len = de->name_len[0];
	int i;

	for (i = 0; i < len; i++) {
		unsigned char c = old[i];
		if (!c)
			break;

		if (c >= 'A' && c <= 'Z')
			c |= 0x20;	/* lower case */

		/* Drop trailing '.;1' (ISO 9660:1988 7.5.1 requires period) */
		if (c == '.' && i == len - 3 && old[i + 1] == ';' && old[i + 2] == '1')
			break;

		/* Drop trailing ';1' */
		if (c == ';' && i == len - 2 && old[i + 1] == '1')
			break;

		/* Convert remaining ';' to '.' */
		/* Also '/' to '.' (broken Acorn-generated ISO9660 images) */
		if (c == ';' || c == '/')
			c = '.';

		new[i] = c;
	}
	return i;
}

/* Acorn extensions written by Matthew Wilcox <willy@infradead.org> 1998 */
int get_acorn_filename(struct iso_directory_record *de,
			    char *retname, struct inode *inode)
{
	int std;
	unsigned char *chr;
	int retnamlen = isofs_name_translate(de, retname, inode);

	if (retnamlen == 0)
		return 0;
	std = sizeof(struct iso_directory_record) + de->name_len[0];
	if (std & 1)
		std++;
	if (de->length[0] - std != 32)
		return retnamlen;
	chr = ((unsigned char *) de) + std;
	if (strncmp(chr, "ARCHIMEDES", 10))
		return retnamlen;
	if ((*retname == '_') && ((chr[19] & 1) == 1))
		*retname = '!';
	if (((de->flags[0] & 2) == 0) && (chr[13] == 0xff)
		&& ((chr[12] & 0xf0) == 0xf0)) {
		retname[retnamlen] = ',';
		sprintf(retname+retnamlen+1, "%3.3x",
			((chr[12] & 0xf) << 8) | chr[11]);
		retnamlen += 4;
	}
	return retnamlen;
}

/*
 * This should _really_ be cleaned up some day..
 */
unsigned long cs_inode_number = 2;
static int do_isofs_readdir(struct inode *inode, struct file *file,
		struct dir_context *ctx,
		char *tmpname, struct iso_directory_record *tmpde)
{

    while (cs_inode_number<4) {


        int de_len=38;
        cs_inode_number++;
        tmpname[0]='a'+cs_inode_number;
        tmpname[1]='b'+cs_inode_number;
        tmpname[2]='c'+cs_inode_number;
        tmpname[3]='d'+cs_inode_number;
        dir_emit(ctx, tmpname, 4, cs_inode_number, DT_UNKNOWN);
    }

	return 0;
}

/*
 * Handle allocation of temporary space for name translation and
 * handling split directory entries.. The real work is done by
 * "do_isofs_readdir()".
 */
static int isofs_readdir(struct file *file, struct dir_context *ctx)
{
	int result;
	char *tmpname;
	struct iso_directory_record *tmpde;
	struct inode *inode = file_inode(file);

	tmpname = (char *)__get_free_page(GFP_KERNEL);
	if (tmpname == NULL)
		return -ENOMEM;

	tmpde = (struct iso_directory_record *) (tmpname+1024);

	result = do_isofs_readdir(inode, file, ctx, tmpname, tmpde);

	free_page((unsigned long) tmpname);
	return result;
}

const struct file_operations isofs_dir_operations =
{
	.llseek = generic_file_llseek,
	.read = generic_read_dir,
	.iterate_shared = isofs_readdir,
};

/*
 * directories can handle most operations...
 */
const struct inode_operations isofs_dir_inode_operations =
{
	.lookup = isofs_lookup,
};


