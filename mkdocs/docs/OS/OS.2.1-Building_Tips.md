# CSSE4011: Tute 2.2 - Application Building Tips

## **1.0 Motivation**

This tutorial will focus on modifications to the CMakeLists file to streamline the application development process. 

## **2.0 Setting Build Options**

Note that these should be added to the top of the CMakeLists file. Additionally, there are other ways to include config files and devicetree overlays. Refer to a [application development](https://docs.zephyrproject.org/latest/application/index.html) and [device tree](https://docs.zephyrproject.org/latest/guides/dts/howtos.html#set-devicetree-overlays) guides for more.

### **2.1 Boards**

 A board to develop can be specified within the top level CMakeLists file as below, the particular name for the board can be found under *Zephyr Supported Boards -> boardname* or [here](https://docs.zephyrproject.org/latest/boards/index.html).

 ```
 set(BOARD board_name)

 set(BOARD particle_argon)
 ```

With this set, west build can now omit the '-b board_name' option. By just using 

```
west build 
```

an application can be created for the particle_argon.

### **2.2 Runners**

The flash runner for the board can be set using the below, within the CMakeLists file.

```
set(BOARD_FLASH_RUNNER runner)

set(BOARD_FLASH_RUNNER jlink)
```

This allows for west flash to be ran with about specifying the '-r runner' when invoked. 

```
west flash
```

### **2.3 Config Files**

Config option files (ending with .conf), can be added to the CMakeLists file using,

```
set(CONF_FILE files.conf)

set(CONF_FILE general_config.conf usb_config.conf uart_shell_config.conf)
```

### **2.4 Devicetree overlays**

Devicetree overlay files can be added to the build system using,

```
set(DTC_OVERLAY_FILE overlay.dtsi)

set(DTC_OVERLAY_FILE dts/spi3.dtsi)
```
