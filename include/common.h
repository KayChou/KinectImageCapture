#pragma once

#include <string>
#include <sys/time.h>
#include <thread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FIFO.h"
#include "FramePacket.h"
#include "PlyIO.h"

#define framemax 500
#define FIFO_LEN 30
#define typesDefault libfreenect2::Frame::Color | libfreenect2::Frame::Depth

enum systemStatus{
    SYSTEM_RUNNING,
    SYSTEM_PAUSE,
    SYSTEM_FINISH
};