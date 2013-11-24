/**
* Project: Replication Project: Intro. to Optimization
* Description: A Branch and Bound Implementation for Control
*				of Traffic Lights in Intersections.
* Author: Bladymir Tellez
*/
#include "GlobalIncludes.h"
#include "CLIDriver.h"

using namespace std;

CLIDriver driver("input.txt", "result.txt"); // Unused
vector<Vehicle>* roadList[2]; // Stores the Sample Input
priority_queue<Configuration*, vector<Configuration*>, ConfigComparator> activeNodes;


int main() {
	for ever {
		driver.showOptions();
		driver.askInput();
		if (driver.stop()) break;
		driver.doTask();
	}

	// Keep the terminal open.
	system("PAUSE");
}