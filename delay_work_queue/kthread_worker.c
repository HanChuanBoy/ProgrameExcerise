/**********************************************
 * Author: lewiyon@hotmail.com
 * File name: delay_wq.c
 * Description: learn the function queue_delayed_work()
 * Date: 2011-12-24
 *********************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define init_kthread_worker(worker)                    \
do {                                \
        static struct lock_class_key __key;            \
        __init_kthread_worker((worker), "("#worker")->lock", &__key); \
} while (0)

#define init_kthread_work(work, fn)                    \
    do {                                \
        memset((work), 0, sizeof(struct kthread_work));        \
        INIT_LIST_HEAD(&(work)->node);                \
        (work)->func = (fn);                    \
        init_waitqueue_head(&(work)->done);            \
    } while (0)

struct kthread_worker buffer_toggle_worker;
struct task_struct *buffer_toggle_thread;
struct kthread_work buffer_toggle_work;
void osd_toggle_buffer(struct work_struct *work);

void osd_toggle_buffer(struct work_struct *work)
{
    int i;
    printk(KERN_INFO "My name is delay_func!\n");
    for (i = 0; i < 3; i++) {
        printk(KERN_ERR "delay_fun:i=%d\n", i);
        msleep(1000);
    }
}

static int __init example_init(void)
{
    int i;
    int ret;
    init_kthread_worker(&buffer_toggle_worker);
    buffer_toggle_thread = kthread_run(
    kthread_worker_fn,&buffer_toggle_worker,"aml_buf_toggle");
    init_kthread_work(&buffer_toggle_work, osd_toggle_buffer);
    for (i = 0; i < 3; i++) { 
        printk(KERN_INFO "Example:ret= %d,i=%d\n", ret, i);
        msleep(100);
    }
   // queue_kthread_work(&buffer_toggle_worker,&buffer_toggle_work);
    printk(KERN_INFO "second ret=%d!\n", ret);

    return 0;
}

static void __exit example_exit(void)
{
    int ret;
    printk(KERN_INFO "Goodbay! ret=%d\n", ret);
}


module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lewiyon <lewiyon@hotmail.com>");