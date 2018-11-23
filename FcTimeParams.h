/*
Manage the time parameters for fear conditioning, transfer them between the window, 
the file and the parameters variables in the program for controlling experiments.
Author: Chenghao Liu
Create: 2018/10/26
Modified: 2018/10/28
*/

#pragma once
#include <map>
#include <QLineEdit>
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class FcTimeParams {
    public:
        // All time parameters have a unit of second
        // "explore", pre-training exploration time
        // "cue", cue delivery time (CS)
        // "shock", electric shock time (US)
        // "iti", inter-trial interval
        // "rest", post-training rest time
        map<string, double> timeParams;
        unsigned int rounds;  // number of trials
        QWidget *parent;

        FcTimeParams(QWidget *p = nullptr); // tl - lineEdit widgets in the gui
        bool update(const vector<QLineEdit *> &tl,
                bool display);  // update member variables based on tl or 
                                // update window display based on member variables
        bool save(const vector<QLineEdit *> &tl,
                const QString &workDir);  // update member and save variables to a file
        bool load(const vector<QLineEdit *> &tl,
                const QString &workDir);  // load variables from a file
};
