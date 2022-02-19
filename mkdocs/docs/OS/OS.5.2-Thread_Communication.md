# CSSE4011: Tute 5.2 - Thread Communication 

## **1.0 Motivation**

The following tutorial aims to introduce inter-thread communication (ITC) and useful communication primitives within Zephyr RTOS. 

### **1.1 Inter-Thread Communication (ITC)**

Similar to inter-process-communications (IPC) in typical operating systems, ITC allows one thead to share data between another thread(s). Where mutexs and semaphores are typically used for mutual exclusion and signalling, data passing/ITC allows for threads to communicate beyond primitive signaling. 


### **1.2. Prerequisites**

Ensure that you have completed/understand the following tutorials. 

* OS.1, OS.2, OS.2.1, BRD.1 and CN.3-Serial_Console or CN.5-Shell (for printk() usage)

## **2.0 Thread Communication in Zephyr**

### **2.1 Data Passing in Zephyr**

Zephyr RTOS (as of V2.7.XX) offers a few different options for ITC. To name a few,
* First in first out buffers (FIFO)
* Last in first out buffer (LIFO)
* Stacks
*  Message queues, Mailboxes and Pipes

Some of these are ideal for particular use cases, to determine which ITC protocol might best fit an application, refer to the *Data Passing* [guidelines table](https://docs.zephyrproject.org/latest/reference/kernel/index.html#data-passing). More information on the implementation of each primitive can be found in the api guides (links below the [table](https://docs.zephyrproject.org/latest/reference/kernel/index.html#data-passing)).

### **2.2 General Message Queues** 

In this tutorial, we will focus on implementing a message queues between threads. As they can be typically be used for a wide range of applications, it is a good starting point for learning ITC in Zephyr. The api guide can be found [here](https://docs.zephyrproject.org/latest/reference/kernel/data_passing/message_queues.html).

What is a message queue?
  
* "*A message queue is a kernel object that implements a simple message queue, allowing threads and ISRs to **asynchronously** send and receive fixed-size data items*."


## **2.3 Implementing Message Queues**

Start by making a basic Zephyr application, you may use a copy of the sample solution provided in CN.4-Shell (*shell_example*) and use this as a boilerplate. The provided solution has USB-Shell and USB-Printk enabled. 

Start by editing the source file.
```shell
vim src/main.c
```
The following macro allows for the definition of a message queue in Zephyr, add this to your source file. 
```C
K_MSGQ_DEFINE(my_msgq, sizeof(struct data_packet), 10, 4);
```
Where, a message queue named *my_msgq* is initialized, that queues *10* struct *data_packet* items, where the structure is aligned to an address that is divisible by 4 (4-byte aligned). 

To make this work, we must also define a struct *data_packet* globally (before macro usage).
```C
struct data_packet {
        uint16_t preamble;
        char string[32];
};
```
In our main(), we can add some data to our struct
```C
/* MSGQ Packet */
struct data_packet packet;
packet.preamble = 0xAA;
snprintk(packet.string, sizeof(packet.string), "Colonels Recipe.exe\n");
```
and lets put this packet on our queue, (add this to the while loop in main())
```C
/* Send Messages to consumers */
if (k_msgq_put(&my_msgq, &packet, K_NO_WAIT) != 0) {
        /* Queue is full, we could purge it, a loop can be
        * implemented here to keep trying after a purge.
        */
        k_msgq_purge(&my_msgq);
     }
```
Now, we will add another thread (alongside main), so we can test the ITC between two threads using the message queue. Note: Make sure to give this thread sufficient stack size as **message queues are passed by copy**. 

```C
/* Define Consumer Thread */
void consumer_thread(void);
#define STACK_SIZE 1024
#define THREAD_PRIOR 3
#define THREAD_DELAY 0
#define OPTIONS 0
K_THREAD_DEFINE(consumer_thread_tid, STACK_SIZE, consumer_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, THREAD_DELAY);

/*
 * Consumer thread for testing message queue
 */
void
consumer_thread(void)
{
        struct data_packet data;

        while(1) {
                if (k_msgq_get(&my_msgq, &data, K_FOREVER) == 0) {
                        if (data.preamble == 0xAA)
                                printk("MSG Received: %s\n", data.string);
                        memset(&data, 0, sizeof(struct data_packet));
                }
        }
}
```

### **2.4 Testing**

This application can now be built and flashed using:
```shell
west  build -p auto -b arduino_nano_33_ble
west flash --bossac=$HOME/csse4011/BOSSA/BOSSA/bin/bossac   #Check the Path is correct
```
As per previous guides on Shell and Console implementation, attach a session to screen. 
```shell
sudo screen /dev/ttyACM0
```
```
[00:00:00.005,340] <inf> usb_cdc_acm: Device suspended
[00:00:00.165,863] <inf> usb_cdc_acm: Device resumed
[00:00:00.432,922] <inf> usb_cdc_acm: Device configured
MSG Received: Colonels Recipe.exe

MSG Received: Colonels Recipe.exe

MSG Received: Colonels Recipe.exe

MSG Received: Colonels Recipe.exe

MSG Received: Colonels Recipe.exe

CSSE4011:~$
```
### **2.5 Additional Notes**

This tutorial is a guide for implementing basic message queues in Zephyr RTOS, it is **strongly suggested** that you refer to the Zephyr API guides [here](https://docs.zephyrproject.org/latest/reference/kernel/index.html#data-passing) and [here](https://docs.zephyrproject.org/latest/reference/kernel/data_passing/message_queues.html) for additional information.

## **3.0 Sample Solution**

A sample solution is provided under, 

* REPO_TOP/tute_soltions/OS-5.2_tute/

this solution implements the above functionality into a Zephyr application. Feel free to use it and modify it.

