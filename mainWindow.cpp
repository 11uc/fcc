#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent) {
    // initialization
    initParam();
    initExpCtl();

    // Setup UI
    buildMenu();
    buildUI();
    setupBinding();
}

mainWindow::~mainWindow() {
    delete vpixmap;  // pixmap used to display the video
    delete imgProc;  // Object for processing the frames
    delete hwParam;  
    delete timeParams;
    if (frCat != nullptr) {
        delete frCat;
    }
    if (ex != nullptr) {
        delete ex;
    }
}

void mainWindow::buildMenu() {  // menu
    QAction *hardwareAct = new QAction("&Hardware configuration", this);
    QAction *loadAct = new QAction("&Load parameters", this);
    QAction *saveAct = new QAction("&Save parameters", this);
    QAction *exitAct = new QAction("E&xit", this);
    exitAct->setShortcut(tr("Ctrl + Q"));
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(hardwareAct);
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exitAct);

    QEventLoop::connect(exitAct, &QAction::triggered, this, &mainWindow::close);
    QEventLoop::connect(hardwareAct, &QAction::triggered, hwParam, &hardware::config);
    QEventLoop::connect(saveAct, &QAction::triggered, 
            [=] () {timeParams->save(timelines, workDir);});
    QEventLoop::connect(loadAct, &QAction::triggered,
            [=] () {timeParams->load(timelines, workDir);});
}

