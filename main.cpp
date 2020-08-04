#include <iostream>
#include "KinectCapture.h"
#include "imageRender.h"
#include "utils.h"

#define numOfKinects 2

int main(int argc, char const *argv[])
{
    // define FIFO for RGBD storage
    FIFO **RGBD_Acquisition = new FIFO*[numOfKinects];
    for(int i=0; i<numOfKinects; i++){
        RGBD_Acquisition[i] = new FIFO();
        RGBD_Acquisition[i]->init(FIFO_LEN);
    }
    
    openAllKinect(numOfKinects, RGBD_Acquisition);
    //saveRGBDFIFO2Image(RGBD_Acquisition, numOfKinects);
    startAllImageRender(numOfKinects, RGBD_Acquisition);

    getchar(); // block main thread
    
    destoryAllKinect(numOfKinects);

    // delete FIFO ptr
    for(int i=0; i<numOfKinects; i++){
        delete RGBD_Acquisition[i];
    }
    delete RGBD_Acquisition;
    return 0;
}
