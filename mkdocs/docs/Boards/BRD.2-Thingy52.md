# CSSE4011: Thingy52: Zephyr Board Guide 

## **1.0 Prerequisites**

Complete following OS tutes

  1. Getting Started
  2. First Program

### **1.1 Test Hardware**

* Thingy52
* 2x mUSB Cable
* Segger J-Link EDU mini

## **3.0 Thingy52 - Zephyr** 

The thingy52 has a simple build and flash procedure. To build for the thingy52 simply:

```shell
#Within the application directory
west build -p auto -b thingy52_nrf52832 
```

the thingy52 can now be flashed with

```shell
#Within the application directory
west flash -r jlink
```



