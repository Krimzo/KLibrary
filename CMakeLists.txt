cmake_minimum_required(VERSION 4.4.0)
project(KLibrary LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE klibrary_sources CONFIGURE_DEPENDS source/*.cpp)
add_library(klibrarylib ${klibrary_sources})
target_compile_definitions(klibrarylib PUBLIC _UNICODE UNICODE)
target_include_directories(klibrarylib PUBLIC source)

file(GLOB_RECURSE klibrary_examples_sources CONFIGURE_DEPENDS examples/source/*.cpp)
add_executable(klibrary_examples ${klibrary_examples_sources})
target_link_libraries(klibrary_examples PUBLIC klibrarylib)
target_include_directories(klibrary_examples PUBLIC examples/source)
set_target_properties(klibrary_examples PROPERTIES
    DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/examples)
