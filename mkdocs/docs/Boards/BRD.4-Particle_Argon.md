# CSSE4011: Particle_Boron: Zephyr Board Guide 

## **1.0 Prerequisites**

Complete following OS tutes

  1. Getting Started
  2. First Program

### **1.1 Test Hardware**

* Particle Argon
* 2 x mUSB Cable
* Segger J-Link EDU mini

## **5.0 Install JLinkSoftware/SEGGER Tools**

**NOTE: For flashing to work (using Segger JLink devices), you must first install [J-Link Software and Documentation pack](https://www.segger.com/downloads/jlink/) in your VM**, use the `64-bit DEB` installer for **Debian/Ubuntu**. 

To install this, open up a terminal and navigate to download location.

```SHELL
cd ~/Downloads

#Make sure the file name is right, versions change often
sudo dpkg -i JLink_Linux_V762_x86_64.deb
```

Make sure that the JLink device is connected up properly to the Thingy52 device, and ensure that the power switch is to the left (facing you - on position). It is a good idea to have the device connected via mUSB for charging during this procedure.

Once this has been completed, you can test that it is functional with

```SHELL
$ JLinkExe
```

```SHELL
SEGGER J-Link Commander V7.62 (Compiled Feb 16 2022 16:09:54)
DLL version V7.62, compiled Feb 16 2022 16:09:39

Connecting to J-Link via USB...O.K.
Firmware: J-Link EDU Mini V1 compiled Dec  7 2021 08:38:51
Hardware version: V1.00
S/N: 801026632
License(s): FlashBP, GDB
USB speed mode: Full speed (12 MBit/s)
VTref=3.286V
```

at this point if you get

```SHELL
Connecting to J-Link via USB...FAILED: Cannot connect to J-Link.
```

This means that either your connections are wrong or more likely that the VM has not been passed the USB from host. You can test this by running.

```SHELL
lsusb

Bus 001 Device 003: ID 1366:0101 SEGGER J-Link PLUS
```
If you do not see this device, check if the host can see this device, if so, you will need to configure the VM to allow passthrough for the SEGGER EDU Mini.

## **3.0 Build a Sample Program (Blinky) for the Argon Board**

If the installation was successful, we should now be able to build a basic blinky program. In the following command, the '-p auto' option automatically cleans byproducts from a previous build if necessary, which is useful if you try building another sample.

The '-b' option specifies a board to build for, in this case the particle_argon, followed by the path to the app source.
```shell
cd ~/csse4011/zephyrproject/zephyr/

west build -p auto -b particle_argon samples/basic/blinky
```
Flash the sample program. Make a sure a 'runner' is connected. You will be given a Segger J-Link device to be used with the Particle Argon.
```shell
west flash -r jlink
```

Option '-r' specifies to flash using JLink, west is capable of tapping into different runners for flashing your devices, later we will explore how to specify runners in the CMakeLists file. 

Additional information for setting up other runners can be found [here](https://docs.zephyrproject.org/latest/guides/west/build-flash-debug.html)

## **4.0 Example with Particle Argon - Zephyr** 

The particle argon has a simple build and flash procedure. To build for it:

```shell
#Within the application directory
west build -p auto -b particle_argon 
```

the argon can now be flashed with

```shell
#Within the application directory
west flash -r jlink
```



