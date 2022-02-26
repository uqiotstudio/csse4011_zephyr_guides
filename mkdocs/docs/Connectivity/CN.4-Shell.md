# CSSE4011: Connectivity - Shell


The following tutorial will cover the setting up a command line interface/Shell through USB with existing Zephyr Drivers. (Note that, USB Debugging/Shell can only be used with platforms that support USB, such as the NRF52840 SoC).

## **1.0 Motivation**

A shell or a command line interface (CLI) allows for a user to interact with the OS in real time. Shell commands can be used to issue unique commands to a device or change application logic. For example, a command to read run-time statistics of threads or to turn an led on or off etc...

Zephyr also allows for Shell to be used as a backend for the Zephyr Logging API. The logging API is explored in ***CN.5-Logging***.

## 1.1 Test Hardware

This implementation will explore setting up shell on the Particle Argon.

* Particle Argon
  
* 2 x mUSB Cable

* Segger J-Link EDU mini

**This implementation is valid for Zephyr RTOS Version 3.0.XX**

## 1.2. Prerequisites

Ensure that you have completed/understand the following tutorials. 

* OS.1, OS.2, OS.2.1 and BRD.1

## 1.3 Setup

Connect the Particle Argon board to the host machine and ensure that the development environment has access to the device (USB passthrough to virtual machine).

## **2.0 Zephyr Shell Implementation**

### **2.1 Boilerplate**

*The following commands assume that you have setup your files/directories following the respective tutorial(s).*

Create a new application directory for a sample shell application. 

```shell
    cd ~/csse4011/csse4011_repo/
    mkdir -p apps/shell_sample/
```

For setting up a basic boilerplate for our application, we will use the provided blinky sample in Zephyr. 

```shell
    cd ~/csse4011/zephyrproject/zephyr/samples/basic/blinky
    cp -R * ~/csse4011/csse4011_repo/apps/shell_sample/
    cd ~/csse4011/csse4011_repo/apps/shell_sample/
```

### **2.2 Enable USB Drivers [prj.conf]**

To show a more structure approach to setting configs in Zephyr, in this tutorials we will explore the addition of segmented config files to the build system. Where each file represents a different subsystem. Such an approach improves modularity for bigger projects. 


First, we will enable the USB subsystem as before, 

