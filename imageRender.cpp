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

        end = clock();
        fps = CLOCKS_PER_SEC/(double)(end - start);
        cv::putText(color, "FPS: " + std::to_string(fps), cv::Point(0, 100), 2, 2, cv::Scalar(0, 255, 0), 4);
        cv::putText(depth, "FPS: " + std::to_string(fps), cv::Point(0, 30), 2, 1, cv::Scalar(0, 255, 0));
        cv::imshow("Color" + str, color);
        cv::imshow("Depth" + str, depth);
        start = clock();
        cv::waitKey(1);
#if 0
        bool save2local = true;
        if(save2local){
            std::vector<Point3f> vertices;
            std::vector<RGB> colors;
            Point3f p;
            RGB tempColor;
            for(int i=0; i<512*424; i++){
                p.X =  - packet->vertices[i].X;
                p.Y = packet->vertices[i].Y;
                p.Z = packet->vertices[i].Z;
                if(p.Z > 0 && p.Z < 4.5){
                    vertices.push_back(p);
                    tempColor.B = packet->vertices[i].B;
                    tempColor.G = packet->vertices[i].G;
                    tempColor.R = packet->vertices[i].R;
                    colors.push_back(tempColor);
                }
            }
            savePlyFile("pointCloud.ply", vertices, true, colors);
            save2local = false;
        }
#endif
        packet->destroy();
    }
    cv::destroyAllWindows();
    std::cout << "Image render " << str << " stopped.\n";
}


void ImageRenderer::destory(){
}