# CSSE4011: Tute 2.2 - Implementing a Simple Library

## **1.0 Motivation**

The following tutorial looks at implementing a simple library and adding library files to the build system. 

## **2.0 Implementing A Basic Library**

## **2.1 Including library files**

We will build on the blinky sample that was created in *OS.2-First_Program*. We will start by creating a library file to implement functions that will:
> Initialize GPIO for led

> Provide a wrapper functions for turning led on and off.

The following directory structure will be implemented for this basic library application. Where, *blinky_tute_ext* is a copy of the application made in *OS.2-First_Program*. 

```shell
.
├── blinky_tute
│   ├── CMakeLists.txt
│   ├── prj.conf
│   └── src
│       └── main.c
├── blinky_tute_ext
│   ├── CMakeLists.txt
│   ├── prj.conf
│   └── src
│       └── main.c
├── lib
│   └── led_driver
│       ├── led_driver.c
│       └── led_driver.h
```
## **2.2 Library Files**

In this example, the library files are in *lib/led_driver/*, where:
> led_driver.c - provides simple led wrapper functions to demonstrate a library implementation.


> led_driver.h - provides the respective header file to be included by any shared targets. 

### **2.2.1 Creating a library source file (led_driver.c)**

First include necessary Zephyr headers files required by the application, in this case, for an led to toggle, we use. 

```
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/* Local Library Include */
#include "led_driver.h"
```
This can be followed by any source code required to implement the application/driver logic. 

### **2.2.2 Creating a library header file (led_driver.h)**

The header file can be made as per usual using typical syntax. There are no particular restrictions on the way this is implemented. 

```
#ifdef LED_DRIVER_H
#define LED_DRIVER_H

/* Device Tree Macros */
#define LED0_NODE DT_ALIAS(led0)

/* Function Prototypes..*/
int led0_init(void);


#endif
```

## **2.3 Adding Sources and Header files to CMakeLists**

With this, you can now created a basic library to implement re-usable code. However, to add this to the build system of this intended application. You must add these files to the CMakeLists file of the target application. In this case the following file  *blinky_tute_ext/CMakeLists.txt*

```
Append the following to the CMakeLists.txt file


#Add Include Directories
include_directories(
                        ../lib/led_driver/
                        )

#Add Target Sources
target_sources(app PRIVATE
                        src/main.c
                        ../lib/led_driver/led_driver.c
                        )
)
```

**Note:** That relative paths are interpreted as relative to the current source directory. 

**Note:** That *include_directories(x/y)* applies the include directories to all targets, if you want to specify which targets use with includes, use *target_include_directories(t x/y)*, see [here](https://cmake.org/cmake/help/v3.20/manual/cmake-buildsystem.7.html#include-directories-and-usage-requirements)

**CMakeLists documentation states that:** The include directories are added to the directory property INCLUDE_DIRECTORIES for the current CMakeLists file. They are also added to the target property INCLUDE_DIRECTORIES for each target in the current CMakeLists file. The target property values are the ones used by the generators.

You can also use a seperate CMakeLists files to compile your libraries using a hierarchical cmake build system if desired. Refer to cmake build system [documentation](https://cmake.org/cmake/help/v3.22/manual/cmake-buildsystem.7.html) for more.

## **2.4 Using Library Code in Target Application**

Finally, we can now use the library code within our target application. To do this, include the respective header files. In this case, include the following in *src/main.c*

```
/* Include file from our library */
#include "led_driver.h"
```

Now you can call the library functions as desired. 

## **3.0 Tutorial Question**
Using the above, extend the blinky program by implementing simple wrapper functions to:
* Init led gpio pin

* Power on/off led0 pin

* Deinit led gpio pin

Refer to the [Zephyr GPIO API](https://docs.zephyrproject.org/latest/reference/peripherals/gpio.html)

## **4.0 Sample Solution**


A sample solution is uploaded in the docs repository. Find located within,

> tute_solutions/blinky_tute_ext/src/

> tute_solutions/lib/led_driver/

This code can be built with:

> west build -p -b <board_name>

and flashed with

> west flash -r 'runner'

Refer to the board flashing tutorials for additional build/flash guides.
