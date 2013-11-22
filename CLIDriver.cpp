#include "GlobalIncludes.h"
#include "CLIDriver.h"

extern vector<Vehicle>* roadList[2]; // Defined in our Main.cpp

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
	cout << "1. Run Simple Example" << endl
		 << "2. Run Branch & Bound On Input" << endl
		 << "0. Quit Program" << endl;
}



bool CLIDriver::stop() {
	return (this->current == CURRENT_SELECTION::QUIT);
}



void CLIDriver::askInput() {
	for ever {
		cout << "Your Choice >> ";
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
				this->current = CURRENT_SELECTION::SAMPLE_CONFIGURATION;
				break;
			case 2:
				this->current = CURRENT_SELECTION::BRANCH_BOUND;
				break;
			}

			cin.ignore(99,'\n'); 
			cout << endl; 
			break;
		}
	}
}

void CLIDriver::doSimpleConfigurationExample() {
	cout << endl << "RUNNING EXAMPLE CONFIGURATION" << endl;
	
	roadList[0] = new vector<Vehicle>; // road 1 
	roadList[1] = new vector<Vehicle>; // road 2
	fillCarsListSimple(roadList[0], roadList[1]); // Hard Coded Sample Values.

	//priority_queue<Configuration, vector<Configuration>, ConfigComparator> activeNodes;

	Configuration root(roadList[0], roadList[1]);
	root.indexRoad = 0; // Road 1
	root.indexCarR1 = 0; // Car1 on Road 1
	root.indexCarR2 = 0; // Car1 on Road 2
	root.calculateLowerBound();
	int lb = root.lowerBound();
	cout << "Lower Bound for Root (Road 1): " << lb << endl << endl;

	Configuration left = root.continueOnSameRoad();
	left.calculateLowerBound();
	lb = left.lowerBound();
	cout << "Lower Bound for Root->Road1: " << lb << endl << endl;

	Configuration right = root.continueOnDifferentRoad();
	right.calculateLowerBound();
	lb = right.lowerBound();
	cout << "Lower Bound for Root->Road2: " << lb << endl << endl;

	// -----------------------

	Configuration leftleft = left.continueOnSameRoad();
	leftleft.calculateLowerBound();
	lb = leftleft.lowerBound();
	cout << "Lower Bound for Root->Road1->Road1: " << lb << endl << endl;

	Configuration leftright = left.continueOnDifferentRoad();
	leftright.calculateLowerBound();
	lb = leftright.lowerBound();
	cout << "Lower Bound for Root->Road1->Road2: " << lb << endl << endl;


	Configuration leftrightleft = leftright.continueOnSameRoad();
	leftrightleft.calculateLowerBound();
	lb = leftrightleft.lowerBound();
	cout << "Lower Bound for Root->Road1->Road2->Road2: " << lb << endl << endl;


	Configuration leftrightright = leftright.continueOnDifferentRoad();
	leftrightright.calculateLowerBound();
	lb = leftrightright.lowerBound();
	cout << "Lower Bound for Root->Road1->Road2->Road1: " << lb << endl << endl;

	cout << endl << endl;

}



void CLIDriver::doTask() {
	switch (this->current){
		case CURRENT_SELECTION::QUIT:
			return;
		case  CURRENT_SELECTION::SAMPLE_CONFIGURATION:
			this->doSimpleConfigurationExample();	
			break;
		default:
			// Do Nothing....
			break;
	}
	

	// Run Branch and Bound Solver
	// Return to a wait State
	this->current = WAIT;
}

CLIDriver::~CLIDriver()
{
}