void mainWindow::buildUI() { // Main window area, input and output
    workDirLine = new QLineEdit(this);
    workDirLine->setText(workDir);
    findDirBtn = new QPushButton("...", this);
    findDirBtn->setFixedWidth(40);
    QHBoxLayout *dirHB = new QHBoxLayout();
    dirHB->addWidget(new QLabel("Working directory", this));
    dirHB->addWidget(workDirLine);
    dirHB->addWidget(findDirBtn);

    soundFileLine = new QLineEdit(this);
    soundFileLine->setText(audioFile);
    findFileBtn = new QPushButton("...", this);
    findFileBtn->setFixedWidth(40);
    QHBoxLayout *soundHB = new QHBoxLayout();
    soundHB->addWidget(new QLabel("Sound file", this));
    soundHB->addWidget(soundFileLine);
    soundHB->addWidget(findFileBtn);

    resolutionCB = new QComboBox(this);
    QStringList resolutionString = {"640x480", "720x640", "1080x720"};
    resolutionCB->addItems(resolutionString);
    frLine = new QLineEdit(this);
    brightnessSld = new QSlider(Qt::Horizontal, this);
    brightnessSld->setValue(50);
    QLabel *brightnessPct = new QLabel(QString::number(brightnessSld->value()), this);
    contrastSld = new QSlider(Qt::Horizontal, this);
    contrastSld->setValue(50);
    QLabel *contrastPct = new QLabel(QString::number(contrastSld->value()), this);
    gammaSld = new QSlider(Qt::Horizontal, this);
    gammaSld->setValue(50);
    QLabel *gammaPct = new QLabel(QString::number(gammaSld->value()), this);
    modifyCheck = new QCheckBox("Apply", this);
    previewBtn = new QPushButton("Start preview", this);
    animalNumLine = new QLineEdit(this);
    sessionNumLine = new QLineEdit(this);
    expTypeCB = new QComboBox(this);
    QStringList expTypes = {"Habituation", "Conditioning", "Test"};
    expTypeCB->addItems(expTypes);
    recordTypeCB = new QComboBox(this);
    QStringList recordTypes = {"Continuing", "Episodic"};
    recordTypeCB->addItems(recordTypes);
    exploreLine = new QLineEdit(this);
    QHBoxLayout *exploreHB = new QHBoxLayout();
    exploreHB->addWidget(exploreLine);
    exploreHB->addWidget(new QLabel("s", this));
    cueLine = new QLineEdit(this);
    QHBoxLayout *cueHB = new QHBoxLayout();
    cueHB->addWidget(cueLine);
    cueHB->addWidget(new QLabel("s", this));
    shockLine = new QLineEdit(this);
    QHBoxLayout *shockHB = new QHBoxLayout();
    shockHB->addWidget(shockLine);
    shockHB->addWidget(new QLabel("s", this));
    itiLine = new QLineEdit(this);
    QHBoxLayout *itiHB = new QHBoxLayout();
    itiHB->addWidget(itiLine);
    itiHB->addWidget(new QLabel("s", this));
    roundsLine = new QLineEdit(this);
    restLine = new QLineEdit(this);
    QHBoxLayout *restHB = new QHBoxLayout();
    restHB->addWidget(restLine);
    restHB->addWidget(new QLabel("s", this));
    intensityLine = new QLineEdit(this);
    QHBoxLayout *intensityHB = new QHBoxLayout();
    intensityHB->addWidget(intensityLine);
    intensityHB->addWidget(new QLabel("mA", this));
    displayCheck = new QCheckBox("Display", this);
    startBtn = new QPushButton("Start", this);
    stopBtn = new QPushButton("Stop", this);
    testBtn = new QPushButton("Test", this);
    QGridLayout *ctlGrid = new QGridLayout();
    ctlGrid->addWidget(new QLabel("Resolution", this), 0, 0, Qt::AlignCenter);
    ctlGrid->addWidget(resolutionCB, 0, 1);
    ctlGrid->addWidget(new QLabel("Frame rate", this), 0, 2, Qt::AlignCenter);
    ctlGrid->addWidget(frLine, 0, 3);
    ctlGrid->addWidget(new QLabel("Brightness", this), 1, 0, Qt::AlignCenter);
    ctlGrid->addWidget(brightnessSld, 1, 1);
    ctlGrid->addWidget(brightnessPct, 1, 2);
    ctlGrid->addWidget(new QLabel("Contrast", this), 2, 0, Qt::AlignCenter);
    ctlGrid->addWidget(contrastSld, 2, 1);
    ctlGrid->addWidget(contrastPct, 2, 2);
    ctlGrid->addWidget(new QLabel("Gamma", this), 3, 0, Qt::AlignCenter);
    ctlGrid->addWidget(gammaSld, 3, 1);
    ctlGrid->addWidget(gammaPct, 3, 2);
    ctlGrid->addWidget(modifyCheck, 3, 3);
    ctlGrid->addWidget(previewBtn, 4, 2);
    ctlGrid->addWidget(new QLabel("Animal number", this), 5, 0, Qt::AlignRight);
    ctlGrid->addWidget(animalNumLine, 5, 1);
    ctlGrid->addWidget(new QLabel("Session number", this), 5, 2, Qt::AlignRight);
    ctlGrid->addWidget(sessionNumLine, 5, 3);
    ctlGrid->addWidget(new QLabel("Experiment type", this), 6, 0, Qt::AlignRight);
    ctlGrid->addWidget(expTypeCB, 6, 1);
    ctlGrid->addWidget(new QLabel("Record type", this), 6, 2, Qt::AlignRight);
    ctlGrid->addWidget(recordTypeCB, 6, 3);
    ctlGrid->addWidget(new QLabel("Exploration", this), 7, 0, Qt::AlignRight);
    ctlGrid->addLayout(exploreHB, 7, 1);
    ctlGrid->addWidget(new QLabel("Cue", this), 7, 2, Qt::AlignRight);
    ctlGrid->addLayout(cueHB, 7, 3);
    ctlGrid->addWidget(new QLabel("Shock", this), 8, 0, Qt::AlignRight);
    ctlGrid->addLayout(shockHB, 8, 1);
    ctlGrid->addWidget(new QLabel("ITI", this), 8, 2, Qt::AlignRight);
    ctlGrid->addLayout(itiHB, 8, 3);
    ctlGrid->addWidget(new QLabel("Rounds", this), 9, 0, Qt::AlignRight);
    ctlGrid->addWidget(roundsLine, 9, 1);
    ctlGrid->addWidget(new QLabel("Rest", this), 9, 2, Qt::AlignRight);
    ctlGrid->addLayout(restHB, 9, 3);
    ctlGrid->addWidget(new QLabel("Shock intensity", this), 10, 0, Qt::AlignRight);
    ctlGrid->addLayout(intensityHB, 10, 1);
    ctlGrid->addWidget(displayCheck, 10, 3);
    ctlGrid->addWidget(startBtn, 11, 0);
    ctlGrid->addWidget(stopBtn, 11, 1);
    ctlGrid->addWidget(testBtn, 11, 2);
    QVBoxLayout *ctlVB = new QVBoxLayout();
    ctlVB->addLayout(dirHB);
    ctlVB->addLayout(soundHB);
    ctlVB->addLayout(ctlGrid);
    ctlVB->addStretch(1);

    videoLb = new QLabel(this);
    videoLb->setMinimumSize(650, 500);
    videoLb->setAlignment(Qt::AlignCenter);
    videoPm = new QPixmap();

    QHBoxLayout *topHB = new QHBoxLayout();
    topHB->addWidget(videoLb);
    topHB->addLayout(ctlVB);
    QWidget *placeHolder = new QWidget(this);
    placeHolder->setLayout(topHB);
    setCentralWidget(placeHolder);

    timelines.push_back(cueLine);
    timelines.push_back(exploreLine);
    timelines.push_back(itiLine);
    timelines.push_back(restLine);
    timelines.push_back(shockLine);
    timelines.push_back(roundsLine);

    QEventLoop::connect(brightnessSld, &QSlider::valueChanged,
        [=] (const int v) {brightnessPct->setText(QString::number(v));});
    QEventLoop::connect(contrastSld, &QSlider::valueChanged,
        [=] (const int v) {contrastPct->setText(QString::number(v));});
    QEventLoop::connect(gammaSld, &QSlider::valueChanged,
        [=] (const int v) {gammaPct->setText(QString::number(v));});
}

