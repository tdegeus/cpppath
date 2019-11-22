
cmake_minimum_required(VERSION 3.0)

# Options
# =======

project(cpppath)

option(BUILD_TESTS "${PROJECT_NAME} Build tests" OFF)

# Internals
# =========

set(CPPPATH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Version
# =======

file(
    STRINGS "${CPPPATH_INCLUDE_DIR}/cpppath.h" cpppath_version_defines
    REGEX "#define CPPPATH_VERSION_(MAJOR|MINOR|PATCH)")

foreach(ver ${cpppath_version_defines})
    if(ver MATCHES "#define CPPPATH_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(CPPPATH_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()

set(CPPPATH_VERSION
    ${CPPPATH_VERSION_MAJOR}.${CPPPATH_VERSION_MINOR}.${CPPPATH_VERSION_PATCH})

message(STATUS "Building cpppath v${CPPPATH_VERSION}")

# Build
# =====

set(CPPPATH_HEADERS
    ${CPPPATH_INCLUDE_DIR}/cpppath.h)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CPPPATH_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>)

# Installation
# ============

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets)

export(
    EXPORT ${PROJECT_NAME}-targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

install(
    FILES ${CPPPATH_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPPPATH_CMAKECONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE
    STRING "install path for ${PROJECT_NAME}Config.cmake")

configure_package_config_file(
    "cpppathConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CPPPATH_CMAKECONFIG_INSTALL_DIR})

set(_CPPPATH_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})

unset(CMAKE_SIZEOF_VOID_P)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${CPPPATH_VERSION}
    COMPATIBILITY AnyNewerVersion)

set(CMAKE_SIZEOF_VOID_P ${_CPPPATH_CMAKE_SIZEOF_VOID_P})

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CPPPATH_CMAKECONFIG_INSTALL_DIR})

install(
    EXPORT ${PROJECT_NAME}-targets
    FILE "${PROJECT_NAME}Targets.cmake"
    DESTINATION ${CPPPATH_CMAKECONFIG_INSTALL_DIR})

configure_file(
    "cpppath.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
     @ONLY)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

# Add builds
# ==========

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
