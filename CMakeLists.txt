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

function(configure_3ds_packaging target output_basename)
  string(REPLACE "." "_" _mclce_pkg_target_base "${target}")

  find_program(MAKEROM
    NAMES makerom makerom.exe
    HINTS "${DEVKITPRO}/tools/bin"
  )

  if(NOT MAKEROM)
    message(FATAL_ERROR "makerom not found")
  endif()

  set(_MCLCE_3DS_ROMFS_ROOT "${CMAKE_CURRENT_BINARY_DIR}/minecraft3ds_romfs_root")
  file(TO_CMAKE_PATH "${_MCLCE_3DS_ROMFS_ROOT}" _MCLCE_3DS_ROMFS_ROOT_CMAKE)

  set(_MCLCE_3DS_ROMFS_ASSET_PAIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/music"           "music"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/Common/Media"    "Common/Media"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/Common/res"      "Common/res"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/Common/Trial"    "Common/Trial"
    "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/Common/Tutorial" "Common/Tutorial"
  )
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/${PLATFORM_NAME}Media")
    list(APPEND _MCLCE_3DS_ROMFS_ASSET_PAIRS
      "${CMAKE_CURRENT_SOURCE_DIR}/Minecraft.Client/${PLATFORM_NAME}Media" "${PLATFORM_NAME}Media"
    )
  endif()

  # --- RSF ---
  set(_MCLCE_3DS_RSF "${CMAKE_CURRENT_BINARY_DIR}/minecraft3ds.rsf")
  file(WRITE "${_MCLCE_3DS_RSF}" "
SystemControlInfo:
  StackSize: 0x00040000
  SaveDataSize: 0
  JumpId: 0

AccessControlInfo:
  CoreVersion: 2
  Priority: 64
  IdealProcessor: 0
  AffinityMask: 1
  HandleTableSize: 0x200
  MemoryType: Application
  SystemMode: 64MB
  EnableL2Cache: true

  FileSystemAccess:
    - \"fs:USER\"

  ServiceAccessControl:
    - \"apt:u\"
    - \"fs:USER\"
    - \"hid:USER\"
    - \"gfx:USER\"

  SystemCallAccess:
    - \"svcBreak\"
    - \"svcSleepThread\"
    - \"svcGetSystemTick\"

RomFs:
  RootPath: \"${_MCLCE_3DS_ROMFS_ROOT_CMAKE}\"
")

  add_custom_target(${_mclce_pkg_target_base}_romfs_root ALL
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${_MCLCE_3DS_ROMFS_ROOT}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${_MCLCE_3DS_ROMFS_ROOT}"
  )

  list(LENGTH _MCLCE_3DS_ROMFS_ASSET_PAIRS _mclce_romfs_pair_count)
  math(EXPR _mclce_romfs_last_pair "${_mclce_romfs_pair_count} - 1")
  foreach(i RANGE 0 ${_mclce_romfs_last_pair} 2)
    math(EXPR j "${i} + 1")
    list(GET _MCLCE_3DS_ROMFS_ASSET_PAIRS ${i} src)
    list(GET _MCLCE_3DS_ROMFS_ASSET_PAIRS ${j} dest)
    list(APPEND _mclce_pkg_target_romfs_commands
      COMMAND ${CMAKE_COMMAND}
        "-DCOPY_SOURCE=${src}"
        "-DCOPY_DEST=${_MCLCE_3DS_ROMFS_ROOT}/${dest}"
        -P "${CMAKE_SOURCE_DIR}/cmake/CopyFolderScript.cmake"
    )
  endforeach()

  if(DEFINED _mclce_pkg_target_romfs_commands)
    add_custom_target(${_mclce_pkg_target_base}_romfs_copy ALL
      ${_mclce_pkg_target_romfs_commands}
      COMMENT "Staging RomFS root for ${output_basename}..."
      VERBATIM
    )
    add_dependencies(${_mclce_pkg_target_base}_romfs_copy ${_mclce_pkg_target_base}_romfs_root)
    set_property(TARGET ${_mclce_pkg_target_base}_romfs_copy PROPERTY FOLDER "Build")
  endif()

  # --- OUTPUT PATHS ---
  set(_MCLCE_NCCH_OUT "${CMAKE_CURRENT_BINARY_DIR}/${output_basename}.ncch")
  set(_MCLCE_CIA_OUT  "$<TARGET_FILE_DIR:${target}>/${output_basename}.cia")
  set(_MCLCE_3DS_OUT  "$<TARGET_FILE_DIR:${target}>/${output_basename}.3ds")

  # --- NCCH ---
  add_custom_target(${_mclce_pkg_target_base}_cxi ALL
    COMMAND ${MAKEROM}
      -f cxi
      -o "${_MCLCE_NCCH_OUT}"
      -elf "$<TARGET_FILE:${target}>"
      -rsf "${_MCLCE_3DS_RSF}"
      -exefslogo
    DEPENDS ${target}
    COMMENT "Generating ${output_basename}.cxi"
    VERBATIM
  )
  if(TARGET ${_mclce_pkg_target_base}_romfs_copy)
    add_dependencies(${_mclce_pkg_target_base}_cxi ${_mclce_pkg_target_base}_romfs_copy)
  endif()

  # --- CIA ---
  add_custom_target(${_mclce_pkg_target_base}_cia ALL
    COMMAND ${MAKEROM}
      -f cia
      -o "${_MCLCE_CIA_OUT}"
      -content "${_MCLCE_NCCH_OUT}:0:0:0"
    DEPENDS ${_mclce_pkg_target_base}_ncch
    COMMENT "Generating ${output_basename}.cia"
    VERBATIM
  )

  # --- 3DS ---
  add_custom_target(${_mclce_pkg_target_base}_cci ALL
    COMMAND ${MAKEROM}
      -f cci
      -o "${_MCLCE_3DS_OUT}"
      -content "${_MCLCE_NCCH_OUT}:0:0:0"
    DEPENDS ${_mclce_pkg_target_base}_cxi
    COMMENT "Generating ${output_basename}.3ds"
    VERBATIM
  )
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

    configure_3ds_packaging(Minecraft.3DS Minecraft3DS)
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
