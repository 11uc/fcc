/*
Qthread class to capture camera recording using OpenCV.
Author: Chenghao Liu
Created: 10/22/18
Last modified: 10/25/28
*/

#pragma once
#include <iostream>
#include <QThread>
#include <QMutex>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class frameCatcher : public QThread {
    Q_OBJECT
    protected:
        int camId;
        int frameRate;
        vector<int> res;
    
    signals:
        void frameCaptured(Mat frame);

    public:
        QMutex *mutex;  
        bool stopSig;  // mutex and stop signal used to stop capturing
        frameCatcher(int ci, int fr, const vector<int> &r, QObject *p);
        ~frameCatcher();
        void run();
};
