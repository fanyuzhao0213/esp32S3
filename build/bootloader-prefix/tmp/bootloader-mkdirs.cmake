# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/espidf/v5.0.7/esp-idf/components/bootloader/subproject"
  "G:/esp32S3/my_esp32_test/build/bootloader"
  "G:/esp32S3/my_esp32_test/build/bootloader-prefix"
  "G:/esp32S3/my_esp32_test/build/bootloader-prefix/tmp"
  "G:/esp32S3/my_esp32_test/build/bootloader-prefix/src/bootloader-stamp"
  "G:/esp32S3/my_esp32_test/build/bootloader-prefix/src"
  "G:/esp32S3/my_esp32_test/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "G:/esp32S3/my_esp32_test/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "G:/esp32S3/my_esp32_test/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
