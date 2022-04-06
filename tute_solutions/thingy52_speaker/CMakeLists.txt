# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(DTC_OVERLAY_FILE pwm0_en.overlay leds.dtsi)
set(BOARD thingy52_nrf52832)
set(BOARD_FLASH_RUNNER jlink)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(spkr_sample)

target_sources(app PRIVATE src/main.c)
