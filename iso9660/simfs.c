#include <linux/init.h>
#include <linux/module.h>

#include <linux/slab.h>
#include <linux/cred.h>
#include <linux/nls.h>
#include <linux/ctype.h>
#include <linux/statfs.h>
#include <linux/cdrom.h>
#include <linux/mpage.h>
#include <linux/user_namespace.h>
#include <linux/seq_file.h>
#include <linux/blkdev.h>
#include <linux/fs_context.h>
#include <linux/fs_parser.h>
#define ISOFS_SUPER_MAGIC	0x9660
#define MAX_TZ_OFFSET (52*15*60)
struct iso_inode_info{
    int counter;
    struct inode vfs_inode;
};
struct isofs_sb_info {
	unsigned long s_ninodes;
};
struct iso9660_options{
	unsigned int rock:1;
	unsigned int joliet:1;
	unsigned int cruft:1;
	unsigned int hide:1;
	unsigned int showassoc:1;
	unsigned int nocompress:1;
	unsigned int overriderockperm:1;
	unsigned int uid_set:1;
	unsigned int gid_set:1;
	unsigned char map;
	unsigned char check;
	unsigned int blocksize;
	umode_t fmode;
	umode_t dmode;
	kgid_t gid;
	kuid_t uid;
	char *iocharset;
	/* LVE */
	s32 session;
	s32 sbsector;
};

static inline struct isofs_sb_info *ISOFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}
static inline struct iso_inode_info *ISOFS_I(struct inode *inode)
{
	return container_of(inode, struct iso_inode_info, vfs_inode);
}

static struct kmem_cache *isofs_inode_cachep;       //这很好理解存inod用的
static void isofs_free_inode(struct inode *inode)
{
	kmem_cache_free(isofs_inode_cachep, ISOFS_I(inode));
}

static struct inode *isofs_alloc_inode(struct super_block *sb)
{
	struct iso_inode_info *ei;
	ei = alloc_inode_sb(sb, isofs_inode_cachep, GFP_KERNEL);
	if (!ei)
		return NULL;
	return &ei->vfs_inode;
}
static void isofs_put_super(struct super_block *sb)
{
	struct isofs_sb_info *sbi = ISOFS_SB(sb);


	kfree(sbi);
	sb->s_fs_info = NULL;
	return;
}

static int isofs_statfs (struct dentry *dentry, struct kstatfs *buf)
{
	struct super_block *sb = dentry->d_sb;
	u64 id = huge_encode_dev(sb->s_bdev->bd_dev);

	buf->f_type = ISOFS_SUPER_MAGIC;
	buf->f_bsize = sb->s_blocksize;
//	buf->f_blocks = (ISOFS_SB(sb)->s_nzones<< (ISOFS_SB(sb)->s_log_zone_size - sb->s_blocksize_bits));
	buf->f_bfree = 0;
	buf->f_bavail = 0;
	buf->f_files = ISOFS_SB(sb)->s_ninodes;
	buf->f_ffree = 0;
	buf->f_fsid = u64_to_fsid(id);
	buf->f_namelen = NAME_MAX;
	return 0;
}
static int isofs_show_options(struct seq_file *m, struct dentry *root)
{
    return 0;
}
static const struct super_operations isofs_sops = {
	.alloc_inode	= isofs_alloc_inode,
	.free_inode	= isofs_free_inode,
	.put_super	= isofs_put_super,
	.statfs		= isofs_statfs,
	.show_options	= isofs_show_options,
};
struct isofs_iget5_callback_data {
	unsigned long block;
	unsigned long offset;
};
static int isofs_iget5_test(struct inode *ino, void *data)
{

	return 0;
}

