#include "KinectCapture.h"

oneKinect::oneKinect(std::string serial, int types){
    init(serial, types);
}


oneKinect::~oneKinect(){
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
    return true;
}


bool oneKinect::getFrameLoop(){
    std::cout << "New thread started" << std::endl;
    
    int framecount = 0;
    

    clock_t start, end;
    // get image from kinect v2
    cv::Mat color, depth, undistortion, registration;
    cv::namedWindow(serial_+"Color", cv::WINDOW_NORMAL);
    cv::namedWindow(serial_+"Depth", cv::WINDOW_NORMAL);
    cv::namedWindow(serial_+"undistorted", cv::WINDOW_NORMAL);
    cv::namedWindow(serial_+"registered", cv::WINDOW_NORMAL);

    bool saveFile2Local = true;

    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);

    while(framecount < framemax){
        start = clock();
        if (!listener_->waitForNewFrame(frames_, 10*1000)) { // 10 seconds
            std::cout << "timeout!" << std::endl;
            return -1;
        }

        color_ = frames_[libfreenect2::Frame::Color];
        depth_ = frames_[libfreenect2::Frame::Depth];
        registration_->apply(color_, depth_, &undistorted, &registered, true, &depth2rgb);

        // framePacket packet(color_, depth_);

        // cv::Mat(packet.height_c, packet.width_c, CV_8UC4, packet.data_c).copyTo(color);
        // cv::Mat(packet.height_d, packet.width_d, CV_8UC4, packet.data_d).copyTo(depth);

        cv::Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(undistortion);
        cv::Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(registration);
        cv::Mat(color_->height, color_->width, CV_8UC4, color_->data).copyTo(color);
        cv::Mat(depth_->height, depth_->width, CV_32FC1, depth_->data).copyTo(depth);

        cv::resizeWindow(serial_+"Color", 512, 424);
        cv::resizeWindow(serial_+"Depth", 512, 424);
        cv::resizeWindow(serial_+"undistorted", 512, 424);
        cv::resizeWindow(serial_+"registered", 512, 424);
        cv::imshow(serial_+"Color", color);
        cv::imshow(serial_+"Depth", depth);
        cv::imshow(serial_+"undistorted", undistortion);
        cv::imshow(serial_+"registered", registration);
        cv::waitKey(1);
        // packet.destroy();

        if(saveFile2Local){
            float rgb;
            Point3f p;
            RGB tempColor;
            std::vector<Point3f> vertices;
            std::vector<RGB> colors;
            for(int i=0; i<512*424; i++){
                
                //registration_->getPointXYZ(&undistorted, i/512, i%512, x, y, z);
                registration_->getPointXYZRGB(&undistorted, &registered, i/512, i%512, p.X, p.Y, p.Z, rgb);

                if(p.Z > 0 && p.Z < 4.5){
                    vertices.push_back(p);
                    const uint8_t *c = reinterpret_cast<uint8_t*>(&rgb);
                    tempColor.B = c[0];
                    tempColor.G = c[1];
                    tempColor.R = c[2];
                    colors.push_back(tempColor);
                }
                
            }
            savePlyFile("pointCloud.ply", vertices, true, colors);
            saveFile2Local = false;

            cv::imwrite("color.jpg", color);
            cv::imwrite("depth.jpg", depth);
            cv::imwrite("undistortion.jpg", undistortion);
            cv::imwrite("registration.jpg", registration);
            
        }   

        listener_->release(frames_);
        end = clock();
        std::printf("Frame count %d | fps: %f\n", framecount, CLOCKS_PER_SEC/(double)(end - start));

        framecount++;
    }

    cv::destroyWindow(serial_+"Depth");
    cv::destroyWindow(serial_+"Color");
    cv::destroyWindow(serial_+"undistorted");
    cv::destroyWindow(serial_+"registered");
    dev_->stop();
    dev_->close();
    return true;
}
