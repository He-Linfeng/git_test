obj-m:=sample.o

PWD:=$(shell pwd) 
LINUX_SRC=/home/hlf/build/kernel
  
modules:
	make -C $(LINUX_SRC) M=$(PWD) modules
 
clean:
	make -C $(LINUX_SRC) M=$(PWD) clean
