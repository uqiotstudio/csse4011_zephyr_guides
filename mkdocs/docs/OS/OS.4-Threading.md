# CSSE4011: Tute 4 - Threading in Zephyr    

## **1.0 Motivation**

In this tutorial, we focus on learning the use of threads in Zephyr RTOS. The use of threads is to implement an application that can 'multi-task'.

## **1.1 Summary of threads**

Threads are typically created with an appropriate level of priority based on their task within the application, and also an appropriate stack size. When a thread is interrupted and switched out for another thread (code context changed), this is know as a context switch. Typically, based on thread synchronization implementation/priorities, the kernel will perform a context switch between threads, allowing for other threads to perform their duties. On a micro-controller unit (MCU), this mimics 'multi-tasking' within a program.


Example, in an IoT based application, where an application may need to maintain a network, communicate and collect data from hardware sensors, maintain a device power profile. Threads allow an application to maintain multiple 'threads' of execution with different contexts.


## **2.0 Threads in Zephyr**

In Zephyr [1], a thread is a kernel object that is used for application processing that is too lengthy or too complex to be performed by an interrupt service routine (ISR). Any number of threads can be defined by an application (limited only by available RAM). Each thread is referenced by a thread id that is assigned when the thread is spawned. Additionally, each thread is allocated a specific priority and a stack size at creation (see [here](https://docs.zephyrproject.org/latest/reference/kernel/threads/index.html) for more details).

## **2.1 Using threads in Zephyr RTOS**

There are two ways in which a threads can be created in Zephyr, compile-time and at run-time. An application that has fixed functionality, can be solely implemented with compile-time threads. When compile-time thread is defined, it starts immediately as the program begins. Note, that there is not inherent diffrence between compile-time and run-time threads, other than the way in which they are created.

If an application requires a thread to be created at run-time, a run-time thread can be created, this process is similar to how threads are created in freeRTOS. 

## **2.2 Thread Creation in Zephyr RTOS**

NOTE: It is strongly recommended that you read the [Zephyr thread API](https://docs.zephyrproject.org/latest/reference/kernel/threads/index.html), as it shows you the intricate details and correct usage of the API for additional features.

### **2.2.1 Compile-Time Threads**

A compile-time thread in Zephyr is created by the following macro.
```
K_THREAD_DEFINE(name, stack_size, entry, p1, p2, p3, prio, options, delay)
```

The below snippet shows how a compile-time thread can be setup. Refer to [Zephyr Thread API](https://docs.zephyrproject.org/latest/reference/kernel/threads/index.html) for more details.

```
#define MY_STACK_SIZE 500
#define MY_PRIORITY 5

void my_entry_point(void *, void *, void *);

K_THREAD_DEFINE(my_tid, MY_STACK_SIZE,
                my_entry_point, NULL, NULL, NULL,
                MY_PRIORITY, 0, 0);


/* A thread, that does some work */
void
my_entry_point(void *a, void *b, void *c)
{
    while(1) {
        /*
         * do some work
         */
        k_msleep(1000);
    }
}
```

### **2.2.2 Run-Time Threads**

Run-time threads in Zephyr can be created as seen below. This code performs the same fundamental actions as compile-time thread. However, it is created at run-time. 

```
#define MY_STACK_SIZE 500
#define MY_PRIORITY 5

void my_entry_point(void *, void *, void *);

K_THREAD_STACK_DEFINE(my_stack_area, MY_STACK_SIZE);
struct k_thread my_thread_data;

/* Application entry point */
int
main(void) {

    /* Spawn new thread at run-time */
    k_tid_t my_tid = k_thread_create(&my_thread_data, my_stack_area,
                                 K_THREAD_STACK_SIZEOF(my_stack_area),
                                 my_entry_point,
                                 NULL, NULL, NULL,
                                 MY_PRIORITY, 0, K_NO_WAIT);

    return 0;
}


/* A thread, that does some work */
void
my_entry_point(void *a, void *b, void *c)
{
    while(1) {
        /*
         * do some work
         */
        k_msleep(1000);
    }
}
```

### **2.2.3 Thread Priorities**

In Zephyr RTOS, a thread’s priority is an integer value, and can be either negative or non-negative. Numerically lower priorities takes precedence over numerically higher values. For example, the scheduler gives thread A of priority 4 higher priority over thread B of priority 7; likewise thread C of priority -2 has higher priority than both thread A and thread B [see here](https://docs.zephyrproject.org/latest/reference/kernel/threads/index.html).

Priorities for threads should be chosen carefully based on the application. You may notice issues with thread starvation (not getting enough time to run), if priorities are chosen inappropriately. A threads priority can also be changed after it has been created. 


### **2.2.4 Thread Scheduling**

The kernel’s priority-based scheduler allows an application’s thread to share the CPU. There are two different 'types' of threads in Zephyr with respect to the scheduler. These are 'Pre-emptive' and 'Cooperative' threads. In summary, once a cooperative thread becomes the current thread, it remains the current thread until it performs an action that makes it unready. Whereas, a preemptive thread becomes the current thread, it remains the current thread until a higher priority thread becomes ready, or until the thread performs an action that makes it unready [see here](https://docs.zephyrproject.org/latest/reference/kernel/scheduling/index.html).




>A cooperative thread has a negative priority value. 

>A preemptible thread has a non-negative priority value. 



In application [as per](https://docs.zephyrproject.org/latest/reference/kernel/scheduling/index.html), 

>Use cooperative threads for device drivers and other performance-critical work.

>Use cooperative threads to implement mutually exclusion without the need for a kernel object, such as a mutex.

>Use preemptive threads to give priority to time-sensitive processing over less time-sensitive processing.


## **3.0 Tutorial Question:**

Copy the zephyr/sample/blinky program to a working application directory, then, implement two threads, where one thread will turn on the led, and the other thread will turn off the led. Use either the thingy52 or the Arduino Sense to test your code.

>Question: What is wrong with an implementation like this? Why is this not ideal, what type of issues might you run into?


## **3.1 Sample Solution**

A sample solution is uploaded in the docs repository. Find located within,

> tute_solutions/OS4_tute/src/

This code can be built with:

> west build -p -b <board_name>

and flashed with

> west flash -r 'runner'

Refer to the board flashing tutorials for additional build/flash guides.