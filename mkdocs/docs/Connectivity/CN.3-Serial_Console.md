# CSSE4011: Connectivity - Serial Console

The following tutorial will cover the setting up a console through USB with existing Zephyr Drivers. (Note that, USB Debugging/Shell can only be used with platforms that support USB, such as the NRF52840 SoC).


## **1.0 Motivation**

Once USB console is setup, it can be used for printk() debugging, and/or to examine the internal data structures of an application where required.                     

## 1.1 Test Hardware

This implementation will explore setting up the console on the Arduino Sense board.

* Arduino Sense Board

* mUSB Cable

**This implementation is valid for Zephyr RTOS Version 2.7.XX**

## 1.2. Prerequisites

Ensure that you have completed/understand the following tutorials. 

* OS.1, OS.2, OS.2.1 and BRD.1

## **2.0 Setting up Console**

### **2.1 Boilerplate**

*The following commands assume that you have setup your files/directories following the respective tutorial(s).*

Create a new application directory for a sample console application. 

```shell
    cd ~/csse4011/csse4011_repo/
    mkdir -p apps/console_sample/
```

For setting up a basic boilerplate for our application, we will use the provided blinky sample in Zephyr. 

```shell
    cd ~/csse4011/zephyrproject/zephyr/samples/basic/blinky
    cp -R * ~/csse4011/csse4011_repo/apps/console_sample/
    cd ~/csse4011/csse4011_repo/apps/console_sample/
```

### **2.2 Enable USB Drivers [prj.conf]**

To enable the existing USB drivers, a few config options need to be added to our project. A detailed guide for the USB API is found [here](https://docs.zephyrproject.org/latest/reference/usb/uds_cdc_acm.html ), which you may need to refer to for additional information. 


Start by editing the **prj.conf** file. Append the following config options. These options can be found [here](https://docs.zephyrproject.org/latest/reference/devicetree/api.html ), for different subsystems. 
```CONF
CONFIG_GPIO=y

#------------------------------ENABLE USB---------------------------------------
CONFIG_BOARD_ARDUINO_NANO_33_BLE_EN_USB_CONSOLE=y
CONFIG_USB_DEVICE_STACK=y
CONFIG_USB_DEVICE_PRODUCT="USB Console Tute"

CONFIG_SERIAL=y
CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y
CONFIG_UART_LINE_CTRL=y
#------------------------------------------------------------------------------
```
Optionally, you can add the following config statements to add some flavour to the USB Stack... These commands let you set USB device ID parameters. 

```CONF
#--------------------------------USB OPTIONS----------------------------------
CONFIG_USB_DEVICE_PRODUCT="Arduino Nano BLE - Zephyr"
CONFIG_USB_DEVICE_MANUFACTURER="Wilfred MK"
CONFIG_USB_DEVICE_VID=0xC553
CONFIG_USB_DEVICE_PID=0x4011
#-----------------------------------------------------------------------------
```
### **2.3 Setting up CDC-ACM [app.overlay]**

Zephyr v2.7 require an overlay file to be added to specify Communication Device Class - Abstract Control Model (CDC-ACM). The CDC ACM can be used as backends for Zephyr Subsystems, such as console and shell. 

There is detailed information about how an overlay is implemented and it's purpose in this sub-system which can be found [here](https://docs.zephyrproject.org/latest/reference/usb/uds_cdc_acm.html). 

For this particular example, start by making an **app.overlay**. If there exists a file named **app.overlay** in the app directory, it get added to the build system by default (See [here](https://docs.zephyrproject.org/latest/guides/dts/howtos.html), for device tree overlays). 

```shell
cd ~/csse4011/csse4011_repo/apps/console_sample/        #App Directory
vim app.overlay                                         #Use any text editor
```

Copy the following overlay details into the newly created file. This is adding a devicetree overlay to the build system, see [here](https://docs.zephyrproject.org/latest/reference/devicetree/api.html ) for more. Here we specify the console should be routed to cdc_acm_uart0.

```DTS
/ {
        chosen {
                zephyr,console = &cdc_acm_uart0;
        };
};

&zephyr_udc0 {
        cdc_acm_uart0: cdc_acm_uart0 {
                compatible = "zephyr,cdc-acm-uart";:q
                label = "CDC_ACM_0";
        };
};
```
### **2.4 Initializing the Driver [main.c]**

At this point, ensure that there are no compile errors by running
```shell
west build -p -b arduino_nano_33_ble
```

and now we tell our application to initialize the USB and print some data.

```shell
vim src/main.c
```
```C
#include <usb/usb_device.h>
#include <drivers/uart.h>       //Include these libraries

//Add the following to main()
void
main(void)
{
    
    /* Setup DTR */
    const struct device *console_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    uint32_t dtr = 0;

    /* Enable the USB Driver */
    if (usb_enable(NULL))   
        return;

   /* Wait on DTR - 'Data Terminal Ready'
    * Will wait here until a terminal has been attached to the device
    * This is not necessary, however, can be useful for printing boot info etc..
    */
    while (!dtr) {
        uart_line_ctrl_get(console_dev, UART_LINE_CTRL_DTR, &dtr);
        k_sleep(K_MSEC(100));
    }

    while(1)
    {
        printk("Hello World\n");
        k_sleep(K_MSEC(500));
    }
}
```

### **2.5 Testing**

This application can now be built and flashed using:
```shell
west  build -p auto -b arduino_nano_33_ble
west flash --bossac=$HOME/csse4011/BOSSA/BOSSA/bin/bossac   #Check the Path is correct
```

Since this is now a **'new usb device'**, **you will need to pass through USB from your host machine to the VM to open the console within your CSSE4011 VM** Typically, you might have to unplug and reconnect the device for the pass through to take effect. Alternatively, you should be able to view the console on the host machine also.

See that the device is connected 
```shell
lsusb  #Command to show currently attached USB devices

Bus 001 Device 099: ID c553:4011 Wilfred MK Arduino Nano BLE - Zephyr #You will see this if you added the USB OPTIONS from above
```

First install screen (App that can monitor terminal)
```shell
sudo apt-get install screen
```
```shell
sudo screen /dev/ttyACM0

Hello World
Hello World
Hello World
```

## **2.6 Sample Application**

A sample application has been provided, this application includes all the steps mentioned above. You can test the console by flashing it to the Arduino Sense Board.

Sample is located in **REPO_TOP/tute_solutions/console_example/**
