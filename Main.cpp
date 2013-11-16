/**
* Project: Replication Project: Intro. to Optimization
* Description: A Branch and Bound Implementation for Control
*				of Traffic Lights in Intersections.
* Author: Bladymir Tellez
*/
#include "GlobalIncludes.h"
#include "CLIDriver.h"

using namespace std;

CLIDriver driver("input.txt", "result.txt");
vector<Vehicle>* roadList[2];


int main() {
	roadList[0] = new vector<Vehicle>; // road 1 
	roadList[1] = new vector<Vehicle>; // road 2
	fillCarsListSimple(roadList[0], roadList[1]); // Hard Coded Sample Values.

	//Configuration x(&roadList[0], &roadList[1]);

	priority_queue<Configuration, vector<Configuration>, ConfigComparator> activeNodes;
	//activeNodes.push(x);
	//activeNodes.push(y);

	cout << "Highest Priority: " << activeNodes.top().lowerBound() << endl;

	for ever {
		driver.showOptions();
		driver.askInput();
		if (driver.stop()) break;
		driver.doTask();
	}

	// Keep the terminal open.
	system("PAUSE");
}