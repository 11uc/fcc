#include "pigpio.h"

GPIO::GPIO(int pinId, IOmode mode, int rest) {
	pid = pinId;
	exportGPIO();
	setMode(mode);
    if (mode == out)
        write(rest);
}

GPIO::~GPIO() {
	unexportGPIO();
}

int GPIO::exportGPIO() {
	string exportDir = "/sys/class/gpio/export";
	ofstream exportf(exportDir.c_str());
	if (!exportf.is_open()) {
		cout << "Unable to export GPIO " << pid << '.' << endl;
		return -1;
	}
	exportf << pid;
	exportf.close();
	return 0;
}
	
int GPIO::unexportGPIO() {
	string unexportDir = "/sys/class/gpio/unexport";
	ofstream unexportf(unexportDir.c_str());
	if (!unexportf.is_open()) {
		cout << "Unable to unexport GPIO " << pid << '.' << endl;
		return -1;
	}
	unexportf << pid;
	unexportf.close();
	return 0;
}
	
int GPIO::setMode(IOmode mode) {
	md = mode;
	string modeDir = "/sys/class/gpio/gpio" + to_string(pid) + \
        "/direction";
	ofstream modef(modeDir.c_str());
    int i;
    // It takes some time for the pin folder to be created
    // after export, so loop until it's found
	for (i = 0; i < 100000 && !modef.is_open(); i++) {
        modef.open(modeDir.c_str());
	}
    if (i == 100000) {
        cout << "Unable to set mode for " << pid << '.' << endl;
        return -1;
    }
    if (md == in) {
        modef << "in";
    }
    else {
        modef << "out";
    }
	modef.close();
	return 0;
}

int GPIO::write(int value) {
	if (md == out) {
		string dir = "/sys/class/gpio/gpio" + to_string(pid) + "/value";
		ofstream f(dir.c_str());
        int i;
        for (i = 0; i < 100000 && !f.is_open(); i++) {
            f.open(dir.c_str());
        }
        if (i == 100000) {
			cout << "Unable to set value for " << pid << '.' << endl;
			return -1;
        }
		f << int(value != 0);
		f.close();
		return 0;
	}
	else {
		cout << "Unable to set value in input mode for " <<
			pid << '.' << endl;
		return -1;
	}
}

int GPIO::read() {
	if (md == in) {
		string dir = "/sys/class/gpio/gpio" + to_string(pid) + "/value";
		ifstream f(dir.c_str());
		if (!f.is_open()) {
			cout << "Unable to get value for " << pid << '.' << endl;
			return -1;
		}
		int value = 0;
		f >> value;
		f.close();
		return 0;
	}
	else {
		cout << "Unable to set value in output mode for " <<
			pid << '.' << endl;
		return -1;
	}
}
