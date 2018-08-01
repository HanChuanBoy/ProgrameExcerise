ifneq  ($(KERNELRELEASE),)
#编译的时候只需要修改obj-m：=为目标文件对应;
obj-m:=delay_work_queue.o kthread_worker.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.symvers *.cmd *.cmd.o
endif
