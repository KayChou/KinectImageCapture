#ifndef KINECT_MANAGE_H
#define KINECT_MANAGE_H

#include <iostream>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/config.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sys/time.h>

#define framemax 360

class oneKinect
{
public:
    oneKinect(std::string serial, int types);
    ~oneKinect();

    bool init(std::string serial, int types);
    bool getFrameLoop();

public:
    libfreenect2::Frame *color_;
    libfreenect2::Frame *depth_;
    libfreenect2::Frame *undistorted_;
    libfreenect2::Frame *registered_;

public:
    libfreenect2::Freenect2 freenect2_;
    libfreenect2::Freenect2Device *dev_;
    libfreenect2::PacketPipeline *pipeline_;

    std::string serial_;
    int types_;
    libfreenect2::SyncMultiFrameListener *listener_;
    libfreenect2::FrameMap frames_;

    libfreenect2::Freenect2Device::Config config_;
    libfreenect2::Registration* registration_;
};

#endif