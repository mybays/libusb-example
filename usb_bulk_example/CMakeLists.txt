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

AUX_SOURCE_DIRECTORY(./ SOURCE_FILES)
add_executable(usb_bulk_example ${SOURCE_FILES})
target_link_libraries(usb_bulk_example ${LIBUSB_LIBRARIES})