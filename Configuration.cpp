#include "GlobalIncludes.h"



ConfigComparator::ConfigComparator() {}
ConfigComparator::~ConfigComparator() {}
bool ConfigComparator::operator () (const Configuration & lhs, const Configuration & rhs) const {
	return (lhs.lowerBound() > rhs.lowerBound());
}

Configuration::Configuration(vector<Vehicle> * r1, vector<Vehicle> * r2) {
	this->road1 = r1;
	this->road2 = r2;
	this->isRoadDiff = false;
	// -1 indicates that there have been no considerations before this
	this->indexRoad = -1;
	this->indexCarR1 = -1;
	this->indexCarR2 = -1;
}

Configuration Configuration::continueOnSameRoad() {
	Configuration sameRoad(road1, road2);
	sameRoad.parent = this;
	sameRoad.indexRoad = this->indexRoad;

	// Determine which road to take a car from
	if (sameRoad.indexCarR1 % 2)
		sameRoad.indexCarR1 = this->indexCarR1 + 1;
	else
		sameRoad.indexCarR2 = this->indexCarR2 + 1;

	return sameRoad;
}


Configuration Configuration::continueOnDifferentRoad() {
	Configuration diffRoad(road1, road2);
	diffRoad.parent = this;
	diffRoad.isRoadDiff = true;
	diffRoad.indexRoad = (this->indexRoad + 1) % 2; // Assign the opposite road.

	// Determine which road to take a car from
	if (diffRoad.indexCarR1 % 2)
		diffRoad.indexCarR2 = this->indexCarR2 + 1;
	else
		diffRoad.indexCarR1 = this->indexCarR1 + 1;

	return diffRoad;
}

unsigned int Configuration::lowerBound() const {
	unsigned int accumulator = 0; 
	accumulator += getTimeT();
	int remainintR1 = this->road1->size() - this->indexCarR1 + 1;
	int remainintR2 = this->road2->size() - this->indexCarR2 + 1;
	accumulator += remainintR1 * Configuration::PASS_TIME;
	accumulator += remainintR2 * Configuration::PASS_TIME;
	
	// Calculate the change time depending on the 
	// number or remaing cars in a diffrent road.

	return 0;
}

unsigned int Configuration::getTimeT() const {
	unsigned int accumulator = 0;
	
	accumulator += parent->getTimeT();
	if (this->isRoadDiff)
		accumulator += Configuration::CHANGE_TIME;
	accumulator += Configuration::PASS_TIME;
	accumulator += this->getWaitTimeCurrentCar();

	return accumulator;
}


unsigned int Configuration::getWaitTimeCurrentCar() const {
	int waitTime = 0;
	unsigned int currentCarIndex = this->indexRoad ? this->indexCarR2+1 : this->indexCarR1+1;
	vector<Vehicle> * currentRoad = this->indexRoad ? this->road2 : this->road1;
	int arrivalTime = currentRoad->at(currentCarIndex).arrival;
	if (this->isRoadDiff){
		waitTime = max(0, (arrivalTime - (int)parent->getTimeT() -  Configuration::CHANGE_TIME));
	} else {
		waitTime = max(0, (arrivalTime - (int)parent->getTimeT()));
	}

	return waitTime;
}
//int  Configuration::LB(int  currentCar)
//{
//	int C;
//	if(n1!=q1||n2!=q2) C=changeTime;
//	else C=0;
//	lb=GetT(currentCar)+(n1+n2-q1-q2) * passTime +C;
//	return lb;
//}

//int Configuration::GetT(int currentCar)
//{
//	int C;
//	if(!isRoadChange) 
//		C=0; //if there is no light change
//	else 
//		C=changTime; //if light change to another road 
//	T = T + WaitingTime(currentCar, T) +passtime + C;
//	return T;
//}

//int Configuration::GetWaitingTime(int currentCar, int T)
//{
//	int A=currentCar;
//	if(!isRoadChange) waitingTime=Max(0, A- T);//if there is no light change
//	else waitingTime=Max(0,A-T-changTime); // if there is light change
//	return waitingTime;
//}

Configuration::~Configuration() {}