void mainWindow::setupBinding() {
    // Buttons
    connect(previewBtn, &QPushButton::clicked, this, &mainWindow::startPreview);
    connect(startBtn, &QPushButton::clicked, this, &mainWindow::startEx);
    stopBtn->setDisabled(true);
    connect(testBtn, &QPushButton::clicked, this, &mainWindow::testSigStim);
    connect(findDirBtn, &QPushButton::clicked, this, &mainWindow::setWorkDir);
    connect(findFileBtn, &QPushButton::clicked, this, &mainWindow::setSoundFile);
    // Lines
    connect(workDirLine, &QLineEdit::textChanged, [=] (QString l) {workDir = l;});
    connect(soundFileLine, &QLineEdit::textChanged, [=] (QString l) {audioFile = l;});
}

void mainWindow::initParam() {
    qRegisterMetaType<Mat>("Mat");  // register type to be passed by signal slot
    codec = "MPEG";
    timeParams = new FcTimeParams(this);
    resolutions.assign({{640, 480}, {720, 640}, {1080, 720}});
    imgProc = new imageProcessor(50, 50, 50);
    frCat = nullptr;
    loadIni("fcrig.ini");
}

void mainWindow::initExpCtl() {
    vpixmap = new QPixmap(resolutions[0][0], resolutions[0][1]);
    vpixmap->fill(Qt::black);
    hwParam = new hardware("fcrig.ini");  // hardcoded ini file name
    ex = nullptr;
}

void mainWindow::startPreview() {
    int fr = frLine->text().toInt();
    if (fr <= 0 or 20 < fr) {
        QMessageBox::warning(this, QString("Warning"), 
            QString("Frame rate is an integer between 0~20"), 
            QMessageBox::Ok);
        return;
    }
    vector<int> rs(resolutions[resolutionCB->currentIndex()]);
    frCat = new frameCatcher(hwParam->getCamId(), fr, rs, this);
    connect(frCat, &frameCatcher::frameCaptured, this, &mainWindow::displayFrame);
    frCat->start();
    previewBtn->setText("Stop preview");
    previewBtn->disconnect();
    connect(previewBtn, &QPushButton::clicked, [=] () {mainWindow::stopDisplay(0);});
    // Modifying slide bars only take effect during preview
    connect(brightnessSld, &QSlider::valueChanged, imgProc, &imageProcessor::setBrightness);
    connect(contrastSld, &QSlider::valueChanged, imgProc, &imageProcessor::setContrast);
    connect(gammaSld, &QSlider::valueChanged, imgProc, &imageProcessor::setGamma);
}

void mainWindow::stopDisplay(int state) {
    frCat->mutex->lock();
    frCat->stopSig = true;
    frCat->mutex->unlock();
    frCat->disconnect();
    while (!frCat->isFinished()) {}
    delete frCat;
    frCat = nullptr;
    if (state == 0) {  // state 0: preview state, after stopped, reset button
        previewBtn->setText("Start preview");
        previewBtn->disconnect();
        connect(previewBtn, &QPushButton::clicked, this, &mainWindow::startPreview);
        // Modifying slide bars only take effect during preview
        brightnessSld->disconnect(imgProc);
        contrastSld->disconnect(imgProc);
        gammaSld->disconnect(imgProc);
    }
}

void mainWindow::displayFrame(Mat img) {
    cvtColor(img, img, CV_BGR2RGB);
    if (modifyCheck->isChecked()) {
        imgProc->process(img);
    }
    QImage imgIn = QImage((uchar*) img.data, img.cols, img.rows, 
        img.step, QImage::Format_RGB888);
    vpixmap->convertFromImage(imgIn);
    videoLb->setPixmap(*vpixmap);
}

