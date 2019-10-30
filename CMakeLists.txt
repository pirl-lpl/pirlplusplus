cmake_minimum_required(VERSION 3.12)

project(PIRL++ VERSION 3.0.0 DESCRIPTION "A Legacy C++ Support Library")

add_library(obj_lib OBJECT
        "Binary_IO.cc"
        "Cache.cc"
        "Data_Block.cc"
        "Dimensions.cc"
        "endian.cc"
        "Files.cc")

set(headers
        "Binary_IO.hh"
        "Cache.hh"
        "Data_Block.hh"
        "Dimensions.hh"
        "endian.hh"
        "Files.hh")

set_target_properties(obj_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(WIN32)
	set(static_lib "lib${PROJECT_NAME}")
	target_compile_options(obj_lib PRIVATE "/W4") # /WX for warnings as errors
else()
    target_compile_options(obj_lib PRIVATE "-Wall")
    set(static_lib ${PROJECT_NAME})
endif()

set(shared_lib "${PROJECT_NAME}")
add_library(${shared_lib} SHARED $<TARGET_OBJECTS:obj_lib>)
add_library(${static_lib} STATIC $<TARGET_OBJECTS:obj_lib>)

target_include_directories(${shared_lib} PUBLIC $<INSTALL_INTERFACE:include>)

set_target_properties(${shared_lib} PROPERTIES PUBLIC_HEADER "${headers}")

if(WIN32)
  find_library(WS2_32_LIBRARY_PATH WS2_32)
  find_library(USERENV_LIBRARY_PATH UserEnv)

  target_link_libraries(${shared_lib} ${WS2_32_LIBRARY_PATH} ${USERENV_LIBRARY_PATH})
  set_target_properties(${shared_lib} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
	if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
		target_link_options(${shared_lib} PRIVATE "/MANIFEST:NO")
	else()
		set(CMAKE_SHARED_LINKER_FLAGS "/MANIFEST:NO")
	endif()

	add_custom_command(TARGET ${shared_lib} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${static_lib}.lib" "${shared_lib}.lib")

endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file("PIRL-config-version.cmake" VERSION ${PACKAGE_VERSION}
   COMPATIBILITY SameMajorVersion
)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(TARGETS ${shared_lib} ${static_lib} NAMESPACE PIRL:: FILE "PIRL-config.cmake")
export(PACKAGE PIRL)

#
# TODO install version file, config file and exported targets file
#
include(GNUInstallDirs)
install(TARGETS ${shared_lib} ${static_lib} EXPORT PIRL-config
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCDIR}
)

install(EXPORT PIRL-config NAMESPACE PIRL:: DESTINATION ${CMAKE_INSTALL_DATADIR}/PIRL/cmake)
