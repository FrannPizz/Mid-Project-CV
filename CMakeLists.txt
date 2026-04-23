cmake_minimum_required(VERSION 3.10)
project(DynamicObjectDetector)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "C:/msys64/ucrt64/lib/cmake/opencv4")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main
    src/main.cpp
    src/preProc.cpp
    src/OpticalFlowFilter.cpp
)

target_link_libraries(main ${OpenCV_LIBS})