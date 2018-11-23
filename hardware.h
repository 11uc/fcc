/*
Manage hardware parameters for the program
Author: Chenghao Liu
Create: 2018/10/26
Modified: 2018/10/31
*/

#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

using namespace std;

class hardware : public QDialog{
    protected:
        unsigned int camId;
        QString fourcc;
        unsigned int shockPinId;
        unsigned int cueSigPinId;
        double audioDelay;
        // Dialog window components
        QLineEdit *cameraIdLine;
        QLineEdit *shockPinIdLine;
        QLineEdit *cueSigPinIdLine;
        QLineEdit *audioDelayLine;
        QLineEdit *codecLine;
        QPushButton *okBtn;
        QPushButton *cancelBtn;

    public:
        hardware(string fileDir = "");  // load parameters from initialization file
                                        // or set as default
        void save(string fileDir);  // save parameters to an initialization file
        bool config();  // configurate parameters using a dialogue window
        void setupUI();  // setup dialog UI for configuration

        unsigned int getCamId();
        QString getFourcc();
        unsigned int getShockPinId();
        unsigned int getCueSigPinId();
        double getAudioDelay();
};
