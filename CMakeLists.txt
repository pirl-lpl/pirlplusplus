cmake_minimum_required(VERSION 3.11)

project(PIRL++ VERSION 3.0.0 DESCRIPTION "A Legacy C++ Support Library")

add_library(${PROJECT_NAME} SHARED
        "Binary_IO.cc"
        "Cache.cc"
        "Data_Block.cc"
        "Dimensions.cc"
        "endian.cc"
        "Files.cc")

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

if(MSVC)
   add_compile_options(/W4) # /WX for warnings as errors
else()
   add_compile_options(-Wall)
endif()

if(WIN32)
  find_library(WS2_32_LIBRARY_PATH WS2_32)
  find_library(USERENV_LIBRARY_PATH UserEnv)

  target_link_libraries(${PROJECT_NAME} ${WS2_32_LIBRARY_PATH} ${USERENV_LIBRARY_PATH})

   # TODO cmake >= 3.13 target_link_options(${PROJECT_NAME} "/MANIFEST:NO")
   set(CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO)

   set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# allows dependent projects to use our headers
# by direct include from this directory
# TODO should this be a target property rather than a project global?
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(PIRL-config-version.cmake VERSION ${PACKAGE_VERSION}
   COMPATIBILITY SameMajorVersion
)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(TARGETS ${PROJECT_NAME} NAMESPACE PIRL:: FILE PIRL-config.cmake)
export(PACKAGE PIRL)