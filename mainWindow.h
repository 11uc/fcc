/*
Main window of fcrig -
Build main window UI, setup menu, button and line input and video display.
Author: Chenghao Liu
Created: 2018/10/21
Last modified: 2018/10/25
*/

#pragma once

#include <unistd.h>
#include <string>
#include <sstream>

#include <QCloseEvent>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMutex>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "frameCatcher.h"
#include "imageProcessor.h"
#include "hardware.h"
#include "FcTimeParams.h"
#include "FcExperiment.h"

#define INIFILESHIFT (3 * sizeof(unsigned int) + sizeof(double) + 4 * sizeof(char))

using namespace std;
using namespace cv;

class mainWindow : public QMainWindow {
    Q_OBJECT
    protected:
        /* Variable members */
        // window components
        QLineEdit *workDirLine, *soundFileLine, *frLine, *animalNumLine, *sessionNumLine,
            *exploreLine, *cueLine, *itiLine, *shockLine, *roundsLine, *restLine,
            *intensityLine;
        QPushButton *findDirBtn, *findFileBtn, *defaultBtn, *previewBtn, *startBtn, 
            *stopBtn, *testBtn;
        QComboBox *resolutionCB, *expTypeCB, *recordTypeCB;
        QSlider *brightnessSld, *contrastSld, *gammaSld;
        QCheckBox *displayCheck, *modifyCheck;
        QLabel *videoLb;
        QPixmap *videoPm;
        vector<QLineEdit *> timelines;

        // control parameters
        hardware *hwParam;  // hardware parameters
        vector<vector<int>> resolutions;  // video resolution
        FcTimeParams *timeParams;  // time parameters
        QString codec;

        // file and directory
        QString workDir;  // working directory
        QString audioFile;  // audio file

        // others
        QPixmap *vpixmap;  // pixmap used to display the video
        frameCatcher *frCat;  // Qthread for capture camera video
        imageProcessor *imgProc;  // Object for processing the frames
        FcExperiment *ex;  // Experiment control object

        /* function members */
        // GUI setup
        void buildUI();  // Draw window and components
        void setupBinding();  // Setup UI connections
        void buildMenu();
        // Initialization and exit
        void initParam();  // Initialize experimental parameters 
        void initExpCtl();  // Initialize parameters for experiment control
        void closeEvent(QCloseEvent *e);  // Exit program
        // Video related functions
        void startPreview();  // respond to preview button click
        void displayFrame(Mat img);  // Display frame in on the video label
        void stopDisplay(int state = 0);  // Stop diplaying video
        // Experiments control
        void startEx();
        void stopEx();
        void testSigStim();  // test signal and stimulation
        void stopTest();  // stop testing signal and stimulation
        // file
        void saveIni(string fileDir);  // load from initialization file
        void loadIni(string fileDir);  // save to initialization file
        void setWorkDir();  // dialog of working directory selection
        void setSoundFile();  // dialog of audio file selection
    public:
        mainWindow(QWidget *parent = 0);
        ~mainWindow();
};
