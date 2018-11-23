#include "imageProcessor.h"

imageProcessor::imageProcessor(int br, int ct, int gm): QObject() {
    brightness = (double) br / 100 - 0.5; 
    contrast = tan((double) ct / 200 * PI);
    gamma = tan((double) gm / 200 * PI);
    lut.create(1, DEPTH, CV_8U);
    setLUT();
}

void imageProcessor::setBrightness(int br) {
    brightness = (double) br / 100 - 0.5; 
    setLUT();
}

void imageProcessor::setContrast(int ct) {
    contrast = tan((double)ct / 200 * PI);
    setLUT();
}

void imageProcessor::setGamma(int gm) {
    gamma = tan((double)gm / 200 * PI);
    setLUT();
}

void imageProcessor::setLUT() {
    double _p;
    for (int i = 0; i < DEPTH; i++) {
        _p = ( i - DEPTH / 2) * contrast + DEPTH * (0.5 + brightness);
        if (DEPTH <= _p) {
            _p = DEPTH - 1;
        }
        else if (_p < 0) {
            _p = 0;
        }
        _p = pow(_p / DEPTH, gamma) * (DEPTH - 1);
        lut.at<char> (0, i) = (unsigned char)_p;
    }
}

void imageProcessor::process(Mat &img){
    LUT(img, lut, img);
}


