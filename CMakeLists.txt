cmake_minimum_required(VERSION 3.24)

project(MinecraftConsoles LANGUAGES C CXX)
if(MSVC)
  enable_language(RC ASM_MASM)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(PLATFORM_NAME STREQUAL "3DS")
  # Legacy code uses an unqualified byte typedef that conflicts with std::byte in C++17.
  set(CMAKE_CXX_STANDARD 14)
endif()

if(NOT WIN32 AND NOT CMAKE_CROSSCOMPILING)
  message(FATAL_ERROR "This CMake build currently supports Windows or approved cross-compiling toolchains only.")
endif()

if(NOT CMAKE_CROSSCOMPILING AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "Use a 64-bit generator/toolchain (x64).")
endif()

set(CMAKE_CONFIGURATION_TYPES
  "Debug"
  "Release"
  CACHE STRING "" FORCE
)
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

function(configure_compiler_target target)
  # MSVC and compatible compilers (like Clang-cl)
  if (MSVC)
    target_compile_options(${target} PRIVATE
      $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C,CXX>>:/W3>
      $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C,CXX>>:/W0>
      $<$<COMPILE_LANGUAGE:C,CXX>:/MP>
      $<$<COMPILE_LANGUAGE:C,CXX>:/FS>
      $<$<COMPILE_LANGUAGE:C,CXX>:/GS>
      $<$<COMPILE_LANGUAGE:CXX>:/EHsc>
      $<$<COMPILE_LANGUAGE:CXX>:/GR>
      $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C,CXX>>:/Od>
      $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C,CXX>>:/O2 /Oi /GT /GF>
    )
  endif()

  # MSVC
  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${target} PRIVATE
      $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C,CXX>>:/GL>
    )
    target_link_options(${target} PRIVATE
      $<$<CONFIG:Release>:/LTCG:incremental>
    )
  endif()

  # Clang
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${target} PRIVATE
      $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C,CXX>>:-O0 -Wall>
      $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C,CXX>>:-O2 -w -flto>
    )
    target_link_options(${target} PRIVATE
      $<$<CONFIG:Release>:-flto>
    )
  endif()
endfunction()


# ---
# Configuration
# ---
set(MINECRAFT_SHARED_DEFINES
  _LARGE_WORLDS
  _DEBUG_MENUS_ENABLED
  $<$<CONFIG:Debug>:_DEBUG>
  _CRT_NON_CONFORMING_SWPRINTFS
  _CRT_SECURE_NO_WARNINGS
)

# Add platform-specific defines
list(APPEND MINECRAFT_SHARED_DEFINES ${PLATFORM_DEFINES})

# ---
# Sources
# ---
option(MCLCE_3DS_BUILD_FULL "Build full Minecraft.Client + Minecraft.World for 3DS (experimental, will require porting fixes)." OFF)

if(PLATFORM_NAME STREQUAL "3DS" AND NOT MCLCE_3DS_BUILD_FULL)
  add_executable(Minecraft.3DS
    "${CMAKE_SOURCE_DIR}/cmake/3ds/main.cpp"
  )
  target_link_libraries(Minecraft.3DS PRIVATE
    "$<$<CONFIG:Debug>:${DEVKITPRO}/libctru/lib/libctrud.a>"
    "$<$<NOT:$<CONFIG:Debug>>:${DEVKITPRO}/libctru/lib/libctru.a>"
  )

  set_target_properties(Minecraft.3DS PROPERTIES
    OUTPUT_NAME "Minecraft3DS"
    SUFFIX ".elf"
  )

  find_program(THREEDSXTOOL
    NAMES 3dsxtool 3dsxtool.exe
    HINTS "${DEVKITPRO}/tools/bin"
  )

  if(THREEDSXTOOL)
    set(MINECRAFT_3DSX_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Minecraft3DS.3dsx")
    add_custom_command(
      OUTPUT "${MINECRAFT_3DSX_OUTPUT}"
      COMMAND ${THREEDSXTOOL} "$<TARGET_FILE:Minecraft.3DS>" "${MINECRAFT_3DSX_OUTPUT}"
      DEPENDS Minecraft.3DS
      COMMENT "Generating Minecraft3DS.3dsx"
      VERBATIM
    )

    add_custom_target(Minecraft.3DSX ALL
      DEPENDS "${MINECRAFT_3DSX_OUTPUT}"
    )
    set_property(TARGET Minecraft.3DSX PROPERTY FOLDER "Build")
  endif()
else()
  add_subdirectory(Minecraft.World)
  add_subdirectory(Minecraft.Client)
  if(PLATFORM_NAME STREQUAL "Windows64") # Server is only supported on Windows for now
    add_subdirectory(Minecraft.Server)
  endif()
endif()

# ---
# Build versioning
# ---
set(BUILDVER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateBuildVer.cmake")
set(BUILDVER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated/Common/BuildVer.h")

add_custom_target(GenerateBuildVer
  COMMAND ${CMAKE_COMMAND}
    "-DOUTPUT_FILE=${BUILDVER_OUTPUT}"
    -P "${BUILDVER_SCRIPT}"
  COMMENT "Generating BuildVer.h..."
  VERBATIM
)

if(PLATFORM_NAME STREQUAL "3DS" AND NOT MCLCE_3DS_BUILD_FULL)
  add_dependencies(Minecraft.3DS GenerateBuildVer)
else()
  add_dependencies(Minecraft.World GenerateBuildVer)
  add_dependencies(Minecraft.Client GenerateBuildVer)
  if(PLATFORM_NAME STREQUAL "Windows64")
    add_dependencies(Minecraft.Server GenerateBuildVer)
  endif()
endif()

# ---
# Project organisation
# ---
# Set the startup project for Visual Studio
if(PLATFORM_NAME STREQUAL "3DS" AND NOT MCLCE_3DS_BUILD_FULL)
  set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Minecraft.3DS)
else()
  set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Minecraft.Client)
endif()

# Setup folders for Visual Studio, just hides the build targets under a sub folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET GenerateBuildVer PROPERTY FOLDER "Build")
if(PLATFORM_NAME STREQUAL "3DS" AND NOT MCLCE_3DS_BUILD_FULL)
  set_property(TARGET Minecraft.3DS PROPERTY FOLDER "Build")
endif()
