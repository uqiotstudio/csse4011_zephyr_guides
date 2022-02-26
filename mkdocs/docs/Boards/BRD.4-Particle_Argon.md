# CSSE4011: Particle_Boron: Zephyr Board Guide 

## **1.0 Prerequisites**

Complete following OS tutes

  1. Getting Started
  2. First Program

### **1.1 Test Hardware**

* Particle Argon
* 2 x mUSB Cable
* Segger J-Link EDU mini

## **3.0 Particle Argon - Zephyr** 

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



