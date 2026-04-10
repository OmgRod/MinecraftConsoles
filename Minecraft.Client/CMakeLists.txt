include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources/Common.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources/Durango.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources/ORBIS.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources/PS3.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources/PSVita.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources/Windows.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources/3DS.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/sources/Xbox360.cmake")

include("${CMAKE_SOURCE_DIR}/cmake/CommonSources.cmake")

include("${CMAKE_SOURCE_DIR}/cmake/Utils.cmake")

# Combine all source files into a single variable for the target
# We cant use CMAKE_CONFIGURE_PRESET here as VS doesn't set it, so just rely on the folder
set(MINECRAFT_CLIENT_SOURCES
  ${MINECRAFT_CLIENT_COMMON}
  $<$<STREQUAL:${PLATFORM_NAME},Durango>:${MINECRAFT_CLIENT_DURANGO}>
  $<$<STREQUAL:${PLATFORM_NAME},Orbis>:${MINECRAFT_CLIENT_ORBIS}>
  $<$<STREQUAL:${PLATFORM_NAME},3DS>:${MINECRAFT_CLIENT_3DS}>
  $<$<STREQUAL:${PLATFORM_NAME},PS3>:${MINECRAFT_CLIENT_PS3}>
  $<$<STREQUAL:${PLATFORM_NAME},PSVita>:${MINECRAFT_CLIENT_PSVITA}>
  $<$<STREQUAL:${PLATFORM_NAME},Windows64>:${MINECRAFT_CLIENT_WINDOWS}>
  $<$<STREQUAL:${PLATFORM_NAME},Xbox>:${MINECRAFT_CLIENT_XBOX360}>
  ${SOURCES_COMMON}
)

add_executable(Minecraft.Client ${MINECRAFT_CLIENT_SOURCES})

# Only define executable on windows
if(PLATFORM_NAME STREQUAL "Windows64")
  set_target_properties(Minecraft.Client PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

target_include_directories(Minecraft.Client PRIVATE
  "${CMAKE_BINARY_DIR}/generated/" # This is for the generated BuildVer.h
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/Iggy/include"
  "${CMAKE_SOURCE_DIR}/include/"
)

if(PLATFORM_NAME STREQUAL "3DS" AND DEFINED ENV{DEVKITPRO})
  target_include_directories(Minecraft.Client PRIVATE
    "$ENV{DEVKITPRO}/libctru/include"
  )
endif()
target_compile_definitions(Minecraft.Client PRIVATE
  ${MINECRAFT_SHARED_DEFINES}
)
target_precompile_headers(Minecraft.Client PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:stdafx.h>")
set_source_files_properties(compat_shims.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON) # This redefines internal MSVC CRT symbols which will cause an issue with PCH

configure_compiler_target(Minecraft.Client)

set_target_properties(Minecraft.Client PROPERTIES
  OUTPUT_NAME "Minecraft.Client"
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Minecraft.Client>"
)

target_link_libraries(Minecraft.Client PRIVATE
  Minecraft.World
)

if(PLATFORM_NAME STREQUAL "Windows64")
  target_link_libraries(Minecraft.Client PRIVATE
    d3d11
    d3dcompiler
    XInput9_1_0
    wsock32
    legacy_stdio_definitions
  )
endif()

if(PLATFORM_NAME STREQUAL "3DS")
  if(DEFINED ENV{DEVKITPRO})
    target_link_libraries(Minecraft.Client PRIVATE
      $<$<CONFIG:Debug>:$ENV{DEVKITPRO}/libctru/lib/libctrud.a>
      $<$<NOT:$<CONFIG:Debug>>:$ENV{DEVKITPRO}/libctru/lib/libctru.a>
    )
  else()
    target_link_libraries(Minecraft.Client PRIVATE
      ctru
    )
  endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs")
  target_link_libraries(Minecraft.Client PRIVATE
    $<$<CONFIG:Debug>: # Debug 4J libraries
      "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Input_d.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Storage_d.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Render_PC_d.lib"
    >
    $<$<NOT:$<CONFIG:Debug>>: # Release 4J libraries
      "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Input.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Storage.lib"
      "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/4JLibs/libs/4J_Render_PC.lib"
    >
  )
endif()

# Iggy libs
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/Iggy/lib")
  foreach(lib IN LISTS IGGY_LIBS)
    target_link_libraries(Minecraft.Client PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/Iggy/lib/${lib}")
  endforeach()
endif()

# ---
# Asset / redist copy
# ---
include("${CMAKE_SOURCE_DIR}/cmake/CopyAssets.cmake")
set(ASSET_FOLDER_PAIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/music"                  "music"
  "${CMAKE_CURRENT_SOURCE_DIR}/Common/Media"           "Common/Media"
  "${CMAKE_CURRENT_SOURCE_DIR}/Common/res"             "Common/res"
  "${CMAKE_CURRENT_SOURCE_DIR}/Common/Trial"           "Common/Trial"
  "${CMAKE_CURRENT_SOURCE_DIR}/Common/Tutorial"        "Common/Tutorial"
)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}Media")
  list(APPEND ASSET_FOLDER_PAIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}Media"  "${PLATFORM_NAME}Media"
  )
endif()
setup_asset_folder_copy(Minecraft.Client "${ASSET_FOLDER_PAIRS}")

# Copy redist files
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_NAME}/Iggy/lib/redist64")
  add_copyredist_target(Minecraft.Client)
endif()

# Make sure GameHDD exists on Windows
if(PLATFORM_NAME STREQUAL "Windows64")
  add_gamehdd_target(Minecraft.Client)
endif()
