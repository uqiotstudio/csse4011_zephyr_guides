# CSSE4011: nRF Dongle: Zephyr Board Guide 

## **1.0 Prerequisites**

Complete following OS tutes

  1. Getting Started
  2. First Program

### **1.1 Test Hardware**

* nRF Dongle

## **2.0 Tools**

For flashing this board, we need to install `nrfutil`.

```SHELL
pip3 install nrfutil

nrfutil
```

```SHELL
Usage: nrfutil [OPTIONS] COMMAND [ARGS]...

Options:
  -v, --verbose            Increase verbosity of output. Can be specified
                           more than once (up to -v -v -v -v).
  -o, --output <filename>  Log output to file
  --help                   Show this message and exit.


```

### **2.1 TTY Permissions for Flashing**

TTY permissions are required to load the binary by `nrfutil` . An easy way to do this is to add the current user to the 'dialout' group.

```shell
sudo usermod -a -G dialout $USER
```

You will **typically need to log out and log in or reboot to see this effect**. 

### **3.0 nRF Dongle (nRF52840) - Zephyr** 

To build an application, from within the application directory

```SHELL
west build -p auto -b nrf52840dongle_nrf52840
```

Package the application for the bootloader using `nrfutil`:

```SHELL
nrfutil pkg generate --hw-version 52 --sd-req=0x00 \
        --application build/zephyr/zephyr.hex \
        --application-version 1 blinky.zip
```

Now, put the device into bootloader mode, by pressing the **sideways** push button (next to the white pushbutton). You should see the red led **steadily** blinky.

```SHELL
nrfutil dfu usb-serial -pkg blinky.zip -p /dev/ttyACM0
```

### **4.0 Troubleshooting** 

If you get a permissions error when flashing, it is likely because you did not log out/in after updating groups.

If it fails to flash, it could be because the VM does not see the `DFU Bootloader`. 
Run:

```SHELL
lsusb
``` 
and see if the following comes up

```SHELL
Bus 001 Device 009: ID 1915:521f Nordic Semiconductor ASA Open DFU Bootloader
```

If not, you will need to edit VM settings and passthrough this USB from the host to the VM.