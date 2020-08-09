#ifndef IMAGERENDER_H
#define IMAGERENDER_H

#include "common.h"

bool startAllImageRender(int numOfKinects, FIFO** input);
bool destoryAllImageRender(int numOfKinects);


class ImageRenderer{
public:
    ImageRenderer(FIFO* input);
    ~ImageRenderer();

    void Draw(std::string str);
    void destory();

public:
    cv::Mat color;
    cv::Mat depth;
    FIFO* input_;
    clock_t start=clock(), end=clock();
    double fps;
};

#endif