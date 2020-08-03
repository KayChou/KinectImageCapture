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
#include <thread>
#include "FramePacket.h"
#include "PlyIO.h"

#define framemax 10000
#define typesDefault libfreenect2::Frame::Color | libfreenect2::Frame::Depth


class oneKinect
{
public:
    oneKinect();
    oneKinect(std::string serial, int types=typesDefault);
    ~oneKinect();

    bool init(std::string serial, int types=typesDefault);
    bool getFrameLoop();

public:
    libfreenect2::Frame *color_;
    libfreenect2::Frame *depth_;

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