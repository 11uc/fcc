/*
Controll raspberry pie GPIO signal
Author: Chenghao Liu
Create: 2019/06/03
Modified: 2019/06/03
*/

#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class GPIO {  // control class for a single pin
	public:
		// Initialize with pin id and input/output mode
		GPIO(int pinId, IOmode mode);
		// Shut down pin
		~GPIO();
		int setMode(IOmode mode);
		int write(int value);
		int read();
		int getPinId();
		enum IOmode : int {in, out};
	protected:
		int pid;
		IOmode md;
		void exportGPIO();
		void unexportGPIO();
};
