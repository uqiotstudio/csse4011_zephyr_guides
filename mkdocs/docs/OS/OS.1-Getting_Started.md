# CSSE4011: Tute 1 - Getting Started with Zephyr RTOS

## **1.0 Zephyr RTOS: Overview**

Zephyr Real-Time Operating System (RTOS) managed by the Linux foundation, is designed for connected and resource constrained embedded devices. Unlike many other real-time operating systems (like free-RTOS), Zephyr is more than just a kernel, it is a software eco-system. Zephyr integrates a broad range of functionality such as:

1. Bluetooth
2. LoRaWan
3. USB / Serial Console / Shell(CLI)
4. SPI/I2C/UART
5. Flash Drivers
6. Filesystems

and more... Depending on what particular feature your application might require, Zephyr allows you to build a program using only the necessary functionality (i.e BLE), this can optimize the overall footprint of binary. This is done using config file(s) that we will cover in the coming tutorials. 


## **1.1 Zephyr Kernel Overview**

The Zephyr kernel is similar to that of free-RTOS at a high level, in that it is primarily concerned with task-scheduling, inter-task communication and synchronization. 

1. Zephyr offers a pre-emptive scheduler, that is, the code context can change at anytime. 
2. Thread based, where 'threads' mean the same thing as a 'task' in free-RTOS. 
3. It is **'Soft'** real-time provided by system ticks


## **1.2 Zephyr West Meta-Tool**

Zephyr includes a command line tool named west. This tool provides a multiple repository management system, and Zephyr uses west to provide conveniences for building, flashing, debugging and cleaning applications. For example, to build a program you could use, 
```shell
west build
```
This will look at predefined build configuration settings and build an application as specified. The west meta-tool can be used to perform many other functions, see [here](https://docs.zephyrproject.org/latest/guides/west/index.html) for a detailed breakdown of it's features. In this course, you will be using west to build, flash and maintain your workspaces. 


## **1.3 Zephyr Documentation**
The Zephyr documentation is well written and contains all the intricate details about Zephyr and it's features. Throughout this course and application development, you will need to refer back to these documentation to understand;

1. [The Build and Configuration Systems](https://docs.zephyrproject.org/latest/guides/build/index.html)                            
2. [Device Tree (A way of describing hardware config to the OS)](https://docs.zephyrproject.org/latest/guides/dts/intro.html)      
3. [Application Development (App config options/Kernel config)](https://docs.zephyrproject.org/latest/application/index.html)
4. [API Usage](https://docs.zephyrproject.org/latest/application/index.html#)                                                        
5. [Using an Interactive Kconfig interfaces (Optional)](https://docs.zephyrproject.org/latest/guides/build/kconfig/menuconfig.html)               

Take some time to read through these documents to get an idea of how Zephyr is implemented. This will be useful later when you are required to implement advanced features using Zephyr. 

## **2.0 Installing Zephyr RTOS**

The following guide is made in reference to the [Zephyr "Getting Started Guide" documentation](https://docs.zephyrproject.org/latest/getting_started/index.html), and is intended to be used on a Debian based linux distribution with the **"apt"** (package manager installed). 

First update the system and any installed packages on your VM
```shell
sudo apt update 
sudo apt full-upgrade
```
Reboot (If this was a fresh OS install, typically kernel is upgraded)
```shell
sudo reboot
```
Update sources list by adding the Kitware APT repo (contains cmake etc...)
```shell
cd ~  

wget https://apt.kitware.com/kitware-archive.sh
sudo bash kitware-archive.sh
sudo rm bash kitware-archive.sh                                   - Remove script
```
Install cmake, python3 and dtc (device tree compiler)
```shell
sudo apt install --no-install-recommends git cmake ninja-build gperf \
        ccache dfu-util device-tree-compiler wget \
        python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
        make gcc gcc-multilib g++-multilib libsdl2-dev
```
Verify dependencies are installed before continuing
```shell
cmake --version
python3 --version
dtc --version
```
## **3.0 Get Zephyr and Install Python dependencies**
Install West (Zephyr Meta-tool)
```shell
pip3 install --user -U west

echo 'export PATH=~/.local/bin:"$PATH"' >> ~/.bashrc
source ~/.bashrc
```
Setup Zephyr Folder
```shell
cd /home/user
mkdir csse4011/
cd csse4011/

west init -m https://github.com/zephyrproject-rtos/zephyr --mr v2.7.0 zephyrproject 
```

or 

```shell
west init zephyrproject/               - Latest version of Zephyr (the development tree, might be unstable)
```
Get Zephyr Source Code (Gets ALL of the source)
```shell
cd ~/zephyrproject
west update                                 - Might take awhile 
```
Export a Zephyr CMake package. This allows CMake to automatically load boilerplate code required for building Zephyr applications.
```shell
west zephyr-export
```
Zephyr’s scripts/requirements.txt file declares additional Python dependencies. Install them with pip3.
```shell
pip3 install --user -r ~/csse4011/zephyrproject/zephyr/scripts/requirements.txt
```
## **4.0 Installing the toolchains**

This tutorial will focus on installing the Zephyr SDK. You may also use 3rd party toolchains, however, for this course, the SDK will be sufficient. Note that Zephyr SDK is not available of Windows or MacOs. You will need to install a 3rd party toolchain, see [here](https://docs.zephyrproject.org/latest/getting_started/toolchain_3rd_party_x_compilers.html).

Download the Zephyr SDK installer
```shell
cd ~

wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.13.2/zephyr-sdk-0.13.2-linux-x86_64-setup.run
```
Install the SDK to /opt/ , see [here](https://docs.zephyrproject.org/latest/getting_started/index.html) for other potential installation locations
```shell
chmod +x zephyr-sdk-0.13.2-linux-x86_64-setup.run

sudo ./zephyr-sdk-0.13.2-linux-x86_64-setup.run -- -d /opt/zephyr-sdk-0.13.2
```
Remove SDK Installer (Optional)
```shell
cd ~
rm zephyr-sdk-0.13.2-linux-x86_64-setup.run
```
Set udev rules, which allow you to flash most Zephyr boards as a regular user
```shell
sudo cp /opt/zephyr-sdk-0.13.2/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d

sudo udevadm control --reload
```

## **5.0 Build a Sample Program (Blinky)**

If the installation was successful, we should now be able to build a basic blinky program. In the following command, the '-p auto' option automatically cleans byproducts from a previous build if necessary, which is useful if you try building another sample.

The '-b' option specifies a board to build for, in this case the particle_boron, followed by the path to source.
```shell
cd ~/zephyrproject/zephyr

west build -p auto -b particle_argon samples/basic/blinky
```
Flash the sample program. Make a sure a 'runner' is connected. You will be given a Segger J-Link device to be used with the Particle Argon.
```shell
west flash -r jlink
```

Option '-r' specifies to flash using JLink, west is capable of tapping into different runners for flashing your devices, later we will explore how to specify runners in the CMakeLists file. 

If flashing fails, a runner may need to be installed. When using a Segger J-Link Device. Download and install the following [JLink Software](https://www.segger.com/downloads/jlink/). Under 'J-Link Software and Documentation Pack', download the 'Linux - 64-bit DEB Installer'. Run the installer and install JLink software.

Installation can be verified by running the following command from a terminal
```shell
JLinkExe
exit 
```

Additional information for setting up other runners can be found [here](https://docs.zephyrproject.org/latest/guides/west/build-flash-debug.html)
