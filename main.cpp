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

clock_t start, end;


int main(int argc, char const *argv[])
{
    // Initialize and Discover Devices
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev = 0;
    libfreenect2::PacketPipeline *pipeline = 0;

    if(freenect2.enumerateDevices() == 0){
        std::cout << "No device connected!\n";
        return -1;
    }

    pipeline = new libfreenect2::CpuPacketPipeline();

    // Open and Configure the Device
    std::string serial = freenect2.getDefaultDeviceSerialNumber();
	std::cout << "Kinect v2 connect successed, device num is： " << serial << std::endl;

    dev = freenect2.openDevice(serial, pipeline);
    std::cout << "Open Device " << serial << std::endl;
    int types = 0;
    //types |= libfreenect2::Frame::Color;
    types |= libfreenect2::Frame::Depth;
    //types |= libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;

    libfreenect2::SyncMultiFrameListener listener(types);
    libfreenect2::FrameMap frames;
    dev->setColorFrameListener(&listener);
    dev->setIrAndDepthFrameListener(&listener);

    if(!dev->start()){ return -1; }
    else{ std::cout << "Device started successfully\n"; }

    // if (!dev->startStreams(false, true)){ std::cerr << "Stream start failed" << std::endl; return -1; }
    // else{ std::cout << "Streams started successfully\n"; }

    std::cout << "Device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "Device firmware: " << dev->getFirmwareVersion() << std::endl;

    dev->setColorManualExposure(10, 2);

    libfreenect2::Freenect2Device::Config config;
    config.EnableBilateralFilter = true;
    config.EnableEdgeAwareFilter = true;
    config.MinDepth = 0.3f;
    config.MaxDepth = 12.0f;
    dev->setConfiguration(config);

    libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);

    int framecount = 0;

    // get image from kinect v2
    cv::Mat color, depthImg, Ir;
    cv::namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
    while(framecount < framemax){

        start = clock();
        if (!listener.waitForNewFrame(frames, 10*1000)) { // 10 seconds
            std::cout << "timeout!" << std::endl;
            return -1;
        }
        //libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        // libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
        // //registration->apply(rgb, depth, &undistorted, &registered);

        // cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(color);
        // // cv::Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(Ir);
        cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthImg);

        // // cv::imwrite("color.png", color);
        // // cv::imwrite("Ir.png", Ir);
        // // cv::imwrite("depth.png",depthImg);
        cv::resizeWindow("MyWindow", 512, 424);
        imshow("MyWindow", depthImg);
        cv::waitKey(1);

        listener.release(frames);
        end = clock();

        std::printf("Frame count %d | Duration: %f ms | fps: %f\n", framecount, (double)(end - start)*1000/CLOCKS_PER_SEC, CLOCKS_PER_SEC/(double)(end - start));

        framecount++;
    }

    cv::destroyWindow("MyWindow");

    dev->stop();
    dev->close();
    return 0;
}
