cmake_minimum_required(VERSION 3.0)

# Project name
project(libGPCGUIRenderer)

# Version (semantic: major, minor, patch-level)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

add_subdirectory(lib)

add_subdirectory(testsuite)

# Export the targets via the build tree
export(TARGETS libGPCGUIRenderer FILE "libGPCGUIRenderer-targets.cmake")
configure_file(project-config.cmake.in "${PROJECT_BINARY_DIR}/libGPCGUIRenderer-config.cmake" @ONLY)
export(PACKAGE libGPCGUIRenderer)