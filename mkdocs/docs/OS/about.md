# Zephyr RTOS - Guides

## Summary

The following set of tutorials are intended to help students get setup with Zephyr RTOS on their newly setup virtual machines (Debian based), and to get a basic application to compile. 

These tutorials are intended to be **attempted in order**, particularly the first few.

## Topic Brief

* OS.1 - Getting Started 
    * Cloning and setting up Zephyr RTOS (installing toolchains and other required packages), on Debian/Ubuntu.
    * Building and flashing sample blinky app
* OS.2 - First Program
    * Setting up and building a Zephyr application (outside Zephyr source directory)
    * Setting up app working directories
* OS.2.1 - Building Tips
    * Basics of the build system
    * Tips to customize build system
    * Including config files
* OS.2.2 - Implementing Libraries
    * Library directory structures
    * Adding library to app build system
* OS.4 - Threading
    * Introduction to threads
    * Thread implementation in Zephyr RTOS
* OS.5.1 - Thread Synchronization
    * Introduction to thread synchronization
    * Semphore, Mutex and Condition Variables implementation
* OS.5.2 - Thread Communication (ITC)
    * Introduction to inter-thread-communication (ITC)
    * Basic communication primitives
    * Implementation of Zephyr message queues.

## Notes:
* The following tutorials were created with **reference to Zephyr RTOS V2.7.XX**. 

* Through these guide, the reference to the environment variable **`(REPO_TOP)`** refers to the top level directory of within tutedocs. 