void mainWindow::startEx() {  // start the experiment
    int fr = frLine->text().toInt();
    if (fr <= 0 or 20 < fr) {
        QMessageBox::warning(this, QString("Warning"), 
            QString("Frame rate is an integer between 0~20"), 
            QMessageBox::Ok);
        return;
    }
    vector<int> rs(resolutions[resolutionCB->currentIndex()]);
    QString outputFile(workDir + "/FC_" + animalNumLine->text() + '_' +
            sessionNumLine->text());
    // change UI
    stopBtn->setDisabled(false);
    startBtn->setDisabled(true);
    testBtn->setDisabled(true);
    repaint();
    timeParams->update(timelines, false);  // update time parameters
    frCat = new frameCatcher(hwParam->getCamId(), fr, rs, this);
    ex = new FcExperiment(hwParam, timeParams, frCat, imgProc);
    ex->setType((FcExperiment::experimentType) expTypeCB->currentIndex(),
            (FcExperiment::recordType) recordTypeCB->currentIndex());
    if (displayCheck->isChecked()) {
        connect(frCat, &frameCatcher::frameCaptured, this, &mainWindow::displayFrame);
    }
    connect(ex, &FcExperiment::stopped, this, &mainWindow::stopEx);
    connect(stopBtn, &QPushButton::clicked, ex, &FcExperiment::stop);
    ex->start(outputFile, fr, rs, audioFile, modifyCheck->isChecked());  // start
}

void mainWindow::stopEx() {
    disconnect(stopBtn, &QPushButton::clicked, ex, &FcExperiment::stop);
    disconnect(ex, &FcExperiment::stopped, this, &mainWindow::stopEx);
    delete ex;
    ex = nullptr;
    stopBtn->setDisabled(true);
    startBtn->setDisabled(false);
    testBtn->setDisabled(false);
    repaint();
}

void mainWindow::testSigStim() {
    stopBtn->setDisabled(false);
    startBtn->setDisabled(true);
    testBtn->setDisabled(true);
    repaint();
    ex = new FcExperiment(hwParam, timeParams, frCat, imgProc);
    connect(ex, &FcExperiment::stopped, this, &mainWindow::stopTest);
    connect(stopBtn, &QPushButton::clicked, ex, &FcExperiment::stopTest);
    ex->testSigStim(audioFile); 
}

void mainWindow::stopTest() {
    disconnect(ex, &FcExperiment::stopped, this, &mainWindow::stopTest);
    disconnect(stopBtn, &QPushButton::clicked, ex, &FcExperiment::stopTest);
    delete ex;
    ex = nullptr;
    stopBtn->setDisabled(true);
    startBtn->setDisabled(false);
    testBtn->setDisabled(false);
    repaint();
}


void mainWindow::saveIni(string fileDir) {
    hwParam->save(fileDir);
    ofstream iniOf(fileDir, ios::out | ios::binary | ios::app);
    if (iniOf.is_open()) {
        unsigned long lenFileDir = workDir.size();
        unsigned long lenAudioFile = audioFile.size();
        iniOf.write((char *) &lenFileDir, sizeof(long));
        iniOf.write((char *) &lenAudioFile, sizeof(long));
        iniOf.write((char *) workDir.toStdString().c_str(), lenFileDir);
        iniOf.write((char *) audioFile.toStdString().c_str(), lenAudioFile);
        iniOf.close();
    }
    else {
        cout << "Error, unable to open initialization file." << endl;
    }
}

void mainWindow::loadIni(string fileDir) {
    ifstream iniIf(fileDir, ios::in | ios::binary);
    if (iniIf.is_open()) {
        try {
            unsigned long lenFileDir, lenAudioFile;
            iniIf.seekg(INIFILESHIFT);
            iniIf.read((char *) &lenFileDir, sizeof(long));
            iniIf.read((char *) &lenAudioFile, sizeof(long));
            char buf[1000];
            iniIf.read(buf, lenFileDir);
            buf[lenFileDir] = '\0';
            workDir = buf;
            iniIf.read(buf, lenAudioFile);
            buf[lenAudioFile] = '\0';
            audioFile = buf;
            iniIf.close();
        }
        catch (Exception e) {
            cerr << "Warning: unable to load from ini file." << endl;
        }
    }
}

void mainWindow::setWorkDir() {
    if (workDir.size() != 0) {
        workDir = QFileDialog::getExistingDirectory(this, "Choose directory", 
                 workDir, QFileDialog::ShowDirsOnly);
    }
    else {
        workDir = QFileDialog::getExistingDirectory(this, "Choose directory", 
                 "/home", QFileDialog::ShowDirsOnly);
    }
    workDirLine->setText(workDir);
}

void mainWindow::setSoundFile() {
    char cwd[1000];
    getcwd(cwd, 1000);
    audioFile = QFileDialog::getOpenFileName(this, "Choose file", 
             cwd, "Audios (*.mp3 *.wav *.au *.aiff)");
    soundFileLine->setText(audioFile);
}

void mainWindow::closeEvent(QCloseEvent *e) {
    int ret = QMessageBox::question(this, "Exit", "Save before exit?",
            QMessageBox::Save | QMessageBox::Close | QMessageBox::Cancel);
    if (ret == QMessageBox::Save) {
        saveIni("fcrig.ini");
        e->accept();
    }
    else if (ret == QMessageBox::Close) {
        e->accept();
    }
    else {
        e->ignore();
    }
}
