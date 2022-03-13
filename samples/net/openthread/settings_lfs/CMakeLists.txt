# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(settings_lfs)

target_sources(app PRIVATE src/main.c)

target_compile_definitions(ot-config INTERFACE "OPENTHREAD_CONFIG_MLE_CHILD_TIMEOUT_DEFAULT=360")