*These options can be found [here](https://docs.zephyrproject.org/latest/guides/dts/howtos.html#set-devicetree-overlays), for diffrent subsystems.*

```shell
vim usb.conf  #create a new .conf file for the USB subsystem
```
Append the following, then save and exit the editor.
```CONF
#----------------------------------USB SETUP----------------------------------
CONFIG_USB_DEVICE_STACK=y
CONFIG_USB_DEVICE_PRODUCT="Zephyr USB shell sample"
#-----------------------------------------------------------------------------

#--------------------------------USB_OPTIONS----------------------------------
CONFIG_USB_DEVICE_PRODUCT="Particle Argon - Zephyr"
CONFIG_USB_DEVICE_MANUFACTURER="Wilfred MK"
CONFIG_USB_DEVICE_VID=0xC553
CONFIG_USB_DEVICE_PID=0x4011
#-----------------------------------------------------------------------------
```
Next, we will enable Zephyr Shell, 

```shell
vim shell.conf #create a new .conf file for shell options
```
append the following config settings.
```CONF
#-----------------------------SHELL_CONF--------------------------------------
CONFIG_SHELL_BACKEND_SERIAL_CHECK_DTR=y
CONFIG_UART_LINE_CTRL=y
CONFIG_SHELL_BACKEND_SERIAL_INIT_PRIORITY=51
CONFIG_SHELL=y
CONFIG_SHELL_BACKENDS=y
CONFIG_SHELL_BACKEND_SERIAL=y

CONFIG_SHELL_PROMPT_UART="CSSE4011:~$"

CONFIG_SHELL_CMDS=y
CONFIG_SHELL_TAB=y
CONFIG_SHELL_TAB_AUTOCOMPLETION=y
CONFIG_SHELL_VT100_COLORS=y
CONFIG_KERNEL_SHELL=y
#-----------------------------------------------------------------------------

#-----------------------------SHELL_LOGGING-----------------------------------
CONFIG_LOG=y
CONFIG_LOG_PRINTK=y
#-----------------------------------------------------------------------------
```
Save the file and exit the editor.


### **2.3 Shell USB Overlay**
As with the console, we must inform Zephyr that the **shell-uart is to be routed through the USB subsystem using CDC_ACM**. This can be done by creating and adding an overlay file to the build-system. An '*app.overlay'* file can be created similarly to the console tutorial, and this will automatically be picked up by the [build-system](https://docs.zephyrproject.org/latest/guides/dts/howtos.html#set-devicetree-overlays). However, here we will explore manually adding it to the build system. 

```shell 
vim dtc_shell.overlay #create new file for shell overlay
```
append the following to the file.
```DTS
/ {
        chosen {
                zephyr,shell-uart = &cdc_acm_uart0;
        };
};

&zephyr_udc0 {
        cdc_acm_uart0: cdc_acm_uart0 {
                compatible = "zephyr,cdc-acm-uart";
                label = "CDC_ACM_0";
        };
};
```
Save and exit the editor.

### **2.4 Adding files to CMakeLists**

At this point, we have created our config/overlay files. We must add these to our build system for any of them to take effect (if a file is named app.conf, it is picked up by the build system by default, as seen in *CN.3-Serial_Console*). More details on this can be found [here](https://docs.zephyrproject.org/latest/application/index.html).

```shell
vim CMakeLists.txt
```
At the top of the file, append the following to add the newly created config files to CMakeLists.
```
set(CONF_FILE usb.conf shell.conf)
set(DTC_OVERLAY_FILE dtc_shell.overlay)
```

### **2.5 Initializing the Driver [main.c]**

At this point, ensure that there are no compile errors by running
```shell
west build -p -b particle_argon
```

and now we tell our application to initialize the USB stack.

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
    const struct device *shell_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
    uint32_t dtr = 0;

    /* Enable the USB Driver */
    if (usb_enable(NULL))   
        return;

   /* Wait on DTR - 'Data Terminal Ready'
    * Will wait here until a terminal has been attached to the device
    * This is not necessary, however, can be useful for printing boot info etc..
    */
    while (!dtr) {
        uart_line_ctrl_get(shell_dev, UART_LINE_CTRL_DTR, &dtr);
        k_sleep(K_MSEC(100));
    }

    while(1)
    {
        //printk("Hello World\n");  //This will get routed to the shell also (optional)
        k_sleep(K_MSEC(500));
    }
}
```
### **2.6 Testing**

This application can now be built and flashed using:
```shell
west  build -p auto -b particle_argon
west flash -r jlink   
```
Since this is now a **'new usb device'**, **you will need to pass through USB from your host machine to the VM to open the shell within your CSSE4011 VM** Typically, you might have to unplug and reconnect the device for the pass through to take effect. Alternatively, you should be able to view the shell on the host machine also.

See that the device is connected 
```shell
lsusb  #Command to show currently attached USB devices

#You will see this if you added the USB OPTIONS from above
Bus 001 Device 099: ID c553:4011 Wilfred MK Particle Argon - Zephyr
```

First install screen (App that can monitor terminal)
```shell
sudo apt-get install screen
```
```shell
sudo screen /dev/ttyACM0
```
You can press 'tab' to see the currently enabled options through shell.
```
[00:00:11.851,226] <inf> usb_cdc_acm: Device resumed
[00:00:11.851,257] <inf> usb_cdc_acm: from suspend
CSSE4011:~$
  clear              device             devmem             help
  history            hwinfo             kernel             log
  nrf_clock_control  resize             shell
```

```
CSSE4011:~$kernel threads
Scheduler: 23890 since last call
Threads:
*0x20000438 shell_uart
        options: 0x0, priority: 14 timeout: 536872112
        state: queued, entry: 0x13029
        stack size 2048, unused 1176, usage 872 / 2048 (42 %)

```

## **2.7 Sample Application**

A sample application has been provided, this application includes all the steps mentioned above. You can test the shell by flashing it to the Particle Argon.

Sample is located in:

* REPO_TOP/tute_solutions/shell_example/
