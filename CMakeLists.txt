cmake_minimum_required(VERSION 3.5)

project(gphoto-preview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_modules/)

find_package(Gphoto2 REQUIRED)
include_directories(${GPHOTO2_INCLUDE_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(manager manager.cpp)
target_link_libraries(manager
    gphoto2pp
    ${GPHOTO2_LIBRARY}
    ${OpenCV_LIBRARIES}
    ParallelTime
    )

add_executable(gphoto-preview main.cpp)

target_link_libraries(gphoto-preview
    manager
    )

add_executable(gphoto-difference difference.cpp)
target_link_libraries(gphoto-difference
    manager
    )
