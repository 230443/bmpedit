cmake_minimum_required(VERSION 3.17)
project(bmpedit)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)


ADD_LIBRARY(LibsModule
        Bitmap.cpp
        Bitmap.h
        bmpedit.cpp
        CImg.h
        )

target_link_libraries(LibsModule -L/usr/X11R6/lib -lm -lpthread -lX11)

add_executable(bmpedit
        Bitmap.cpp
        Bitmap.h
        bmpedit.cpp
        CImg.h)

target_link_libraries(bmpedit LibsModule)