cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project("Kinect_Color_IR_Depth")

set(CMAKE_CXX_FLAGS "-std=c++11")

# Set cmake prefix path to enable cmake to find freenect2
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{HOME}/lib/freenect2/lib/cmake/freenect2)
find_package(freenect2 REQUIRED)
INCLUDE_DIRECTORIES( ${freenect2_INCLUDE_DIR} )
FIND_PACKAGE(OpenCV REQUIRED HINTS "$ENV{HOME}/lib/opencv3_4_11/share/OpenCV")

INCLUDE_DIRECTORIES("/usr/include/libusb-1.0/")
INCLUDE_DIRECTORIES("include")


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(main main.cpp KinectCapture.cpp FramePacket.cpp PlyIO.cpp)
target_link_libraries(main ${freenect2_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(main Threads::Threads)