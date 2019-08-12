cmake_minimum_required(VERSION 3.11)

project(PIRL++ VERSION 3.0.0 DESCRIPTION "A Legacy C++ Support Library")

add_library(obj_lib OBJECT
        "Binary_IO.cc"
        "Cache.cc"
        "Data_Block.cc"
        "Dimensions.cc"
        "endian.cc"
        "Files.cc")

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

target_include_directories(${shared_lib} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file("PIRL-config-version.cmake" VERSION ${PACKAGE_VERSION}
   COMPATIBILITY SameMajorVersion
)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(TARGETS ${shared_lib} ${static_lib} NAMESPACE PIRL:: FILE "PIRL-config.cmake")
export(PACKAGE PIRL)

include(GNUInstallDirs)
install(TARGETS ${shared_lib} ${static_lib}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCDIR}
)