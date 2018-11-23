#include "recorder.h"

recorder::recorder(string fileDir, string codec, double fr, vector<int> res):
    frameRate(fr), resolution(res) {
    if (codec.size() != 4) {
        cout << "Error: wrong fourcc codec format." << endl;
        fourcc = 0;
    }
    else {
        fourcc = VideoWriter::fourcc(codec[0], codec[1], codec[2], codec[3]);
    }
    output = new VideoWriter(fileDir, fourcc, fr, Size(res[0], res[1]));
    if (!output->isOpened()) {
        cout << "Error: couldn't open video output file." << endl;
    }
}

recorder::~recorder() {
    delete output;
}

void recorder::changeOutput(string newFileDir) {
    delete output;
    output = new VideoWriter(newFileDir, fourcc, frameRate, 
            Size(resolution[0], resolution[1]));
    if (!output->isOpened()) {
        cout << "Error: couldn't open video output file." << endl;
    }
}

void recorder::record(const Mat &frame) {
    *output << frame;
}

