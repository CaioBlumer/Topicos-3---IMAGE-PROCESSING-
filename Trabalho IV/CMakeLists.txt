cmake_minimum_required(VERSION 2.8)
project( flood )

set(CMAKE_CXX_FLAGS "-g -ggdb -Wall -fpermissive")
#set(CMAKE_CXX_FLAGS "-O3")


find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable(flood main.cpp)
target_link_libraries( flood ${OpenCV_LIBS} )
target_link_libraries( flood lb_flood )

add_library(lb_flood flood.cpp)
