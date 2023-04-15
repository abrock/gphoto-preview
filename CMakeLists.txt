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

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
        stereo-capture.cpp
        stereo-capture.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(stereo-capture
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(stereo-capture SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(stereo-capture
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(stereo-capture
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(stereo-capture
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
