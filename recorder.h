/*
Class for saving opencv frames in a video file
Author: Chenghao Liu
Created: 2018/10/25
Modified: 2018/10/29
*/

#pragma once
#include <string>
#include <iostream>
#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

class recorder : public QObject {
    Q_OBJECT

    protected:
        VideoWriter *output;
        int fourcc;
        double frameRate;
        vector<int> resolution;
    
    public:
        recorder(string fileDir, string codec, double fr, vector<int> res);
        ~recorder();
        void changeOutput(string newFileDir);  // write to another file without 
                                            // destroying this object.

    public slots:
        void record(const Mat &frame);
};
