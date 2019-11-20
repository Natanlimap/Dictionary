cmake_minimum_required(VERSION 3.5)
project (DAL VERSION 1.0.0 LANGUAGES CXX )

#=== FINDING PACKAGES ===#

# Locate GTest package (library)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#--------------------------------
# This is for old cmake versions
set (CMAKE_CXX_STANDARD 11)
#--------------------------------

#=== SETTING VARIABLES ===#
# Compiling flags
set( GCC_COMPILE_FLAGS "-Wall" )
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}" )

#Include dir
include_directories( include )

#=== Test target ===

add_executable(run_tests "src/test_manager.cpp"
                         "src/run_tests.cpp" )

#define C++11 as the standard.
set_property(TARGET run_tests PROPERTY CXX_STANDARD 11)
#target_compile_features(run_tests PUBLIC cxx_std_11)
