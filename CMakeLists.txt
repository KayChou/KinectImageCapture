cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project("Kinect_Color_IR_Depth")

set(CMAKE_CXX_FLAGS "-std=c++11")

# Set cmake prefix path to enable cmake to find freenect2
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{HOME}/lib/freenect2/lib/cmake/freenect2)
find_package(freenect2 REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED HINTS "$ENV{HOME}/lib/opencv3_4_11/share/OpenCV")

include_directories("/usr/include/libusb-1.0/")

INCLUDE_DIRECTORIES(
  ${freenect2_INCLUDE_DIR}
)

add_executable(main ./main.cpp)
target_link_libraries(main ${freenect2_LIBRARIES} ${OpenCV_LIBS})