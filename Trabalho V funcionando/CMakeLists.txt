cmake_minimum_required(VERSION 2.8)
project( video )

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -ggdb -O3 -Wall -fpermissive")
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable( detect main.cpp )
target_link_libraries( detect ${OpenCV_LIBS} )
