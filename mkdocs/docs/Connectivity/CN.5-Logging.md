# CSSE4011: Connectivity - Loggging

## **1.0 Motivation**

The aim of this tutorial is to introduce the Zephyr Logging Api. Logging in an application can be used for for implementing debugging, providing status updates and other general information. 

In complex systems, logging is often implemented and is a fundamental part of providing a robust application. Using the Zephyr logging api, this tutorial will introduce how to implement logging in an application. 

Typically, logging is coupled with a command line interface. In addition to the previous tutorial *CN.4-Shell*, we will append the logging subsystem on to our shell.

## 1.1 Test Hardware

> Arduino Sense Board

> mUSB Cable

**This implementation is valid for Zephyr RTOS Version 2.7.XX**

## 1.2. Prerequisites

Ensure that you have completed/understand the following tutorials. 
> OS.1, OS.2, OS.2.1 , BRD.1 and CN.4-Shell. 

## **2.0 Zephyr Logging Implementation**

### **2.1 Boilerplate**

*The following commands assume that you have setup your files/directories following the respective tutorial(s).*

Create a new application directory for a sample shell application. 

```shell
cd ~/csse4011/csse4011_repo/
mkdir -p apps/logging_sample/
```

Here we will a copy of the previous shell sample and add to it. You may also use the provided shell solution.

```shell
cd shell_example/
cp -R * ../shell_sample/
cd ~/csse4011/csse4011_repo/apps/shell_sample/
```

### **2.2 Driver and Config Setup**

Since we are appending to our existing solution of the shell sample, we do need additional overlays or config options here. It is worth noting that in `shell.conf` the following lines enable logging. Here, **the shell is set default as the backend for logging** (preset in Kconfig options: `CONFIG_SHELL_LOG_BACKEND` [1]).

```CONF
#-----------------------------SHELL_LOGGING-----------------------------------
CONFIG_LOG=y
CONFIG_LOG_PRINTK=y
#-----------------------------------------------------------------------------
```

This also shows the simplicity of Zephyr, once an application is setup, using Zephyr features is as simple as adding config options (for the most part anyway...)!

## **3.0 Using the Logging API**

The API guide for logging can be found [here](https://docs.zephyrproject.org/latest/reference/logging/index.html), it is suggested that you read through it and understand the implementation options that logging can provide.


### **3.1 Register a Module**

A `log module` must be declared prior to using logging in an application. These modules may represent a device driver or a particular sub-system. For instance, you may define a module for a particular temperature sensor driver -- any logged output from this driver will now contain a signature referring to the name of the respective module. 

Since we only have the one source file, we will define only one module. The [API guide](https://docs.zephyrproject.org/latest/reference/logging/index.html) has more information on how to use this across multiple files using different macros. It is simple to setup and highly suggested for your projects. 

Start by editing our source file. 
```shell
vim src/main.c
```
append the following.
```C
#include <logging/log.h>

/* Define logging module */
LOG_MODULE_REGISTER(sample_module);

//OR

LOG_MODULE_REGISTER(sample_module, LOG_LEVEL_DBG);
```

This registers a module names `sample_module` with a log filter level of `DEBUG` (this is optional). 

### **3.2 Using a Module**

Once a module as been registered as above, we can use the following macros to implement the logging. Keep in mind that if you have set a `LOG_LEVEL_X` some of these may be filtered out.

```C
LOG_ERR("Some error message: %d", errno);

LOG_WRN("It's getting hot in here...");

LOG_INF("The table is made out of...table");

LOG_DBG("Read 0x8F register value: %ld", rVal);
```

Refer to the [API guide](https://docs.zephyrproject.org/latest/reference/logging/index.html), for more useful logging macros. 

In our `main.c` file, we can add in some logging options. 

```C
...
...
while (1) {
        gpio_pin_set(dev, PIN, (int)led_is_on);
        led_is_on = !led_is_on;
        LOG_INF("Led is currently: %s", led_is_on ? "On" : "Off");
        k_sleep(K_SECONDS(5));
}

```

## **4.0 Testing**

This application can now be built and flashed using:
```shell
west  build -p auto -b arduino_nano_33_ble
west flash --bossac=$HOME/csse4011/BOSSA/BOSSA/bin/bossac   #Check the Path is correct
```

Once flashed,
```shell
sudo screen /dev/ttyACM0
```

```
...
...
[00:00:03.203,369] <dbg> sample_module.main: DTR OK
[00:00:03.203,369] <inf> sample_module: Led is currently: Off
[00:00:08.203,430] <inf> sample_module: Led is currently: On
```

## **4.1 Sample Application**

A sample application has been provided, this application includes all the steps mentioned above. You can test the shell by flashing it to the Arduino Sense Board.

Sample is located in **REPO_TOP/tute_solutions/logging_example/**
