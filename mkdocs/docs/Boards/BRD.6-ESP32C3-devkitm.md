# CSSE4011: ESP32C3-DevKitM: Zephyr Board Guide 
Reference: [https://docs.zephyrproject.org/3.1.0/boards/riscv/esp32c3_devkitm/doc/index.html](ESP32C3-devkitM Zephyr Doc)

## **1.0 Prerequisites**

Complete following OS tutes

  1. Getting Started
  2. First Program

### **1.1 Test Hardware**

* ESP32C3-DevKitM Board
* 1 x mUSB Cable

## **2.0 *ESP32C3-DevKitM - Zephyr** 

The * ESP32C3-DevKitM Board has a simple build and flash procedure. To build for it:

```shell
#Within the application directory
west build -p auto -b esp32c3_devkitm
```

the ESP32C3-DevKitM   can now be flashed with

```shell
#Within the application directory
west flash
```



