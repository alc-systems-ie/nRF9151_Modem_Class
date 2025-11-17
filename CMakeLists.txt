cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(cellular_fundamentals)

target_sources(app PRIVATE 
  src/main.cpp
  src/alc_modem.cpp
  src/alc_ui.cpp)

zephyr_library_include_directories(.)
