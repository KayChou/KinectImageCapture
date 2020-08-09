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

            cv::Mat point(packet->height_d, packet->width_d, CV_32FC3);
            cv::Mat RGB(packet->height_d, packet->width_d, CV_8UC3);

            for(int i=0; i<packet->height_d; i++){
                for(int j=0; j<packet->width_d; j++){
                    //std::printf("%d %d\n", i, j);
                    point.at<cv::Vec3f>(i, j)[0] = packet->vertices[i*packet->width_d + j].X;
                    point.at<cv::Vec3f>(i, j)[1] = packet->vertices[i*packet->width_d + j].Y;
                    point.at<cv::Vec3f>(i, j)[2] = packet->vertices[i*packet->width_d + j].Z;
                    // std::printf("i: %d , j: %d | %f %f %f\n", i, j, point.at<cv::Vec3f>(i, j)[0],
                    //  point.at<cv::Vec3f>(i, j)[1], point.at<cv::Vec3f>(i, j)[2]);
                    RGB.at<cv::Vec3b>(i, j)[0] = packet->vertices[i*packet->width_d + j].B;
                    RGB.at<cv::Vec3b>(i, j)[1] = packet->vertices[i*packet->width_d + j].G;
                    RGB.at<cv::Vec3b>(i, j)[2] = packet->vertices[i*packet->width_d + j].R;
                }
                
            }

            cv::Mat point_save;
            point.convertTo(point_save, CV_32F);
            //cv::normalize(point_save, point_save, 0, 1.0, cv::NORM_MINMAX);

            cv::imwrite("./datas/color" + std::to_string(i) + std::to_string(cnt[i]) + ".png", color);
            cv::imwrite("./datas/depth" + std::to_string(i) + std::to_string(cnt[i]) + ".png", depth);
            cv::imwrite("./datas/point" + std::to_string(i) + std::to_string(cnt[i]) + ".tif", point_save);
            //cv::ImwriteFlags;
            cv::imwrite("./datas/rgb" + std::to_string(i) + std::to_string(cnt[i]) + ".png", RGB);

            packet->destroy();
        }
    }
}