# CSSE4011: B-L475E-IOT: Zephyr Board Guide
Reference: [https://docs.zephyrproject.org/3.1.0/boards/arm/disco_l475_iot1/doc/index.html](B-L475E-IOT Zephyr Doc)

## **1.0 Prerequisites**

Complete following OS tutes

  1. Getting Started
  2. First Program

### **1.1 Test Hardware**

* B-L475E-IOT Board
* 1 x mUSB Cable

## **2.0 * B-L475E-IOT Board - Zephyr** 

The * B-L475E-IOT Board has a simple build and flash procedure. To build for it:

```shell
#Within the application directory
west build -p auto -b disco_l475_iot1
```

the B-L475E-IOT  can now be flashed with

```shell
#Within the application directory
west flash
```



