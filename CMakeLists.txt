cmake_minimum_required(VERSION 3.13)

project(PIRL VERSION 3.0.0 DESCRIPTION "A Legacy C++ Support Library")

add_library(obj_lib OBJECT
        "Binary_IO.cc"
        "Cache.cc"
        "Data_Block.cc"
        "Dimensions.cc"
        "endian.cc"
        "Files.cc"
)

set(headers
        "Binary_IO.hh"
        "Cache.hh"
        "Data_Block.hh"
        "Dimensions.hh"
        "endian.hh"
        "Files.hh"
        "Reference_Counted_Pointer.hh"
)

set_target_properties(obj_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(WIN32)
    target_compile_options(obj_lib PRIVATE "/W4") # /WX for warnings as errors
else()
    target_compile_options(obj_lib PRIVATE "-Wall")
endif()

set(shared_lib "${PROJECT_NAME}++")
set(static_lib "${PROJECT_NAME}++_static")

add_library(${shared_lib} SHARED $<TARGET_OBJECTS:obj_lib>)
add_library(${static_lib} STATIC $<TARGET_OBJECTS:obj_lib>)

set_target_properties(${shared_lib} PROPERTIES PUBLIC_HEADER "${headers}")
set_target_properties(${shared_lib} PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(${shared_lib} PUBLIC $<INSTALL_INTERFACE:include>)
target_include_directories(${static_lib} PUBLIC $<INSTALL_INTERFACE:include>)

if(WIN32)
    find_library(WS2_32_LIBRARY_PATH WS2_32)
    find_library(USERENV_LIBRARY_PATH UserEnv)

    target_link_libraries(${shared_lib} ${WS2_32_LIBRARY_PATH} ${USERENV_LIBRARY_PATH})
    set_target_properties(${shared_lib} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_link_options(${shared_lib} PRIVATE "/MANIFEST:NO")

    #  add_custom_command(TARGET ${shared_lib} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${static_lib}.lib" "${shared_lib}.lib")
endif()

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(TARGETS ${shared_lib} ${static_lib} NAMESPACE ${PROJECT_NAME}:: FILE "${PROJECT_NAME}-exports.cmake")
export(PACKAGE ${PROJECT_NAME})

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

write_basic_package_version_file("pirl-config-version.cmake" VERSION ${PROJECT_VERSION}
   COMPATIBILITY SameMajorVersion)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/pirl-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake")

install(TARGETS ${shared_lib} ${static_lib} EXPORT ${PROJECT_NAME}-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT ${PROJECT_NAME}-export FILE "${PROJECT_NAME}-exports.cmake"
        NAMESPACE ${PROJECT_NAME}:: DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake")

# install config files
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/pirl-config-version.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/pirl-config.cmake"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake")
