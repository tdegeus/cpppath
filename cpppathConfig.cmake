# cpppath cmake module
#
# This module sets the target:
#
#   cpppath
#
# In addition, it sets the following variables:
#
#   cpppath_FOUND - true if cpppath found
#   cpppath_VERSION - cpppath's version
#   cpppath_INCLUDE_DIRS - the directory containing cpppath headers

include(CMakeFindDependencyMacro)
find_dependency(xtensor)

if(NOT TARGET cpppath)
    include("${CMAKE_CURRENT_LIST_DIR}/cpppathTargets.cmake")
    get_target_property(cpppath_INCLUDE_DIRS cpppath INTERFACE_INCLUDE_DIRECTORIES)
endif()
