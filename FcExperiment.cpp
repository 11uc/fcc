#include "FcExperiment.h"

FcExperiment::FcExperiment(hardware *hw, FcTimeParams *tp, frameCatcher *fc, 
        imageProcessor *ip): hwParam(hw), tmParam(tp->timeParams), rounds(tp->rounds),
        fcat(fc), imgProc(ip) {
    timer = new QTimer();
    timer->setSingleShot(true);
    record = nullptr;
    splayer = nullptr;
    expType = habituation;
    recType = continuous;
    stat = exp;
    leftRounds = tp->rounds;
}

FcExperiment::~FcExperiment() {
    delete timer;
    if (record != nullptr) 
        delete record;
    if (splayer != nullptr) 
        delete splayer;
    if (shockPin != nullptr) 
        delete shockPin;
    if (cueSigPin != nullptr) 
        delete cueSigPin;
}

void FcExperiment::setType(experimentType et, recordType rt) {
    expType = et;
    recType = rt;
}

void FcExperiment::run(QString ofile, int fr, const vector<int> &res, bool modify) {
    switch (stat) {
        case exp: 
            cout << "exp" << endl;
            stat = expOn;
            if (recType == continuous or tmParam["explore"] < tmParam["cue"]) {
                recExp = tmParam["explore"];
                timer->start(0);
            }
            else {
                recExp = tmParam["cue"];
                timer->start((int)((tmParam["explore"] - tmParam["cue"]) * 1000));
            }
            break;
        case expOn: 
            cout << "expOn" << endl;
            //turn on camera and record
            if (recType == episodic) {
                ofile.append('_').append(QString::number(0));
            }
            ofile.append(".avi");
            record = new recorder(ofile.toStdString(), 
                    hwParam->getFourcc().toStdString(), fr, res);
            if (!modify) {
                connect(fcat, &frameCatcher::frameCaptured, record, &recorder::record);
            }
            else {
                connect(fcat, &frameCatcher::frameCaptured, [=] (Mat img) 
                        {imgProc->process(img); record->record(img);});
            }
            fcat->start();
            stat = expType == habituation ? rest : cue;
            timer->start((int) (recExp * 1000));
            break;
        case cue:
            cout << "cue" << endl;
            // start play cue sound
            splayer->play();
			cueSigPin->write(1);
            stat = shock;
            timer->start((int) ((tmParam["cue"] + hwParam->getAudioDelay())* 1000));
            break;
        case shock:
            cout << "shock" << endl;
            if (expType == conditioning) {
                // start the shock
				shockPin->write(0);
            }
            if (fr == 0) {
                stat = off;
            }
            else {
                stat = --leftRounds == 0 ? rest : iti;
            }
            timer->start((int) (tmParam["shock"] * 1000));
            break;
        case iti:
            cout << "iti" << endl;
            // stop the cue
            splayer->stop();
			cueSigPin->write(0);
            if (expType == conditioning) {
                // stop the shock
				shockPin->write(1);
            }
            stat = recType == continuous ? cue : itiOff;
            if (recType == continuous) {
                timer->start((int) (tmParam["iti"] * 1000));
            }
            else {
                recIti = 2 * tmParam["cue"] < tmParam["iti"] ? 
                    tmParam["cue"] : tmParam["iti"] / 2;
                timer->start((int) (recIti * 1000));
            }
            break;
        case itiOff:
            cout << "itiOff" << endl;
            // turn off recording, switch output file, stop capturing
            fcat->mutex->lock();
            fcat->stopSig = true;
            fcat->mutex->unlock();
            while(!fcat->isFinished()) {}
            fcat->stopSig = false;
            ofile.append('_').append(
                    QString::number(rounds - leftRounds)).append(".avi");
            record->changeOutput(ofile.toStdString());
            stat = itiOn;
            timer->start((int) ((tmParam["iti"] - 2 * recIti) * 1000));
            break;
        case itiOn:
            cout << "itiOn" << endl;
            // turn on recording again
            fcat->start();
            stat = cue;
            timer->start((int) (recIti * 1000));
            break;
        case rest:
            cout << "rest" << endl;
            if (expType != habituation) {
                // turn off the cue 
                splayer->stop();
				cueSigPin->write(0);
                if (expType == conditioning) {
                    // turn off the shock
                    shockPin->write(1);
                }
            }
            stat = restOff;
            unrecRest = recType == continuous || tmParam["rest"] < tmParam["cue"] ?
                0 : tmParam["rest"] - tmParam["cue"];
            timer->start((int) ((tmParam["rest"] - unrecRest) * 1000));
            break;
        case restOff:
            cout << "restOff" << endl;
            // turn off recording
            fcat->mutex->lock();
            fcat->stopSig = true;
            fcat->mutex->unlock();
            while(!fcat->isFinished()) {}
            stat = off;
            timer->start((int) (unrecRest * 1000));
            break;
        default:
            cout << "off" << endl;
            if (fr == 0) {
                stopTest();
            }
            else {
                stop();
            }
    }
}

void FcExperiment::start(QString ofile, int fr, const vector<int> &res,
        const QString &audioFile, bool modify) {
    if (expType != habituation) {
        // setup audio file
		splayer = new AudioPlayer(audioFile);
        // setup gpio
		cueSigPin = new GPIO(hwParam->getCueSigPinId(), GPIO::out);
		shockPin = new GPIO(hwParam->getShockPinId(), GPIO::out, 1);
    }
    stat = exp;
    connect(timer, &QTimer::timeout, [=] () {run(ofile, fr, res, modify);});
    run(ofile, fr, res, modify);
}

void FcExperiment::stop() {
    switch (stat) {
        case iti: case rest:
            if (expType == conditioning) {
                // stop the shock
				shockPin->write(1);
            }
        case shock:
            if (expType != habituation) {
                splayer->stop();
				cueSigPin->write(0);
                delete splayer;
                splayer = nullptr;
				delete cueSigPin;
				delete shockPin;
				shockPin = cueSigPin = nullptr;
            }
        case cue: case itiOff: case restOff:
            fcat->mutex->lock();
            fcat->stopSig = true;
            fcat->mutex->unlock();
            while(!fcat->isFinished()) {}
            fcat->stopSig = false;
            delete fcat;
            fcat = nullptr;
            delete record;
            record = nullptr;
        case expOn: case itiOn: case off:
            timer->stop();
            timer->disconnect();
        default:
            break;
    }
    emit stopped();
}

void FcExperiment::testSigStim(const QString &audioFile) {
    // setup audio file
	splayer = new AudioPlayer(audioFile);
    // Setup GPIO
	cueSigPin = new GPIO(hwParam->getCueSigPinId(), GPIO::out);
	shockPin = new GPIO(hwParam->getShockPinId(), GPIO::out, 1);
    stat = cue;
    vector<int> tmp(2, 0);
    connect(timer, &QTimer::timeout, [=] () {run("", 0, tmp, false);});
    run("", 0, tmp, false);
}

void FcExperiment::stopTest() {
    switch (stat) {
        case off:
            // stop the shock
			shockPin->write(1);
        default:
            splayer->stop();
			cueSigPin->write(0);
            delete splayer;
            splayer = nullptr;
			delete cueSigPin;
			delete shockPin;
			shockPin = cueSigPin = nullptr;
            timer->stop();
            timer->disconnect();
    }
    emit stopped();
}
