#include "GlobalIncludes.h"


/**
 * This comparator will be used to order our Queue during our Branch and Bound search.
 */ 
ConfigComparator::ConfigComparator() {}
ConfigComparator::~ConfigComparator() {}
bool ConfigComparator::operator () (const Configuration & lhs, const Configuration & rhs) const {
	return (lhs.lowerBound() > rhs.lowerBound());
}



/**
 * Constructor for each configuration. Sets default values.
 */
Configuration::Configuration(vector<Vehicle> * r1, vector<Vehicle> * r2) {
	this->road1      = r1;
	this->road2		 = r2;
	this->isRoadDiff = false;
	
	// -1 indicates that there have been no considerations before this
	this->indexRoad  = -1;
	this->indexCarR1 = -1;
	this->indexCarR2 = -1;
	this->parent	 = NULL;

	// Negative Value means not evaluated yet.
	this->_carsLeft   = -1;
	this->_lambda     = -1;
	this->_timeT      = -1;
	this->_waitTime	  = -1;
	this->_lowerBound = -1;
}



/**
 * Create a new configuration based on this one, but continuing on the same road.
 */
Configuration Configuration::continueOnSameRoad() {
	Configuration sameRoad(road1, road2);
	sameRoad.parent = this;
	sameRoad.indexRoad = this->indexRoad;

	if (this->indexRoad) { // Index of road = 1
		sameRoad.indexCarR2 = this->indexCarR2 + 1;
		sameRoad.indexCarR1 = this->indexCarR1;
	} else {
		sameRoad.indexCarR1 = this->indexCarR1 + 1;
		sameRoad.indexCarR2 = this->indexCarR2;
	}

	return sameRoad;
}



/**
 * Create a new configuration based on this one, but continuing on a different road.
 */
Configuration Configuration::continueOnDifferentRoad() {
	Configuration diffRoad(road1, road2);
	diffRoad.parent = this;
	diffRoad.isRoadDiff = true;
	diffRoad.indexRoad = (this->indexRoad == 1) ? 0 : 1;
	
	if (this->indexRoad == 1) {
		diffRoad.indexCarR1 = this->indexCarR1;
		diffRoad.indexCarR2 = this->indexCarR2 + 1;
	} else {
		diffRoad.indexCarR2 = this->indexCarR2;
		diffRoad.indexCarR1 = this->indexCarR1 + 1;
	}

	return diffRoad;
}



/**
 * Retreive the cached lowerBound of the current Configuration.
 */
unsigned int Configuration::lowerBound() const {
	return this->_lowerBound;
}



/**
 * Calculate the lower bound estimate for this configuration.
 * Calculation based on Formulation #2 from "Control of Traffic Lights in Intersectoins...."
 */
unsigned int Configuration::calculateLowerBound() {
	int remainInR1 = this->getLeftOnRoad(1);
	int remainInR2 = this->getLeftOnRoad(2);
	
	unsigned int accumulator = 0; 
	accumulator += getTimeT();
	
	accumulator += remainInR1 * Configuration::PASS_TIME;
	accumulator += remainInR2 * Configuration::PASS_TIME;
	accumulator += this->getTimeLambda();

	// Uncomment to see the variable values at each step.
	/*cout << "Remaining: " << remainInR1 << " in road 1 | " << remainInR2 << " in road 2" << endl;
	cout << "Time T: " << getTimeT() << endl;
	cout << "Passing Time: " << (remainInR1 + remainInR2) * Configuration::PASS_TIME << endl;
	cout << "Waiting Time:" << this->getWaitTimeCurrentCar() << endl;
	cout << "Lambda: " << getTimeLambda() << endl;*/

	this->_lowerBound = accumulator; // Cache the Value
	return accumulator;
}



/**
 * Calculate variable T for the current vehicle.
 * Calculation based on Formulation #4 from "Control of Traffic Lights in Intersectoins...."
 */
unsigned int Configuration::getTimeT() {
	if (this->_timeT >= 0)
		return this->_timeT;

	unsigned int accumulator = 0;
	
	if (this->parent)
	    accumulator += this->parent->getTimeT();

	if (this->isRoadDiff)
		accumulator += Configuration::CHANGE_TIME;

	accumulator += Configuration::PASS_TIME;
	accumulator += this->getWaitTimeCurrentCar();

	this->_timeT = accumulator;
	return accumulator;
}



/**
 * Lambda
 * Calculation based on Formulation #3 from "Control of Traffic Lights in Intersectoins...."
 */
int Configuration::getTimeLambda() {
	if (this->_lambda >= 0)
		return this->_lambda; // Return a cached value for future requests.

	int leftOnDiffRoad = this->indexRoad ? this->getLeftOnRoad(2) : this->getLeftOnRoad(1) ;
	if (leftOnDiffRoad == 0) {
		this->_lambda = 0; // Cache the calculation
		return 0;
	} else {
		this->_lambda = Configuration::CHANGE_TIME; // Cache the calculation
		return Configuration::CHANGE_TIME;
	}
}



/**
 * Wait Time for the current vehicle.
 * Calculation based on Formulation #5 from "Control of Traffic Lights in Intersectoins...."
 */
unsigned int Configuration::getWaitTimeCurrentCar() {
	if (this->_waitTime >= 0)
		return this->_waitTime; // Return a cached value for future requests

	unsigned int currentCarIndex = this->indexRoad ? this->indexCarR2 : this->indexCarR1;
	vector<Vehicle> * currentRoad = this->indexRoad ? this->road2 : this->road1;
	int arrivalTime = currentRoad->at(currentCarIndex).arrival;	
	int waitTime = 0;
	if (this->parent) {
		if (this->indexRoad != this->parent->indexRoad){
			waitTime = max(0, (arrivalTime - (int)parent->getTimeT() -  Configuration::CHANGE_TIME));
		} else {
			waitTime = max(0, (arrivalTime - (int)parent->getTimeT()));
		}
	}

	this->_waitTime = waitTime;
	return waitTime;
}



/**
 * Helper Method to return the number of vehicles left on a particular road.
 * Note this method used real index (1 or 2) not (0 or 1).
 */
int Configuration::getLeftOnRoad(int rIndex) {
	vector<Vehicle> * road = (rIndex % 2) ? this->road1 : this->road2;
	int index = (rIndex % 2) ? this->indexCarR1 : this->indexCarR2;
	index = this->indexRoad != (rIndex % 2) ? index + 1 : index;
	
	/*cout << "Road " << rIndex << " Index for Left On Road: " << index << endl;
	cout << "Road " << rIndex << " Size Road: " << road->size() << endl;
	cout << "Current Config Road Index" << this->indexRoad << endl << endl;*/
	if (road->size() == index + 1) {
		return 0;
	} else {
		return (road->size() - index);
	}
}



/**
 * Desctructor! Evil Genius Code Goes Here...
 */
Configuration::~Configuration() {}
