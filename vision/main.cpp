//
// Created by renan on 03/07/22.
//

#include "SpinnakerFrameGrabber.h"
#include <iostream>

typedef unsigned char UC;

int clamp(int value, int min, int max);
void yuvToRgb(int y, int u, int v, int *r, int *g, int *b);
void uyvyToRgb(UC *uyvy, UC *rgb, int width, int height);
void saveImage(int i);

SpinnakerFrameGrabber SpinnakerFrameGrabber(false, 0);
int cols = SpinnakerFrameGrabber.getImageWidth();
int rows = SpinnakerFrameGrabber.getImageHeight();

int main() {
    SpinnakerFrameGrabber.startCapturing();

    int i = 0;

    while(1) {
        char c;
        std::cin >> c;
        if (c == 'c') {
            saveImage(i);
            i++;
        }

        std::cout << i << '\n';
    }

}

int clamp(int value, int min, int max) {
    return std::max(min, std::min(max, value));
}

void yuvToRgb(int y, int u, int v, int *r, int *g, int *b) {
    y = y -  16;
    u = u - 128;
    v = v - 128;

    *r = (298 * y +   0 * u + 408 * v + 128) >> 8;
    *g = (298 * y - 100 * u - 208 * v + 128) >> 8;
    *b = (298 * y + 516 * u +   0 * v + 128) >> 8;

    *r = clamp(*r, 0, 255);
    *g = clamp(*g, 0, 255);
    *b = clamp(*b, 0, 255);

}

void uyvyToRgb(UC *uyvy, UC *rgb, int width, int height) {
    int r, g, b,
            y, u, v;

    int lim = (width * height)/2;
    for (int p = 0; p < lim; p++) {
        y = uyvy[4*p + 1];
        u = uyvy[4*p + 0];
        v = uyvy[4*p + 2];
        yuvToRgb(y, u, v, &r, &g, &b);

        rgb[6*p + 0] = (UC) r;
        rgb[6*p + 1] = (UC) g;
        rgb[6*p + 2] = (UC) b;

        y = uyvy[4*p + 3];
        yuvToRgb(y, u, v, &r, &g, &b);

        rgb[6*p + 3] = (UC) r;
        rgb[6*p + 4] = (UC) g;
        rgb[6*p + 5] = (UC) b;
    }
}

void saveImage(int i){
    unsigned char *uyvyframe = SpinnakerFrameGrabber.grabFrame();

    auto frameAux = cv::Mat(rows, cols, CV_8UC2, uyvyframe);
    cv::Mat frameReturn = cv::Mat(rows, cols, CV_8UC3);;
    uyvyToRgb(frameAux.data, frameReturn.data, frameAux.cols, frameAux.rows);
    cv::cvtColor(frameReturn, frameReturn, cv::COLOR_RGB2BGR);


    cv::imwrite("../dataset/test_" + std::to_string(i) + ".jpg", frameReturn);
}