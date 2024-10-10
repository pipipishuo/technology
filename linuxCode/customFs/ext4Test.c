/*
 * Demonstrate a trivial filesystem using libfs.
 *
 * Copyright 2002, 2003 Jonathan Corbet <corbet@lwn.net>
 * This file may be redistributed under the terms of the GNU GPL.
 *
 * Chances are that this code will crash your system, delete your
 * nethack high scores, and set your disk drives on fire.  You have
 * been warned.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pagemap.h> 	/* PAGE_CACHE_SIZE */
#include <linux/fs.h>     	/* This is where libfs stuff is declared */
#include <asm/atomic.h>
#include <asm/uaccess.h>	/* copy_to_user */
struct inode *cs_iget(struct super_block *super, unsigned long ino);
static struct dentry *cs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data);
static int cs_fill_super(struct super_block *s, void *d, int silent);

static struct file_system_type ext4_fs_type = {
	.owner			= THIS_MODULE,
	.name			= "ext4CS",
	//.init_fs_context= ext4_init_fs_context,
	//.parameters		= ext4_param_specs,
	.mount          =cs_mount,
    .kill_sb		= kill_block_super,
	.fs_flags		= FS_REQUIRES_DEV | FS_ALLOW_IDMAP,
};
static struct dentry *cs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data, cs_fill_super);
}

static int cs_fill_super(struct super_block *s, void *d, int silent)
{
	struct inode *root;
	s->s_time_min = 0;
	s->s_time_max = U32_MAX;
	s->s_magic		= EFS_SUPER_MAGIC;		
	root = cs_iget(s,FS_STATINODE);
	if (IS_ERR(root)) {
		pr_err("get root inode failed\n");
		return PTR_ERR(root);
	}
	s->s_root = d_make_root(root);
	if (!(s->s_root)) {
		pr_err("get root dentry failed\n");
		return -ENOMEM;
	}
	return 0;
}

struct inode *cs_iget(struct super_block *super, unsigned long ino)
{
	int i, inode_index;
	dev_t device;
	u32 rdev;
	struct inode *inode;
	inode = iget_locked(super, ino);
	if (!inode)
		return ERR_PTR(-ENOMEM);
	if (!(inode->i_state & I_NEW))
		return inode;

    inode->i_fop = &generic_ro_fops;
	unlock_new_inode(inode);
	return inode;

read_inode_error:
	pr_warn("failed to read inode %lu\n", inode->i_ino);
	iget_failed(inode);
	return ERR_PTR(-EIO);
}
static int __init ext4_init_fs(void)
{
    int err = register_filesystem(&ext4_fs_type);


	return 0;
}
static void __exit ext4_exit_fs(void)
{
}
MODULE_LICENSE("GPL");
module_init(ext4_init_fs);
module_exit(ext4_exit_fs);

