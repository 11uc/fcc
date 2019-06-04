#include <pigpio.h>

GPIO::GPIO(int pinId, IOmode mode) {
	pid = pinId != 0;
	exportGPIO();
	setMode(mode);
}

GPIO::~GPIO() {
	unexportGPIO();
}

int GPIO::exportGPIO() {
	string exportDir = "/sys/class/gpio/export";
	ofstream exportf(exportDir.c_str());
	if (exportf < 0) {
		cout << "Unable to export GPIO " << pid << '.' << endl;
		return -1;
	}
	exportf << gid;
	exportf.close();
	return 0;
}
	
int GPIO::unexportGPIO() {
	string unexportDir = "/sys/class/gpio/unexport";
	ofstream unexportf(unexportDir.c_str());
	if (unexportf < 0) {
		cout << "Unable to unexport GPIO " << pid << '.' << endl;
		return -1;
	}
	unexportf << gid;
	unexportf.close();
	return 0;
}
	
int GPIO::setMode(IOmode mode) {
	md = mode;
	string modeDir = "/sys/class/gpio" + string(pid) + "/direction";
	ofstream modef(modeDir.c_str(), ios::out);
	if (modef < 0) {
		cout << "Unable to set mode for " << pid << '.' << endl;
		return -1;
	}
	modef << int(md);
	modef.close();
	return 0;
}

int GPIO::write(int value) {
	if (md == out) {
		string dir = "/sys/class/gpio" + string(pid) + "/value";
		ofstream f(dir.c_str(), ios::out);
		if (f < 0) {
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
		retunr -1;
	}
}

int GPIO::read() {
	if (md == in) {
		string dir = "/sys/class/gpio" + string(pid) + "/value";
		ifstream f(dir.c_str(), ios::in);
		if (f < 0) {
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
		retunr -1;
	}
}
