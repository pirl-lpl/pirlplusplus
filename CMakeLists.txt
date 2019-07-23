cmake_minimum_required(VERSION 3.11)

project(PIRL++ VERSION 3.0.0 DESCRIPTION "A Legacy C++ Support Library")

set(LibSrc
        "Binary_IO.cc"
        "Cache.cc"
        "Data_Block.cc"
        "Dimensions.cc"
        "endian.cc"
        "Files.cc"
)

# this is the "object library" OBJECT ObjectLib target: compiles the sources only once
add_library(ObjectLib OBJECT ${LibSrc})

if(WIN32)
   set(CMAKE_CXX_FLAGS "/LD /EHsc /Fe${PROJECT_NAME}.dll")

   # TODO figure out why I need to set this
   link_directories("C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\")
else()
   # TODO set this properly (see below)
   set(CMAKE_CXX_FLAGS "-fPIC")
endif()

# TODO do we really need to make both static and shared
# 
set(StaticLib ${PROJECT_NAME}_static)
set(SharedLib ${PROJECT_NAME})

# shared and static libraries built from the same object files
add_library(${SharedLib} SHARED $<TARGET_OBJECTS:ObjectLib>)
add_library(${StaticLib} STATIC $<TARGET_OBJECTS:ObjectLib>)

if(WIN32)
  find_library(WS2_32_LIBRARY_PATH WS2_32)   
  find_library(USERENV_LIBRARY_PATH UserEnv) 

  target_link_libraries(${SharedLib} ${WS2_32_LIBRARY_PATH} ${USERENV_LIBRARY_PATH})
#  3.13 target_link_options(${SharedLib} "/MANIFEST:NO")
   set(CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO)
   set(CMAKE_STATIC_LINKER_FLAGS /MANIFEST:NO)
   set_target_properties(${SharedLib} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
   set_target_properties(${StaticLib} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
   set_property(TARGET ${ObjectLib} PROPERTY POSITION_INDEPENDENT_CODE ON)
   set_target_properties(${StaticLib} PROPERTIES OUTPUT_NAME ${SharedLib})
endif()

# allows dependent projects to use our headers
# by direct include from this directory
# TODO should this be a target property rather than a project global?
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(pirl-config-version.cmake VERSION ${PACKAGE_VERSION}
   COMPATIBILITY SameMajorVersion
)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(TARGETS ${PROJECT_NAME} NAMESPACE PIRL:: FILE pirl-config.cmake)
export(PACKAGE PIRL)
