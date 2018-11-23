/*
Fcrig -
Video recording, cue delivering and shock control for fear conditioning
experiment.
Author: Chenghao Liu
Created: 2018/10/21
Last modified: 2018/10/21
*/

#include <QApplication>
#include "mainWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    mainWindow mW;
    mW.show();
    return app.exec();
}
