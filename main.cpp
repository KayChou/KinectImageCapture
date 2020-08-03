#include <iostream>

#include "KinectCapture.h"

clock_t start, end;


int main(int argc, char const *argv[])
{
    libfreenect2::Freenect2 freenect2_;
    int numOfKinects_ = 1;
    if(numOfKinects_ > freenect2_.enumerateDevices()){
        std::cerr << "The number of devices does not match the specified\n";
        return -1;
    }

    int* types_;
    std::string* serials_;
    std::thread* kinectThreadTask;
    oneKinect** devices_;

    types_ = new int[numOfKinects_];
    serials_ = new std::string[numOfKinects_];
    kinectThreadTask = new std::thread[numOfKinects_];

    devices_ = new oneKinect*[numOfKinects_];

    for(int i=0; i<numOfKinects_; i++){
        serials_[i] = freenect2_.getDeviceSerialNumber(i);
        devices_[i] = new oneKinect(serials_[i], typesDefault);
        kinectThreadTask[i] = std::thread(&oneKinect::getFrameLoop, std::ref(devices_[i]));
    }

    for(int i=0; i<numOfKinects_; i++){
        kinectThreadTask[i].detach();
    }
    int a;
    std::cin >> a;
    return 0;
}
