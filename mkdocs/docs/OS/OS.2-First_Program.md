# CSSE4011: Tute 2 - First Program

## 1.0 Motivation

The following tutorial will focus on developing your own very first Zephyr application. This involves, setting up an appropriate directory structure and necessary configuration files.

## 1.1 Hardware Required

* Segger JLink EDU Mini

* Particle Argon

* 2xMicro USB Cables

**Connection guide:**
1. Connect the Particle Argon using the micro-usb cable to your computer
2. Connect JLink ribbon cable to the JTAG header on the Particle Argon, and the opposing end to the J-Link EDU-mini. The lip of the header should be facing away from the edu mini.
3. Connect the JLink edu mini to your computer, and make sure that USB passthrough for this device is enabled in your virtual machine settings. 

## 2.0 Directory Overview

A typical Zephyr application is [organized](https://docs.zephyrproject.org/latest/application/index.html) in the following format.

```
<home>/apps
├── CMakeLists.txt
├── prj.conf
└── src
    └── main.c
```

In this course, you will be required to modify and re-use code to implement new features/applications. To allow for this, code that is not specific to an application is placed in a 'myoslib' directory. Where code within 'myoslib' could be drivers for a particular module, for example, an ultrasonic sensor.  The structure below is an example representation.

```
.
├── csse4011_repo/apps/
│   ├── prac1/
│   │   ├── CMakeLists.txt
│   │   ├── prj.conf
│   │   └── src/
│   │       └── main.c 
│   └── prac2/
│       ├── CMakeLists.txt
│       ├── prj.conf
│       └── src/
│           └── main.c
|
└── csse4011_repo/myoslib/
    ├── hal/
    │   ├── status_leds/
    │   │   ├── hal_status_leds.c 
    │   │   └── hal_status_leds.h 
    │   └── ultrasonic/
    │       ├── hal_ultrasonic.c
    │       └── hal_ultrasonic.h
    └── cli/
        ├── cli_kernel_time_cmd.c
        └── cli_status_leds_cmd.c
```

## **3.0 Making A Zephyr Application**
Within the 'csse4011_repo', make the following modifications to setup a sample directory
```shell
cd ~/csse4011/csse4011_repo/
mkdir -p apps/sample
```
For the purposes of this tutorial, we will use the existing blinky sample provided in the Zephyr source code.

Start by navigating to the Zephyr source we attained in Tute 1.

```shell
cd ~/csse4011/zephyrproject/zephyr/samples/basic/blinky
```
Copy blinky files into our sample application directory, then navigate to sample directory.
```shell
cp -R * ~/csse4011/csse4011_repo/apps/sample/
cd ~/csse4011/csse4011_repo/apps/sample/
```
Verify that the following exist within your sample dir
```
.
└── csse4011/csse4011_repo/apps/
    └── sample/
        ├── CMakeLists.txt
        ├── prj.conf
        ├── README.rst
        ├── sample.yaml
        └── src/
            └── main.c
```
Add ZEPHYR_BASE to path (Lets you build from outside the Zephyr Source Dir). Append the following line to the end of 'bashrc'.
```
vim ~/.bashrc                               or use any text editor
    
#Add Zephyr Base to PATH
export ZEPHYR_BASE=~/csse4011/zephyrproject/zephyr   
```
save and exit.

Read and execute bashrc (Loads ZEPHYR_BASE for current shell instance)
```shell
source ~/.bashrc
echo $ZEPHYR_BASE
```
Should now display the path to zephyr base. 

Build the sample application within our directory
```shell
cd ~/csse4011/csse4011_repo/apps/sample/
west build -p auto -b particle_argon
```
Flash and Verify that Blinky works. 
```shell
west flash -r jlink
```

The option '-r' lets you specify a runner, in this case we are using JLink to flash the Particle Argon. Make sure that the cables are connected properly.

If flashing fails, refer to the end of `OS.1-Getting Started` and follow instructions for installing runners / additional runners. 


## **4.0 Sample**

A sample of this is located within

> tute_solutions/blinky_tute/
