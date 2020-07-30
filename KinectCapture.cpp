#include "KinectCapture.h"

oneKinect::oneKinect(std::string serial, int types){
    init(serial, types);
}


oneKinect::~oneKinect(){
    dev_->stop();
    dev_->close();
    // delete pipeline_;
    // delete listener_;
    // delete registration_;
}


bool oneKinect::init(std::string serial, int types){
    serial_ = serial;
    types_ = types;

    pipeline_ = new libfreenect2::OpenGLPacketPipeline();
    dev_ = freenect2_.openDevice(serial_, pipeline_);
    listener_ = new libfreenect2::SyncMultiFrameListener(types_);

    dev_->setColorFrameListener(listener_);
    dev_->setIrAndDepthFrameListener(listener_);

    if(!dev_->start()){ return false; }

    config_.EnableBilateralFilter = true;
    config_.EnableEdgeAwareFilter = true;
    config_.MinDepth = 0.3f;
    config_.MaxDepth = 12.0f;
    dev_->setConfiguration(config_);

    registration_ = new libfreenect2::Registration(dev_->getIrCameraParams(), dev_->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);
    return true;
}


bool oneKinect::getFrameLoop(){
    int framecount = 0;

    clock_t start, end;

    // get image from kinect v2
    cv::Mat color, depth;
    cv::namedWindow("Color", cv::WINDOW_NORMAL);
    cv::namedWindow("Depth", cv::WINDOW_NORMAL);
   

    while(framecount < framemax){
        start = clock();
        if (!listener_->waitForNewFrame(frames_, 10*1000)) { // 10 seconds
            std::cout << "timeout!" << std::endl;
            return -1;
        }

        color_ = frames_[libfreenect2::Frame::Color];
        depth_ = frames_[libfreenect2::Frame::Depth];

        cv::Mat(color_->height, color_->width, CV_8UC4, color_->data).copyTo(color);
        cv::Mat(depth_->height, depth_->width, CV_8UC4, depth_->data).copyTo(depth);

        cv::resizeWindow("Color", 512, 424);
        cv::resizeWindow("Depth", 512, 424);
        cv::imshow("Color", color);
        cv::imshow("Depth", depth);
        cv::waitKey(1);

        listener_->release(frames_);
        end = clock();
        std::printf("Frame count %d | Duration: %f ms | fps: %f\n", framecount, (double)(end - start)*1000/CLOCKS_PER_SEC, CLOCKS_PER_SEC/(double)(end - start));

        framecount++;
    }

    cv::destroyWindow("Depth");
    cv::destroyWindow("Color");
}