static int isofs_iget5_set(struct inode *ino, void *data)
{
	
	return 0;
}
static int isofs_fill_super(struct super_block *s, void *data1, int silent)
{
    unsigned long hashval=0;
    s->s_op = &isofs_sops;
    s->s_magic = ISOFS_SUPER_MAGIC;
    struct isofs_iget5_callback_data data;
    data.block = 0;
	data.offset = 0;
    struct inode *inode=inode = iget5_locked(s, hashval, &isofs_iget5_test,
				&isofs_iget5_set, &data);;
    set_nlink(inode, 1);
    inode->i_uid =  GLOBAL_ROOT_UID;
	inode->i_gid =  GLOBAL_ROOT_GID;
	inode->i_blocks = 0;
    inode->i_size = 0;
    inode->i_mode = S_IFREG;
    inode->i_blocks = (inode->i_size + 511) >> 9;
	/*
	 * With multi-extent files, file size is only limited by the maximum
	 * size of a file system, which is 8 TB.
	 */
	s->s_maxbytes = 0x80000000000LL;

	/* ECMA-119 timestamp from 1900/1/1 with tz offset */
	s->s_time_min = mktime64(1900, 1, 1, 0, 0, 0) - MAX_TZ_OFFSET;
	s->s_time_max = mktime64(U8_MAX+1900, 12, 31, 23, 59, 59) + MAX_TZ_OFFSET;
    
    s->s_root = d_make_root(inode);
	if (!(s->s_root)) {
		int error = -ENOMEM;
		
	}


	return 0;
}
static int isofs_parse_param(struct fs_context *fc,
			       struct fs_parameter *param)
{
    return 0;
}
static void isofs_free_fc(struct fs_context *fc)
{
	kfree(fc->fs_private);
}
static int isofs_reconfigure(struct fs_context *fc)
{
	sync_filesystem(fc->root->d_sb);
	if (!(fc->sb_flags & SB_RDONLY))
		return -EROFS;
	return 0;
}
/*static int isofs_get_tree(struct fs_context *fc)
{
	return get_tree_bdev(fc, isofs_fill_super);
}*/
static const struct fs_context_operations isofs_context_ops = {
	.parse_param	= isofs_parse_param,
	//.get_tree	= isofs_get_tree,
	.reconfigure	= isofs_reconfigure,
	.free		= isofs_free_fc,
};
//头文件先都给我干上
static int isofs_init_fs_context(struct fs_context *fc)
{
	fc->ops = &isofs_context_ops;

	return 0;
}
/*static struct file_system_type iso9660_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "csiso9660",
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
	.init_fs_context = isofs_init_fs_context,
//	.parameters	= isofs_param_spec,
};*/
static struct dentry *isofs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data, isofs_fill_super);
}

static struct file_system_type iso9660_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "csiso9660",
	.mount		= isofs_mount,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};
MODULE_ALIAS_FS("csiso9660");
MODULE_ALIAS("csiso9660");
static void init_once(void *foo)
{
	struct iso_inode_info *ei = foo;

	inode_init_once(&ei->vfs_inode);
}
static int __init init_inodecache(void)
{
	isofs_inode_cachep = kmem_cache_create("isofs_inode_cache",
					sizeof(struct iso_inode_info),
					0, (SLAB_RECLAIM_ACCOUNT|
					SLAB_ACCOUNT),
					init_once);
	if (!isofs_inode_cachep)
		return -ENOMEM;
	return 0;
}
static void destroy_inodecache(void)
{
	/*
	 * Make sure all delayed rcu free inodes are flushed before we
	 * destroy cache.
	 */
	rcu_barrier();
	kmem_cache_destroy(isofs_inode_cachep);
}
static int __init init_iso9660_fs(void)
{
	int err = init_inodecache();
	if (err)
		goto out;
	err = register_filesystem(&iso9660_fs_type);
	if (err)
		goto out2;
	return 0;
out2:
	destroy_inodecache();
out:
	return err;
}

static void __exit exit_iso9660_fs(void)
{
    unregister_filesystem(&iso9660_fs_type);
	destroy_inodecache();
}

module_init(init_iso9660_fs)
module_exit(exit_iso9660_fs)
MODULE_DESCRIPTION("ISO 9660 CDROM file system support");
MODULE_LICENSE("GPL");
