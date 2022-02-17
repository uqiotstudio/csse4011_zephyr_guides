# Connectivity Documentation

## Summary

The following set of tutorial aim to expose aspects of Zephyr RTOS and handle connectivity. Such as basic GPIO to more advance features like using a console, shell and a logging system.

## Topic Brief
* CN.1 - GPIO
    * Exposing GPIO pins and toggling pins
    * Using devicetree/macros to interface hardware to application(s)

* CN.3 - Serial Console
    * Setting up a console through USB
    * Using printk() and printing to console
    * Specify hardware usage using device tree overlays

* CN.4 - Shell
    * Setting up the Zephyr Shell through USB
    * Use of kernel config files
    * Specify hardware usage using device tree overlays

* CN.4 - Shell Commands
    * Building on CN.4 to add shell commands
    * Commands that can execute a particular subroutine
    * Exposing the Zephyr Shell Command API

* CN.5 - Logging
    * Expose Zephyr Logging sub-system
    * Setup Shell as Logging backend
    * Setup log modules and log filtering

* CN.6 - Using SEGGER RTT
    * Using a Shell through RTT
    * Using a Console through RTT
