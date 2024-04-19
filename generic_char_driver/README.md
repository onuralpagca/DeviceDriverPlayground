## Brief

This device driver provides a communication channel between user space application and the kernel module, facilitating message passing through printk for informational purposes.

#### User Space Application
* The user space application's primary function is to create a device node file and perform file operations through this node file.
* Creates a character device node file using *mknod* system call if it doesn't exist already.
* Accesses the device node file using open system call with read-write permissions (O_RDWR).

#### Device Node File
* The device node file acts as the interface between the user space application and the kernel module, enabling communication and data transfer between the two components.

#### Kernel Module
* The kernel module serves as the driver. It handles interactions between the user space application and the kernel.
* Registers a character device with the kernel using *register_chrdev_region*.
* Initializes and adds the character device to the kernel using *cdev_init* and *cdev_add*.
* Implements file operations such as *open* and *release*, which are invoked when the user space application accesses the device node file.
* Upon module exit, removes the character device from the kernel and unregisters the char device.


#### Initialization and Cleanup:
* Upon initialization, registers the character device and adds it to the kernel.
* Upon exit, removes the character device and unregisters the char device.

## Build Kernel Module
```bash
$ make file=generic_char_driver
$ sudo insmod generic_char_driver.ko
$ lsmod
```

## Build User Space App
```bash
$ gcc -Wall -o user_space user_space.c
$ sudo ./user_space
```

## See the logs
```bash
$ sudo dmesg
```