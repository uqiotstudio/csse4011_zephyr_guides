# CSSE4011: Arduino-Sense: Zephyr Board Guide 

## 1.0 Prerequisites

Complete follwing OS tutes

  1. Getting Started
  2. First Program

## 2.0 Arduino Sense - Zephyr 

This Arduino board requires an extra step compared to building and flashing a most other boards. This particular board requires "the Arduino variant of bossac". Where, BOSSA - "Basic Open Source Sam-ba Application" and "SAM-BA" is the "SAM Boot Assistant". BOSSAC is a command-line implementation of BOSSA.


# 2.1 Getting the BOSSAC

The bossac can be setup in a few different ways, the easiest is to clone the appropriate repository and compile the program. 


In the csse4011-vm, after completing the getting started guide, perform the following.

Step 1: Install Dependencies
```shell
sudo apt-get install libreadline-dev libwxgtk3.0-gtk3-dev
```
Step 2: Clone BOSSA
```shell
cd csse4011/
git clone https://github.com/arduino/BOSSA.git
```
Step 3: Make Bossa
```shell
cd BOSSA/
make
```
Step 4: Verify that bin/ has been succefully created with a binary named 'bossac'. 


# 2.2 TTY Permissions for Flashing

To allow for west to load binary into bootloader, tty permissions must be given to the user. An easy way to do this is to add the current user to the 'dialout' group.

```shell
sudo usermod -a -G dialout $USER
```

You will **typically need to log out and log in or reboot to see this effect**. 

# 2.3 Building and Flashing

Ensure that the USB is connected to the Arduino Sense board, and that **it is passed through by the host machine into the virtual machine.**

An application for this board can be built with, (from within Zephyr Application directory - see OS.1)

```shell
west  build -p auto -b arduino_nano_33_ble
```
and flashed with

```shell
west flash --bossac=$HOME/csse4011/BOSSA/BOSSA/bin/bossac
```

where $HOME is typically, */home/user*, so ensure that the path to 'bossac' is correct.

# Links
[1]https://docs.zephyrproject.org/latest/boards/arm/arduino_nano_33_ble/doc/index.html