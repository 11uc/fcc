/*
Thread class to control video recording and CS/US dilivery, derived from QThread
Author: Chenghao Liu
Create: 2018/10/25
Modified: 2018/10/31
*/

#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>

#include "hardware.h"
#include "FcTimeParams.h"
#include "recorder.h"
#include "frameCatcher.h"
#include "imageProcessor.h"
#include "audioPlayer.h"
#include "pigpio.h"

class FcExperiment : public QObject {
    Q_OBJECT
    public:
        enum status : int {exp, expOn, cue, shock, iti, itiOff, 
            itiOn, rest, restOff, off};
        enum experimentType : int {habituation, conditioning, testing};
        enum recordType : int {continuous, episodic};
        FcExperiment(hardware *hw, FcTimeParams *tp, frameCatcher *fc, 
                imageProcessor *ip);
        ~FcExperiment();
        void setType(experimentType et, recordType rt);
        // start the experiment
        void start(QString ofile, int fr, const vector<int> &res, 
            const QString &audioFile, bool modify);
        void testSigStim(const QString &audioFile);
        void stopTest();
    protected:
        hardware *hwParam;  // Ids of hardwares to control
        map<string, double> tmParam;  // Times of different procedures
        unsigned int rounds;  // number of rounds
        frameCatcher *fcat;  // Capture camera frames
        QTimer *timer;  // Timer used to control time of each period
        recorder *record;  // Video recorder object
		AudioPlayer *splayer;
        imageProcessor *imgProc;  // processing image
        experimentType expType;  // Experiment type, habituation/conditioning/testing
        recordType recType;  // Recording type, continuous/episodic
        status stat;  // current status
        unsigned int leftRounds;  // rounds left to run
        double recExp;  // time of exploration recorded
        double recIti;  // time of inter-trial interval recorded
        double unrecRest;  // time of unrecorded rest
		GPIO *cueSigPin;
        GPIO *shockPin;  // GPIO controlling classes for shock

    public slots:
        // run the experiment
        void run(QString ofile, int fr, const vector<int> &res, bool modify);  
        void stop();  // stop the experiment

    signals:
        void stopped();

};
