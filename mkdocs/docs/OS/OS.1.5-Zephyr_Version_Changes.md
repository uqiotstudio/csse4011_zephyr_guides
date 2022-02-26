# CSSE4011: Tute 1.5 - Zephyr Version Changes

## **1.0 Motivation**

This guide will cover how you can go about quickly changing the version of Zephyr. Why might you want to do this?

*   Newly implemented features you  might want to try.
*   Bug fixes
*   Compatibility Issues
*   Worked on version X, but not on version Y.

## **2.0 Upgrading/Downgrading**

Start by navigating to Zephyr source location.

```SHELL
$ cd zephyrproject/zephyr/
```
fetch any updates/changes
```SHELL
$ git fetch --all
$ git checkout main

#Bring across all changes
$ git pull
```
To see what the latest releases
```SHELL
$ git tag -l | tail

...
zephyr-v2.5.0
zephyr-v2.6.0
zephyr-v2.7.0
zephyr-v2.7.1
zephyr-v3.0.0
```
checkout the desired release tag/version
```SHELL
$ git checkout zephyr-v3.0.0
```
Update `west` to match release requirements
```SHELL
west update
```

## **3.0 Build an app**

Any app you build now (assuming this is where `ZEPHYR_BASE` is pointing to) will build now on the version you have changed to. If you have build errors, make sure to remove the `build/` directory for the application.

```SHELL
$ west build -p -b particle_argon samples/basic/blinky
.
.
.
-- west build: building application
[1/157] Preparing syscall dependency handling

[2/157] Generating include/generated/version.h
-- Zephyr version: 3.0.0 (/home/twilfred/Embedded_Dev/zephyrproject/zephyr), build: zephyr-v3.0.0
[147/157] Linking C executable zephyr/zephyr_pre0.elf

[151/157] Linking C executable zephyr/zephyr_pre1.elf

[157/157] Linking C executable zephyr/zephyr.elf
Memory region         Used Size  Region Size  %age Used
           FLASH:       19128 B         1 MB      1.82%
            SRAM:        4168 B       256 KB      1.59%
        IDT_LIST:          0 GB         2 KB      0.00%
```
