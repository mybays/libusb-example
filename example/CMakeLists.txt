cmake_minimum_required(VERSION 3.5)
project(usb_bulk_example)


find_package(PkgConfig REQUIRED)
message(STATUS "--- PKG_CONFIG_FOUND: ${PKG_CONFIG_FOUND}")
message(STATUS "--- PKG_CONFIG_VERSION_STRING: ${PKG_CONFIG_VERSION_STRING}")


pkg_check_modules(LIBUSB REQUIRED IMPORTED_TARGET libusb-1.0)


MESSAGE(STATUS ${LIBUSB_LIBRARIES})
MESSAGE(STATUS ${LIBUSB_LIBRARY_DIRS})
MESSAGE(STATUS ${LIBUSB_INCLUDE_DIRS})


include_directories(${LIBUSB_INCLUDE_DIRS})
link_directories(${LIBUSB_LIBRARY_DIRS})


add_executable(test test.c)
target_link_libraries(test ${LIBUSB_LIBRARIES})

add_executable(usb2 usb2.c)
target_link_libraries(usb2 ${LIBUSB_LIBRARIES})


pkg_check_modules(LIBUSB-COMPAT REQUIRED IMPORTED_TARGET libusb)


MESSAGE(STATUS ${LIBUSB_LIBRARIES})
MESSAGE(STATUS ${LIBUSB_LIBRARY_DIRS})
MESSAGE(STATUS ${LIBUSB_INCLUDE_DIRS})


include_directories(${LIBUSB-COMPAT_INCLUDE_DIRS})
link_directories(${LIBUSB-COMPAT_LIBRARY_DIRS})


add_executable(usb1 usb1.c)
target_link_libraries(usb1 ${LIBUSB-COMPAT_LIBRARIES})
