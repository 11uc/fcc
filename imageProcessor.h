/*
A class to adjust brightness, contrast and gamma value of an image.
Author: Chenghao Liu
Created: 10/23/18
Last modified: 10/25/18
*/

#pragma once
#include <cmath>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QObject>

#define PI 3.141592653
#define DEPTH 256

using namespace std;
using namespace cv;

class imageProcessor : public QObject {
    Q_OBJECT
    protected:
        // all these parameters are integers in [0, 100) 
        // they'll be converted 
        double brightness;
        double contrast;
        double gamma;
        Mat lut;
        void setLUT();

    public:
        imageProcessor(int br = 50, int ct = 50, int gm = 50);
        void process(Mat &img);

    public slots:
        void setBrightness(int br);
        void setContrast(int ct);
        void setGamma(int gm);
};
