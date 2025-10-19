cmake_minimum_required(VERSION 3.15)
project(umfeld-library-gpio-linux LANGUAGES CXX)

set(LIBGPIO_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(umfeld-library-gpio-linux INTERFACE)

target_sources(umfeld-library-gpio-linux INTERFACE
    ${LIBGPIO_PATH}/src/gpio-linux.cpp
)

target_include_directories(umfeld-library-gpio-linux INTERFACE
    "${LIBGPIO_PATH}/src"
)

# libgpiod via pkg-config → expose a :: target, then link it
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

add_library(libgpiod::gpiod INTERFACE IMPORTED)
target_link_libraries(libgpiod::gpiod INTERFACE PkgConfig::GPIOD)

target_link_libraries(umfeld-library-gpio-linux INTERFACE
    libgpiod::gpiod
    umfeld-lib-interface
)
