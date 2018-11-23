#include "FcTimeParams.h"


FcTimeParams::FcTimeParams(QWidget *p): rounds(1), parent(p) {
    timeParams["explore"] = 5;
    timeParams["cue"] = 5;
    timeParams["shock"] = 1;
    timeParams["iti"] = 5;
    timeParams["rest"] = 5;
}

bool FcTimeParams::update(const vector<QLineEdit *> &tl, bool display) {
    map<string, double>::iterator in = timeParams.begin();
    vector<QLineEdit *>::const_iterator out = tl.begin();
    bool *ok = new bool;
    for (;in != timeParams.end() && out != tl.end(); in++, out++) {
        if (display) {
            (*out)->setText(QString::number(in->second));
        }
        else {
            in->second = (*out)->text().toDouble(ok);
            if (!*ok || in->second < 0) {
                cout << "Error: time parameters should be positve real number." << endl;
                delete ok;
                return false;
            }
        }
    }
    if (display) {
        tl.back()->setText(QString::number(rounds));
    }
    else {
        rounds = tl.back()->text().toUInt(ok);
        if (!*ok || rounds < 1) {
            cout << "Error: rounds number should be positive integer." << endl;
            delete ok;
            return false;
        }
    }
    delete ok;
    return true;
}

bool FcTimeParams::save(const vector<QLineEdit *> &tl, const QString &workDir) {
    bool ret = update(tl, false);
    if (ret) {
        QString fileName = QFileDialog::getSaveFileName(parent, 
                "Save protocal", workDir, "Text files (*.txt)");
        ofstream of(fileName.toStdString(), ios::out);
        if (of.is_open()) {
            of << "explore time = " << timeParams["explore"] << endl;
            of << "cue time = " << timeParams["cue"] << endl;
            of << "shock time = " << timeParams["shock"] << endl;
            of << "iter-trial interval = " << timeParams["iti"] << endl;
            of << "rest time = " << timeParams["rest"] << endl;
            of << "rounds = " << rounds << endl;
            of.close();
            return true;
        }
        else {
            cout << "Error: unable to open file for saving." << endl;
            return false;
        }
    }
    else {
        return false;
    }
}

bool FcTimeParams::load(const vector<QLineEdit *> &tl, const QString &workDir) {
    QString fileName = QFileDialog::getOpenFileName(parent, 
            "Load protocal", workDir, "Text files (*.txt)");
    ifstream pf(fileName.toStdString(), ios::in);
    vector<double> times(5, 0);
    double num;
    int r, count = 0;
    string name;
    if (pf.is_open()) {
        while (pf >> name) {
            if (name == "explore") {
                if (pf >> name >> name && pf >> num) {
                    times[0] = num;
                }
            }
            else if (name == "cue") {
                if (pf >> name >> name && pf >> num) {
                    times[1] = num;
                }
            }
            else if (name == "shock") {
                if (pf >> name >> name && pf >> num) {
                    times[2] = num;
                }
            }
            else if (name == "iter-trial") {
                if (pf >> name >> name && pf >> num) {
                    times[3] = num;
                }
            }
            else if (name == "rest") {
                if (pf >> name >> name && pf >> num) {
                    times[4] = num;
                }
            }
            else if (name == "rounds") {
                pf >> name && pf >> r;
            }
            else {
                cout << "Error: unknown format." << endl;
                return false;
            }
            count++;
        }
        if (count < 6) {
            cout << "Error: not enough parameters in file." << endl;
            return false;
        }
        for (vector<double>::iterator it = times.begin();
                it != times.end(); it++) {
            if ((*it) < 0) {
                cout << "Error: time parameters should be positive." << endl;
                return false;
            }
        }
        if (r < 0) {
            cout << "Error: round number should be positive." << endl;
            return false;
        }
        timeParams["explore"] = times[0];
        timeParams["cue"] = times[1];
        timeParams["shock"] = times[2];
        timeParams["iti"] = times[3];
        timeParams["rest"] = times[4];
        rounds = r;
        return update(tl, true);
    }
    else if (fileName.size() != 0) {
        cout << "Error: unable to open file." << endl;
        return false;
    }
}
