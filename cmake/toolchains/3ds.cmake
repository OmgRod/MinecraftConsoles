if(NOT DEFINED ENV{DEVKITPRO})
  message(FATAL_ERROR "DEVKITPRO is not set. Install devkitPro and run CMake from an environment that exports it.")
endif()

set(DEVKITPRO "$ENV{DEVKITPRO}")
set(DEVKITARM "$ENV{DEVKITARM}")

if(NOT DEVKITARM OR NOT EXISTS "${DEVKITARM}")
  set(DEVKITARM "${DEVKITPRO}/devkitARM")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

set(_DEVKITARM_BIN "${DEVKITARM}/bin")
find_program(CMAKE_C_COMPILER NAMES arm-none-eabi-gcc arm-none-eabi-gcc.exe HINTS "${_DEVKITARM_BIN}" REQUIRED)
find_program(CMAKE_CXX_COMPILER NAMES arm-none-eabi-g++ arm-none-eabi-g++.exe HINTS "${_DEVKITARM_BIN}" REQUIRED)
find_program(CMAKE_AR NAMES arm-none-eabi-ar arm-none-eabi-ar.exe HINTS "${_DEVKITARM_BIN}" REQUIRED)
find_program(CMAKE_RANLIB NAMES arm-none-eabi-ranlib arm-none-eabi-ranlib.exe HINTS "${_DEVKITARM_BIN}" REQUIRED)
find_program(CMAKE_STRIP NAMES arm-none-eabi-strip arm-none-eabi-strip.exe HINTS "${_DEVKITARM_BIN}" REQUIRED)

set(CMAKE_FIND_ROOT_PATH
  "${DEVKITPRO}"
  "${DEVKITARM}"
)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(_DEVKITPRO_3DS_COMMON_FLAGS "-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -ffunction-sections -fdata-sections -Wno-psabi")
set(CMAKE_C_FLAGS_INIT "${_DEVKITPRO_3DS_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${_DEVKITPRO_3DS_COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-specs=3dsx.specs -Wl,--gc-sections")
