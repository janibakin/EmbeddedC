obj-m += stackdev.o
USER_TEST = test_stack_device

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc $(USER_TEST).c -o $(USER_TEST)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f $(USER_TEST)