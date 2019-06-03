#include "frameCatcher.h"

frameCatcher::frameCatcher(int ci, int fr, const vector<int> &r, QObject *p):
    QThread(p), camId(ci), frameRate(fr), res(r) {
    mutex = new QMutex();
    stopSig = false;
}

frameCatcher::~frameCatcher() {
    delete mutex;
    wait();
}

void frameCatcher::run() {
    VideoCapture cap(camId);
    if (!cap.isOpened()) {
        cerr << "ERROR: Can't initialize camera capture" << endl;
        return;
    }
    cap.set(CAP_PROP_FRAME_WIDTH, res[0]);
    cap.set(CAP_PROP_FRAME_HEIGHT, res[1]);
    cap.set(CAP_PROP_FPS, frameRate);
    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cerr << "ERROR: Can't grab camera frame." << endl;
            break;
        }
        cvtColor(frame, frame, CV_BGR2RGB);
        emit frameCaptured(frame);
        mutex->lock();
        if (stopSig) {
            break;
        }
        mutex->unlock();
    }
    mutex->unlock();
    cap.release();
}
