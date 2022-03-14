# CSSE4011: Connectivity - Using Segger RTT

## **1.0 Motivation**

Aim of this tutorial is to use Segger RTT for serial access on devices where USB is not available. For instance, the Thingy52, has an nRF52832 SOC, which does not support USB-D. To debug such a device, where a JTAG is exposed, we can configure the Zephyr kernel to backend a shell or a console to Segger RTT. 

## **1.1 Test Hardware**

* Thingy52 or Particle Argon 

* 2x mUSB Cable

* Segger JLinkEDUMini


## **1.2. Prerequisites**

Ensure that you have completed/understand the following tutorials. 

* OS.1, OS.2, OS.2.1 and BRD.2

## **1.3 Setup**

Connect the the JLinkEdu mini to the Thingy52 and to your VM (with USB passthrough). Make sure that the Thingy52 is powered on (switch in left position, when looking directly at it) and connected to a PC for charging the battery.


## **2.0 Zephyr Console - Segger RTT  Implementation**

*The following commands assume that you have setup your files/directories following the respective tutorial(s).*

### **2.1 Boilerplate**

Start by making setting up a sample Zephyr application as we have done before. You could use the `blinkly` provided sample in the Zephyr source.

```shell
cd ~/csse4011/csse4011_repo/apps
mkdir segger_rtt_console/
```
Use the boilerplate from blinky to setup the application directory

```shell
cd ~/csse4011/zephyrproject/zephyr/samples/basic/blinky
cp -R * ~/csse4011/csse4011_repo/apps/segger_rtt_console/
cd ~/csse4011/csse4011_repo/apps/segger_rtt_console/
```

### **2.2 Add Segger RTT KConf for Console**
We'll create a new config file.
```shell
vim segger_rtt_console.conf
```
and add in:
```CONF
#-----------------------------RTT_CONFIG--------------------------------------
CONFIG_USE_SEGGER_RTT=y

CONFIG_RTT_CONSOLE=y
CONFIG_UART_CONSOLE=n
#-----------------------------------------------------------------------------
```

### **2.3 Add KConf to CMakeLists**
Now, we will add these to the build system (as per `OS2.1 - Building Tips`)
```shell 
vim CMakeLists.txt

set(CONF_FILE segger_rtt_console.conf)
```
Now you can compile and flash this application. See below **`4.0 Testing`** for more. Be sure to add a `printk()`, so you can see some output in the console.

```C
while(1) {
    printk("Out like a light\n");
    k_msleep(500);
}
```

## **3.0 Zephyr Shell - Segger RTT  Implementation**

*The following commands assume that you have setup your files/directories following the respective tutorial(s).*

### **3.1 Boilerplate**

Start by making setting up a sample Zephyr application as we have done before. You could use the `blinkly` provided sample in the Zephyr source.

```shell
cd ~/csse4011/csse4011_repo/apps
mkdir segger_rtt_shell/
```
Use the boilerplate from blinky to setup the application directory

```shell
cd ~/csse4011/zephyrproject/zephyr/samples/basic/blinky
cp -R * ~/csse4011/csse4011_repo/apps/segger_rtt_shell/
cd ~/csse4011/csse4011_repo/apps/segger_rtt_shell/
```

### **3.2 Add Segger RTT KConf for Shell**

We'll create a new config file.
```shell
vim segger_rtt_shell.conf
```
And add these options into tell the kernel we want to use these features.
```CONF
#-----------------------------RTT_CONFIG--------------------------------------
CONFIG_USE_SEGGER_RTT=y
CONFIG_SHELL_BACKEND_RTT=y
#-----------------------------------------------------------------------------
```
Since we are using the shell, we will also need to configure it (same config as in `CN.4 - Shell`). To summarize:
```shell
vim shell.conf
```
and add the following
```CONF
#-----------------------------SHELL_CONF--------------------------------------
CONFIG_SHELL_BACKEND_SERIAL_CHECK_DTR=y
CONFIG_UART_LINE_CTRL=y
CONFIG_SHELL_BACKEND_SERIAL_INIT_PRIORITY=51
CONFIG_SHELL=y
CONFIG_SHELL_BACKENDS=y
CONFIG_SHELL_BACKEND_SERIAL=y

CONFIG_SHELL_PROMPT_RTT="CSSE4011:~$"

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
### **3.3 Add KConf to CMakeLists**
Now, we will add these to the build system (as per `OS2.1 - Building Tips`)
```shell 
vim CMakeLists.txt

set(CONF_FILE shell.conf segger_rtt_shell.conf)
```
Now you can compile and flash this application.

## **4.0 Testing**

To connect to the Segger RTT target, open up a terminal and use the following command
```shell
JLinkRTTLogger -Device NRF52840_XXAA -RTTChannel 1 -if SWD -Speed 4000 ~/rtt.log
```
You should see something like....
```
------------------------------------------------------------
Connected to:
  J-Link EDU Mini V1 compiled Dec  7 2021 08:38:51
  S/N: 801026632

Searching for RTT Control Block...OK.
3 up-channels found:
0: Terminal
1: 
2: 
Selected RTT Channel description: 
  Index: 1
  Name:  
  Size:  0 bytes.

Output file: /home/twilfred/rtt.log

Getting RTT data from target. Press any key to quit.
------------------------------------------------------------ 
```

at this point, open up another terminal and
```shell
nc localhost 19021
```
If you setup a console, you should see:
```
SEGGER J-Link V7.58c - Real time terminal output
J-Link EDU Mini V1 compiled Dec  7 2021 08:38:51 V1.0, SN=801026632
Process: JLinkRTTLoggerExe
*** Booting Zephyr OS build zephyr-v2.7.0  ***
```
and any `printk()` used after the bootbanner.

If you had setup a shell, you should see the shell prompt, where you can now enter commands and/or see logging outputs.

## **5.0 Troubleshooting**

1. Make sure that your VM has access to the JLink (USB Passthrough), Test with: `lsusb`
```
$ lsusb
$ Bus 001 Device 003: ID 1366:0101 SEGGER J-Link PLUS
```

2. If this fails:
```shell
JLinkRTTLogger -Device NRF52840_XXAA -RTTChannel 1 -if SWD -Speed 4000 ~/rtt.log
```
Try setting the `-RTTChannel 0`
```shell
JLinkRTTLogger -Device NRF52840_XXAA -RTTChannel 0 -if SWD -Speed 4000 ~/rtt.log
```
3. If a shell prompt doesn't come up at first, in the `nc` window. **Try pressing enter a few times.**

## **6.0 Sample Applications**

Sample applications (shell/console) have been provided, these applications include all the steps mentioned above. You can test your devices with using these apps.

Shell sample is located in **REPO_TOP/tute_solutions/segger_rtt_shell/**

Console sample is located in:

* REPO_TOP/tute_solutions/segger_rtt_console/