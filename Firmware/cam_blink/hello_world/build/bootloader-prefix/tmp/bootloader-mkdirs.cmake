# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/aeris/esp/esp-idf/components/bootloader/subproject"
  "/home/aeris/work/Skyworks/cam_blink/hello_world/build/bootloader"
  "/home/aeris/work/Skyworks/cam_blink/hello_world/build/bootloader-prefix"
  "/home/aeris/work/Skyworks/cam_blink/hello_world/build/bootloader-prefix/tmp"
  "/home/aeris/work/Skyworks/cam_blink/hello_world/build/bootloader-prefix/src/bootloader-stamp"
  "/home/aeris/work/Skyworks/cam_blink/hello_world/build/bootloader-prefix/src"
  "/home/aeris/work/Skyworks/cam_blink/hello_world/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/aeris/work/Skyworks/cam_blink/hello_world/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/aeris/work/Skyworks/cam_blink/hello_world/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
