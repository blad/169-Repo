#include "GlobalIncludes.h"
#include "CLIDriver.h"

CLIDriver::CLIDriver(string inFile, string outFile)
{
	this->selection = 0;
	this->inputFile = inFile;
	this->outputFile = outFile;
	this->current = CURRENT_SELECTION::WAIT;
	cout << "Branch and Bound Solver\n\n"
		<< "Control of Traffic Lights in Intersection"
		<< endl;
}



void CLIDriver::showOptions() {
	cout /*<< "1. Specify Input File\n"
		<< "2. Specify Output File\n"
		<< "3. Show Current Settings\n"*/
		<< "1. Run Branch & Bound On Input\n" 
		<< "0. Quit Program\n"
		<< endl;
}



bool CLIDriver::stop() {
	return (this->current == CURRENT_SELECTION::QUIT);
}



void CLIDriver::askInput() {
	for ever {
		cin >> this->selection;
		if (!cin){			
			cin.sync(); cin.clear();
			cout << "Invalid input, try again." << endl;
			continue;
		} else {
			switch (this->selection)
			{
				case 0:
					this->current = CURRENT_SELECTION::QUIT;
					break;
				case 1:
					this->current = CURRENT_SELECTION::BRANCH_BOUND;
					break;
			}
			
			cin.ignore(99,'\n'); 
			cout << endl; 
			break;
		}
	}
}


void CLIDriver::doTask() {
	if (this->current == CURRENT_SELECTION::QUIT)
		return;
	
	// Run Branch and Bound Solver
	// Return to a wait State
	this->current = WAIT;
}

CLIDriver::~CLIDriver()
{
}