#include "GlobalIncludes.h"
#include "CLIDriver.h"

extern vector<Vehicle>* roadList[2]; // Defined in our Main.cpp
extern priority_queue<Configuration*, vector<Configuration*>, ConfigComparator> activeNodes;
Configuration * curBest; // Holds the best Solution.
const char CLIDriver::ROAD_INDEX_RESULT_STRING[] = {"Car Index: %d\t From Road: %d \n"};

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
	cout << endl 
		 << "> 1. Run Simple Example" << endl
		 << "> 2. Run Branch & Bound On Input" << endl
		 << "> 0. Quit Program" << endl;
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


	Configuration root(roadList[0], roadList[1]);
	root.indexRoad = 0; // Road 1
	root.indexCarR1 = 0; // Car1 on Road 1
	root.indexCarR2 = 0; // Car1 on Road 2
	root.calculateLowerBound();
	int lb = root.lowerBound();
	cout << "Lower Bound for Root (Road 1): " << lb << endl << endl;

	// -----------------------

	Configuration * left = root.continueOnSameRoad();
	left->calculateLowerBound();
	lb = left->lowerBound();
	cout << "Lower Bound for Root->Road1: " << lb << endl << endl;

	Configuration * right = root.continueOnDifferentRoad();
	right->calculateLowerBound();
	lb = right->lowerBound();
	cout << "Lower Bound for Root->Road2: " << lb << endl << endl;

	// -----------------------

	Configuration * leftleft = left->continueOnSameRoad();
	leftleft->calculateLowerBound();
	lb = leftleft->lowerBound();
	cout << "Lower Bound for Root->Road1->Road1: " << lb << endl << endl;

	Configuration * leftright = left->continueOnDifferentRoad();
	leftright->calculateLowerBound();
	lb = leftright->lowerBound();
	cout << "Lower Bound for R1->R1->R2: " << lb << endl << endl;


	// ------------------------

	Configuration * leftrightleft = leftright->continueOnSameRoad();
	leftrightleft->calculateLowerBound();
	lb = leftrightleft->lowerBound();
	cout << "Lower Bound for R1->R1->R2->R2: " << lb << endl << endl;


	Configuration * leftrightright = leftright->continueOnDifferentRoad();
	leftrightright->calculateLowerBound();
	lb = leftrightright->lowerBound();
	cout << "Lower Bound for R1->R1->R2->R1: " << lb << endl << endl;

}



void CLIDriver::doBranchAndBoundSample() {
	cout << endl << "RUNNING BRANCH AND BOUND ON SAMPLE DATA" << endl;
	roadList[0] = new vector<Vehicle>; // road 1 
	roadList[1] = new vector<Vehicle>; // road 2
	fillCarsListSimple(roadList[0], roadList[1]); // Hard Coded Sample Values.

	Configuration * root =  new Configuration(roadList[0], roadList[1]);
	root->indexRoad = 0;  // Road 1
	root->indexCarR1 = 0; // Car1 on Road 1
	root->indexCarR2 = 0; // Car1 on Road 2
	root->calculateLowerBound();

	int time = root->lowerBound();
	curBest = root;
	curBest = this->branching(curBest);

	cout << endl << "Best Solution from Root Node:" << curBest->lowerBound() << endl << endl;

	// Explore Other Nodes & Pruned Potentially worse solutions.
	while (!activeNodes.empty()){
		Configuration * next = activeNodes.top();
		next->calculateLowerBound();
		if (next->lowerBound() < curBest->lowerBound()) {
			curBest = this->branching(next);
		} // Branch is Pruned otherwise
		activeNodes.pop();
	}

	cout << "The Best Solution Of All: " << curBest->lowerBound() << endl;
	
	// Display the Best Solution:
	Configuration * res = curBest;
	for (int i = res->getLeftOnRoad(1); i > 0; i--) {
		printf(CLIDriver::ROAD_INDEX_RESULT_STRING, (res->indexCarR1 +  i), 1);
	}

	for (int i = res->getLeftOnRoad(2); i > 0; i--) {
		printf(CLIDriver::ROAD_INDEX_RESULT_STRING, (res->indexCarR2 +  i), 2);
	}

	while (res->parent) {
		printf(CLIDriver::ROAD_INDEX_RESULT_STRING, (res->indexRoad ? res->indexCarR2 : res->indexCarR1), (res->indexRoad+1));
		res = res->parent;
	}
	printf(CLIDriver::ROAD_INDEX_RESULT_STRING, (res->indexRoad ? res->indexCarR2 : res->indexCarR1), (res->indexRoad+1));
}

Configuration * CLIDriver::branching(Configuration * node) {
	Configuration * left;
	Configuration * right;
	while (!node->isSolution()) {
		int leftVal, rightVal;
		bool canExploreSameRoad, canExploreDifferentRoad; 
		int rIndex = node->indexRoad;
		canExploreSameRoad      =  rIndex    % 2 ? (node->indexCarR1 + 1) < node->road1->size() : (node->indexCarR2 + 1) < node->road2->size();
		canExploreDifferentRoad = (rIndex+1) % 2 ? (node->indexCarR1 + 1) < node ->road1->size() : (node->indexCarR2 + 1) < node->road2->size();

		if (canExploreSameRoad) {
			left = node->continueOnSameRoad();
			left->calculateLowerBound();
			leftVal = left->lowerBound();
		} else {
			left = NULL;
			leftVal = INT_MAX;
		}

		if (canExploreDifferentRoad) {
			right = (node->continueOnDifferentRoad());
			right->calculateLowerBound();
			rightVal = right->lowerBound();
		} else {
			right = NULL;
			rightVal = INT_MAX;
		}

		if (leftVal < rightVal){
			node = left;
			if (right) activeNodes.push(right);
		} else {
			node = right;
			if (left) activeNodes.push(left);
		}
	}

	if ((node->lowerBound() < curBest->lowerBound() || !curBest->isSolution())  &&  node->isSolution())
		curBest = node;

	return curBest;
}


void CLIDriver::doTask() {
	switch (this->current){
	case CURRENT_SELECTION::QUIT:
		return;
	case  CURRENT_SELECTION::SAMPLE_CONFIGURATION:
		this->doSimpleConfigurationExample();	
		break;
	case CURRENT_SELECTION::BRANCH_BOUND:
		this->doBranchAndBoundSample();
		break;
	default:
		// Do Nothing....
		break;
	}

	// Return to a wait State
	this->current = WAIT;
}

CLIDriver::~CLIDriver()
{
}