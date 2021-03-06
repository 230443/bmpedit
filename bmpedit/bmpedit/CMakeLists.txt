cmake_minimum_required(VERSION 3.17)
project(bmpedit)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)


ADD_LIBRARY(LibsModule
        src/Bitmap.cpp
        include/Bitmap.h
        src/main.cpp
        include/CImg.h
        )

target_link_libraries(LibsModule -L/usr/X11R6/lib -lm -lpthread -lX11)

add_executable(bmpedit
        src/Bitmap.cpp
        include/Bitmap.h
        src/main.cpp
        include/CImg.h src/Task2.cpp src/Task3.cpp src/Task4.cpp include/Task4.h)

target_link_libraries(bmpedit LibsModule)

enable_testing()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(SOURCE_TEST_FILES
        Test/config.cpp
        Test/BitmapTest.cpp Test/Task3Test.cpp Test/Task3TimeTest.cpp Test/Task4Test.cpp)

include_directories(
        ${Boost_INCLUDE_DIRS}
)

add_executable(Test ${SOURCE_TEST_FILES}
        include/Bitmap.h
        src/Bitmap.cpp)

target_link_libraries(Test
        LibsModule
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

add_custom_target(check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 BOOST_TEST_LOG_LEVEL=all
        ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --verbose
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

enable_testing()
add_test(LibraryTest Test)