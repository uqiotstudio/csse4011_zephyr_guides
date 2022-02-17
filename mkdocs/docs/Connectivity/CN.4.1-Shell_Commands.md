# CSSE4011: Connectivity - Shell Commands

This tutorial aims to introduce adding shell commands to the Zephyr shell that we explored in  `CN.4-Shell`.

## **1.0 Motivation**

To get the most out of an application that implement a shell, it is often useful to add specific functionality. By adding application specific shell commands, specific features of an application can be exposed to a user/developer. 

Typically, a command will `hook` into a `piece of code` or a `callback function` and execute some functionality for that command. For instance, in a embedded application, if the user aims to turn some hardware off, you could issue a shell command to the device to carry this out, or if you wanted to get a reading from a sensor and print it to the shell, you could issue a command to do this. Similar to how in a unix based shell you would issue commands to interact/request services from the OS.

## 1.1 Test Hardware

> Arduino Sense Board

> mUSB Cable

**This implementation is valid for Zephyr RTOS Version 2.7.XX**

## 1.2. Prerequisites

Ensure that you have completed/understand the following tutorials. 
> OS.1, OS.2, OS.2.1, BRD.1 and **CN.4-Shell**

## 1.3 Setup

Connect the Arduino sense board to the host machine and ensure that the development environment has access to the device (USB passthrough to virtual machine). 

## **2.0 Zephyr Shell Command Implementation**

### **2.1 Boilerplate**

In the previous tutorial `(CN.4 - Shell)` we set up the Zephyr Shell through USB. We will use that as our boilerplate for this tutorial and add to it the functionality required to implement shell commands.

*The following commands assume that you have setup your files/directories following the respective tutorial(s).*


```shell
cd ~/csse4011/csse4011_repo/apps
mkdir shell_cmd_sample/
```
Copy shell sample files (from `CN.4 - Shell`) into new directory for shell commands
```Shell
cd -R shell_sample/* shell_cmd_sample/
```

### **2.1 Implementing a Shell Command**

In this tutorial, we will explore using shell commands, to toggle on and off the onboard led. This should give you a general idea of how commands should be implemented/used. For more information on the following see [here](https://docs.zephyrproject.org/2.7.0/reference/shell/index.html).

Zephyr allows you to create commands that have sub-commands, these can be useful for instance, if you wanted a top level command (`root command - level 0`) for a particular sub-system/hardware, and then sub-commands (`static/dynamic sub-commands - level > 0`) that perform unique a function for that system. It can be thought of as a tree of commands.

Here, we will look at creating a **root command** (led) with **two static commands** (on/off). However, you can refer here [1] for the api guide for more information on other types of commands.

First, 

```shell
cd shell_cmd_sample/
vim src/main.c
```
add the shell include
```
#include <shell/shell.h>
```

Now we can specify what commands we are going to make, using the Shell API macros `SHELL_CMD_REGISTER()` and `SHELL_STATIC_SUBCMD_SET_CREATE()`

Let's first declare some prototype handlers for our commands
```C
 /* Declare command handler prototypes */
 static int cmd_led_ctrl_on(const struct shell *, size_t, char **);
 static int cmd_led_ctrl_off(const struct shell *, size_t, char **);
```
and let's use the macros to setup our commands.
```C
/* Specify Shell Commands for LED Toggling */
/* Creating subcommands (level 1 command) array for command "led". */ 
SHELL_STATIC_SUBCMD_SET_CREATE(led_ctrl,
        SHELL_CMD(on, NULL, "Turn led on.", cmd_led_ctrl_on),
        SHELL_CMD(off,   NULL, "Turn led off.", cmd_led_ctrl_off),
        SHELL_SUBCMD_SET_END
); 

/* Creating root (level 0) command "led" */
SHELL_CMD_REGISTER(led, &led_ctrl, "Led Control (On/Off)", NULL);
```
`SHELL_CMD_REGISTER()` creates out root level command for `led`, and `SHELL_STATIC_SUBCMD_SET_CREATE()`
create the level 1 subcommands for `on` and `off`, which are linked to their respective command handlers `cmd_led_ctrl_on` and `cmd_led_ctrl_off`. That is, when a user types `led off`, the `cmd_led_ctrl_off` is triggered.


Finally, we can implement the actual handlers. Note, that these don't check if the `led-gpio pin` has been initialized and assumes it has been pre-configured in `main()` (if not, `gpio_pin_set()` will fail). 

```C
/* Command Handler for toggling led0 on, note that it assumes
 * the pin has been preconfigured */
static int cmd_led_ctrl_on(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
        ARG_UNUSED(argv);

        const struct device *dev = device_get_binding(LED0);

        if (dev == NULL) {
                return ENODEV;
        }
        /* Pin default is Active low (as per FLAGS) */
        return gpio_pin_set(dev, PIN, (int)0);
}

/* Command Handler for toggling led0 off, note that it assumes
 * the pin has been preconfigured */
static int cmd_led_ctrl_off(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
        ARG_UNUSED(argv);

        const struct device *dev = device_get_binding(LED0);

        if (dev == NULL) {
                return ENODEV;
        }
        /* Pin default is active low */
        return gpio_pin_set(dev, PIN, (int)1);
}
```
Since we copied the boilerplate from `CN-4 Shell`, make sure to cleanup the `while()` loop in `main()` that toggles the gpio, but leave the code that initializes the `led-gpio pin`. Alternatively, you could make two other subcommands to init/deinit the pin.

## **2.2 Testing**

Once you have built and flashed the application, attach `screen` to the device. In the shell, you can press `tab` to show available commands. You should be able to see one that says `led` now. Try `led --help` to see what it can do, then, `led on`, this should turn the led on.

```
[00:00:00.234,588] <inf> usb_cdc_acm: Device resumed
[00:00:00.429,962] <inf> usb_cdc_acm: Device configured
CSSE4011:~$
  clear              device             devmem             help
  history            hwinfo             kernel             led
  log                nrf_clock_control  resize             shell
CSSE4011:~$led --help
led - Led Control (On/Off)
Subcommands:
  on   :Turn led on.
  off  :Turn led off.
CSSE4011:~$
CSSE4011:~$led on
CSSE4011:~$led off
```

## **3.0 Sample Application**

A sample application has been provided, this application includes all the steps mentioned above. You can test the shell commands to toggle the onboard led by flashing it to the Arduino Sense Board.

Sample is located in **REPO_TOP/tute_solutions/shell_cmd_sample/**
