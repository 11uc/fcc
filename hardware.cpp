#include "hardware.h"

hardware::hardware(string fileDir) {
    ifstream iniIf(fileDir, ios::in | ios::binary);
    camId = 0;
    fourcc = "MPEG";
    shockPinId = 0;
    cueSigPinId = 0;
    audioDelay = 0;
    if (iniIf.is_open()) {
        char bufC[5];
        bufC[4] = '\0';
        unsigned int bufInt[3];
        double bufDb;
        if(iniIf.read((char*) bufInt, 3 * sizeof(unsigned int)) &&
            iniIf.read((char *) &bufDb, sizeof(double)) &&
            iniIf.read(bufC, 4 * sizeof(char))) {
            camId = bufInt[0], shockPinId = bufInt[1], cueSigPinId = bufInt[2];
            audioDelay = bufDb;
            fourcc = bufC;
        }
        else {
            cout << "Warning, wrong initilization file format." << endl;
        }
    }
    else {
        cout << "Warning, initialization file not found." << endl;
    }
    iniIf.close();
    //
}

void hardware::save(string fileDir) {
    ofstream iniOf(fileDir, ios::out | ios::binary);
    if (iniOf.is_open()) {
        iniOf.write((char *) &camId, sizeof(unsigned int));
        iniOf.write((char *) &shockPinId, sizeof(unsigned int));
        iniOf.write((char *) &cueSigPinId, sizeof(unsigned int));
        iniOf.write((char *) &audioDelay, sizeof(double));
        iniOf.write(fourcc.toStdString().c_str(), 4 * sizeof(char));
        iniOf.close();
    }
}

void hardware::setupUI() {
    cameraIdLine = new QLineEdit(this);
    cameraIdLine->setText(QString::number(camId));
    shockPinIdLine = new QLineEdit(this);
    shockPinIdLine->setText(QString::number(shockPinId));
    cueSigPinIdLine = new QLineEdit(this);
    cueSigPinIdLine->setText(QString::number(cueSigPinId));
    audioDelayLine = new QLineEdit(this);
    audioDelayLine->setText(QString::number(audioDelay, 10, 2));
    codecLine = new QLineEdit(this);
    codecLine->setText(fourcc);
    okBtn = new QPushButton("OK", this);
    cancelBtn = new QPushButton("Cancel", this);
    QGridLayout grid(this);
    grid.addWidget(new QLabel("Camera Id", this), 0, 0, Qt::AlignCenter);
    grid.addWidget(cameraIdLine, 0, 1, Qt::AlignCenter);
    grid.addWidget(new QLabel("Shock Pin Id", this), 1, 0, Qt::AlignCenter);
    grid.addWidget(shockPinIdLine, 1, 1, Qt::AlignCenter);
    grid.addWidget(new QLabel("Cue Signal Pin Id", this), 2, 0, Qt::AlignCenter);
    grid.addWidget(cueSigPinIdLine, 2, 1, Qt::AlignCenter);
    grid.addWidget(new QLabel("Audio file Delay", this), 3, 0, Qt::AlignCenter);
    grid.addWidget(audioDelayLine, 3, 1, Qt::AlignCenter);
    grid.addWidget(new QLabel("Video codec", this), 4, 0, Qt::AlignCenter);
    grid.addWidget(codecLine, 4, 1, Qt::AlignCenter);
    grid.addWidget(okBtn, 5, 0, Qt::AlignCenter);
    grid.addWidget(cancelBtn, 5, 1, Qt::AlignCenter);
    setLayout(&grid);
    // binding
    connect(okBtn, &QPushButton::clicked, this, &hardware::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &hardware::reject);
    show();
}

bool hardware::config() {
    setupUI();
    bool ret = exec();
    if (ret == QDialog::Accepted) {
        bool *ok = new bool;
        bool good = true;
        camId = cameraIdLine->text().toUInt(ok);
        good = good && ok;
        shockPinId = shockPinIdLine->text().toUInt(ok);
        good = good && ok;
        cueSigPinId = cueSigPinIdLine->text().toUInt(ok);
        good = good && ok;
        audioDelay = audioDelayLine->text().toDouble(ok);
        good = good && ok;
        fourcc = codecLine->text();
        if (!good || fourcc.size() != 4) {
            cout << "Warning, wrong configuration parameters" << endl;
            camId = 0;
            fourcc = "MPEG";
            shockPinId = 0;
            cueSigPinId = 0;
            audioDelay = 0;
        }
        return true;
    }
    return false;
}

unsigned int hardware::getCamId() {
    return camId;
}

QString hardware::getFourcc() {
    return fourcc;
}

unsigned int hardware::getShockPinId() {
    return shockPinId;
}
unsigned int hardware::getCueSigPinId() {
    return cueSigPinId;
}

double hardware::getAudioDelay() {
    return audioDelay;
}
