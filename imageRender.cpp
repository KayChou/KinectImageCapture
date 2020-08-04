#include "imageRender.h"

std::thread *ImageRenderThreadTask;
ImageRenderer **imagerender;

//=======================================================================================
// start Image render
// get data from FIFO and render using cv
//=======================================================================================
bool startAllImageRender(int numOfKinects, FIFO** input){
    ImageRenderThreadTask = new std::thread[numOfKinects];
    imagerender = new ImageRenderer*[numOfKinects];
    for(int i=0; i<numOfKinects; i++){
        imagerender[i] = new ImageRenderer(input[i]);
        cv::namedWindow("Color" + std::to_string(i), cv::WINDOW_NORMAL);
        cv::namedWindow("Depth" + std::to_string(i), cv::WINDOW_NORMAL);
        cv::resizeWindow("Color" + std::to_string(i), 512, 424);
        cv::resizeWindow("Depth" + std::to_string(i), 512, 424);
        ImageRenderThreadTask[i] = std::thread(&ImageRenderer::Draw, std::ref(imagerender[i]), std::to_string(i));
    }

    for(int i=0; i<numOfKinects; i++){
        ImageRenderThreadTask[i].detach();
    }
    return true;
}


//=======================================================================================
// destory image render thread and delete ptr
//=======================================================================================
bool destoryAllImageRender(int numOfKinects){
    for(int i=0; i<numOfKinects; i++){
        delete imagerender[i];
    }
    delete imagerender;
    return true;
}


ImageRenderer::ImageRenderer(FIFO* input){
    this->input_ = input;
}

ImageRenderer::~ImageRenderer(){
}


//=======================================================================================
// image render main loop
//=======================================================================================
void ImageRenderer::Draw(std::string str){
    while(true){
        framePacket *packet = input_->get();
        if( packet == NULL ) {
            break;
        }
        cv::Mat(packet->height_c, packet->width_c, CV_8UC4, packet->data_c).copyTo(color);
        cv::Mat(packet->height_d, packet->width_d, CV_8UC4, packet->data_d).copyTo(depth);

        cv::imshow("Color" + str, color);
        cv::imshow("Depth" + str, depth);
        cv::waitKey(1);
        packet->destroy();
    }
    cv::destroyAllWindows();
    std::cout << "Image render " << str << " stopped.\n";
}


void ImageRenderer::destory(){
}