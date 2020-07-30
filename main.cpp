#include <iostream>

#include "KinectCapture.h"

clock_t start, end;


int main(int argc, char const *argv[])
{
    libfreenect2::Freenect2 freenect2;
    if(freenect2.enumerateDevices() == 0){
        std::cout << "No device connected!\n";
        return -1;
    }

    // Open and Configure the Device
    std::string serial = freenect2.getDefaultDeviceSerialNumber();
	std::cout << "Kinect v2 connect successed, device num is： " << serial << std::endl;

    int types = 0;
    types |= libfreenect2::Frame::Color;
    types |= libfreenect2::Frame::Depth;
    //types |= libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
    oneKinect device(serial, types);
    device.getFrameLoop();
    
    return 0;
}
