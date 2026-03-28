cmake_minimum_required(VERSION 3.10)

project(hello_cmake)

set(MY_LIB_TYPE "STATIC" CACHE STRING "Type of library to build (STATIC or SHARED)")
set_property(CACHE MY_LIB_TYPE PROPERTY STRINGS "STATIC" "SHARED")

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES 
    ${SOURCE_DIR}/10.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(my_prog ${SOURCES})


if(MY_LIB_TYPE STREQUAL "STATIC")
    target_link_libraries(my_prog ${CMAKE_CURRENT_SOURCE_DIR}/libMY_LIB.a)
else()
    target_link_libraries(my_prog ${CMAKE_CURRENT_SOURCE_DIR}/libMY_DIN_LIB.dll)
endif()