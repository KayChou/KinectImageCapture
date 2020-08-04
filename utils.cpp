#include "utils.h"

bool saveRGBDFIFO2Image(FIFO **input, int numOfKinects){
    cv::Mat color, depth;


    int *cnt = new int[numOfKinects];
    for(int i=0; i<numOfKinects; i++){
        cnt[i] = 0;
    }

    for(int i=0; i<numOfKinects; i++){
        while(cnt[i]++ < framemax){
            framePacket *packet = input[i]->get();
            if( packet == NULL ) {
                break;
            }
            cv::Mat(packet->height_c, packet->width_c, CV_8UC4, packet->data_c).copyTo(color);
            cv::Mat(packet->height_d, packet->width_d, CV_32FC1, packet->data_d).copyTo(depth);

            cv::imwrite("./datas/color" + std::to_string(i) + std::to_string(cnt[i]) + ".png", color);
            cv::imwrite("./datas/depth" + std::to_string(i) + std::to_string(cnt[i]) + ".png", depth);

            packet->destroy();
        }
    }
}