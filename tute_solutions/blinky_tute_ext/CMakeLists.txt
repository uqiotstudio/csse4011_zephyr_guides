# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

#Add Include Directories
include_directories(
			../lib/led_driver/
			)


#Add Target Sources
target_sources(app PRIVATE 
			src/main.c
			../lib/led_driver/led_driver.c
			)
