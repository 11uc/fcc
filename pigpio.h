/*
Controll raspberry pie GPIO signal
Author: Chenghao Liu
Create: 2019/06/03
Modified: 2019/06/03
*/

#pragma once
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class GPIO {  // control class for a single pin
	public:
		enum IOmode : int {in, out};
		// Initialize with pin id and input/output mode
        // and set to rest value if mode is out
		GPIO(int pinId, IOmode mode, int rest = 0);
		// Shut down pin
		~GPIO();
		int setMode(IOmode mode);
		int write(int value);
		int read();
		int getPinId();
	protected:
		int pid;
		IOmode md;
		int exportGPIO();
		int unexportGPIO();
};
