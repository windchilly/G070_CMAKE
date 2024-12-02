cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME G070)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# file(GLOB MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

file(GLOB BOARD_SOURCES                 "./board.c")
file(GLOB APP_SOURCES                   "./app/*.c")
file(GLOB CDOS_SOURCES                  "./cdos/*.c")
file(GLOB DEV_DRV_SOURCES               "./dev_drv/*.c")
file(GLOB DRV_SOURCES                   "./drv/*.c")
file(GLOB PKG_CMBACKTRACE_SOURCES       "./packages/cmbacktrace/*.c")
file(GLOB PKG_CMBACKTRACE_S_SOURCES     "./packages/cmbacktrace/*.s")
file(GLOB PKG_HEAP_SOURCES              "./packages/heap/*.c")
file(GLOB PKG_LETTERSHELL_SOURCES       "./packages/letter-shell-master/src/*.c")
file(GLOB PKG_MULTIBUTTON_SOURCES       "./packages/multibutton/*.c")
file(GLOB PKG_SIMPLEGUI_SOURCES         "./packages/simplegui/src/*.c")
file(GLOB PKG_SIMPLEGUI_APP_SOURCES     "./packages/simplegui/app/*.c")
file(GLOB PKG_UMCN_SOURCES              "./packages/umcn/uMCN.c")

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${BOARD_SOURCES}
    ${APP_SOURCES}
    ${CDOS_SOURCES}
    ${DEV_DRV_SOURCES}
    ${DRV_SOURCES}
    ${PKG_CMBACKTRACE_SOURCES}
    ${PKG_CMBACKTRACE_S_SOURCES}
    ${PKG_HEAP_SOURCES}
    ${PKG_LETTERSHELL_SOURCES}
    ${PKG_MULTIBUTTON_SOURCES}
    ${PKG_SIMPLEGUI_SOURCES}
    ${PKG_SIMPLEGUI_APP_SOURCES}
    ${PKG_UMCN_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ./
    ./app
    ./cdos
    ./dev_drv
    ./packages/cmbacktrace
    ./packages/heap
    ./packages/letter-shell-master/src
    ./packages/multibutton
    ./packages/simplegui/inc
    ./packages/umcn
)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objdump -d ${CMAKE_PROJECT_NAME}.elf > ${CMAKE_PROJECT_NAME}.lst
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating assembly listing with objdump"
    VERBATIM
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating hex file with objcopy"
    VERBATIM
)

# Generate bin file with objcopy
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating bin file with objcopy"
    VERBATIM
)
