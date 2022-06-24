cmake_minimum_required(VERSION 3.18..3.21)

project(cpppath)

string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)

# Command-line options
# ====================

option(BUILD_ALL "${PROJECT_NAME}: Build tests" OFF)
option(BUILD_TESTS "${PROJECT_NAME}: Build tests" OFF)

# Read version
# ============

if (DEFINED ENV{SETUPTOOLS_SCM_PRETEND_VERSION})
    set(PROJECT_VERSION $ENV{SETUPTOOLS_SCM_PRETEND_VERSION})
    message(STATUS "Building ${PROJECT_NAME} ${PROJECT_VERSION} (read from SETUPTOOLS_SCM_PRETEND_VERSION)")
else()
    execute_process(
        COMMAND python -c "from setuptools_scm import get_version; print(get_version())"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PROJECT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    message(STATUS "Building ${PROJECT_NAME} ${PROJECT_VERSION}")
endif()

# Set target
# ==========

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_definitions(${PROJECT_NAME} INTERFACE
    ${PROJECT_NAME_UPPER}_VERSION="${PROJECT_VERSION}")

# Libraries
# =========

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CTest)
include("${PROJECT_NAME}Config.cmake")

# Installation headers / CMake / pkg-config
# =========================================

if(NOT SKBUILD)

    configure_file("include/${PROJECT_NAME}.h"
                   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.h"
                    @ONLY)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.h"
            DESTINATION "include/")

    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)

    install(
        EXPORT ${PROJECT_NAME}-targets
        FILE "${PROJECT_NAME}Targets.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    set(${PROJECT_NAME}_TMP ${CMAKE_SIZEOF_VOID_P})
    unset(CMAKE_SIZEOF_VOID_P)

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY AnyNewerVersion)

    set(CMAKE_SIZEOF_VOID_P ${${PROJECT_NAME}_TMP})

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" @ONLY)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

endif()

# Build tests
# ===========

if(BUILD_TESTS OR BUILD_ALL)

    enable_testing()
    add_subdirectory(tests)

endif()
