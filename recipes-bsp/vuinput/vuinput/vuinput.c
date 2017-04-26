#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/ioctl.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,1)
#include <linux/seq_file.h>
#endif

static struct proc_dir_entry *proc_vuinput2;
static struct proc_dir_entry *proc_vuinput3;

DEFINE_MUTEX(vuinput_table_mutex);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,1)
static int vuinput_read_proc (char *page, char **start, off_t off, int count, int *eof, void *data_unused)
{
	int len;
	off_t   begin = 0;

	mutex_lock(&vuinput_table_mutex);
	len = sprintf(page, "FIXME!\n");
	mutex_unlock(&vuinput_table_mutex);

	if (off >= len+begin)
		return 0;
	*start = page + (off-begin);
	return ((count < begin+len-off) ? count : begin+len-off);
}

static int __init init_vuinput(void)
{
	if ((proc_vuinput2 = create_proc_entry( "stb/tsmux/input2_choices", 0666, NULL )))
	proc_vuinput2->read_proc = vuinput_read_proc;

	if ((proc_vuinput3 = create_proc_entry( "stb/tsmux/input3_choices", 0666, NULL )))
	proc_vuinput3->read_proc = vuinput_read_proc;

        return 0;
}

static void __exit cleanup_vuinput(void)
{
	remove_proc_entry( "stb/tsmux/input2_choices", NULL);
	remove_proc_entry( "stb/tsmux/input3_choices", NULL);
}
#else
static int proc_vuinput_show(struct seq_file *seq, void *v)
{
        off_t   begin = 0;
        seq_printf(seq, "FIXME!\n");
        return 0;
}

int proc_vuinput_open(struct inode *inode, struct file *file)
{ 
	return single_open(file, proc_vuinput_show, PDE_DATA(inode));
}

static const struct file_operations proc_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_vuinput_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init init_vuinput(void)
{
	proc_vuinput2 = proc_create_data( "stb/tsmux/input2_choices", 0666, NULL, &proc_fops, NULL );
	proc_vuinput3 = proc_create_data( "stb/tsmux/input3_choices", 0666, NULL, &proc_fops, NULL );
	return 0;
}

static void __exit cleanup_vuinput(void)
{
        remove_proc_entry( "stb/tsmux/input2_choices", NULL);
	remove_proc_entry( "stb/tsmux/input3_choices", NULL);
}
#endif
module_init(init_vuinput);
module_exit(cleanup_vuinput);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OpenPLi");
MODULE_DESCRIPTION("Add missing stb/tsmux/input2_choices proc helper");
