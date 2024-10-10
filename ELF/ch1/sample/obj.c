#include<linux/init.h>
#include <linux/module.h>
#include"obj.h"
MODULE_LICENSE("Dual BSD/GPL");
void testAddssss(void){
    for(int i=0;i<2;i++){

        printk(KERN_ALERT "Hello,hahahahahaha world\n");
    }
    return ;
}
