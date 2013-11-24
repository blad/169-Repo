#include "GlobalIncludes.h"


/**
 * This comparator will be used to order our Queue during our Branch and Bound search.
 */ 
ConfigComparator::ConfigComparator() {}
ConfigComparator::~ConfigComparator() {}
bool ConfigComparator::operator () (const Configuration * lhs, const Configuration * rhs) const {
	return (lhs->lowerBound() > rhs->lowerBound());
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
	this->_carsLeftR1 = -1;
	this->_carsLeftR2 = -1;
	this->_lambda     = -1;
	this->_timeT      = -1;
	this->_waitTime	  = -1;
	this->_lowerBound = -1;
	this->_diffRoad   = NULL;
	this->_sameRoad   = NULL;
}



/**
 * Create a new configuration based on this one, but continuing on the same road.
 */
Configuration * Configuration::continueOnSameRoad() {
	if (this->_sameRoad) 
		return this->_sameRoad;

	this->_sameRoad = new Configuration(road1, road2);
	this->_sameRoad->parent = this;
	this->_sameRoad->indexRoad = this->indexRoad;

	if (this->indexRoad) { // Index of road = 1
		this->_sameRoad->indexCarR2 = this->indexCarR2 + 1;
		this->_sameRoad->indexCarR1 = this->indexCarR1;
	} else {
		this->_sameRoad->indexCarR1 = this->indexCarR1 + 1;
	  this->_sameRoad->indexCarR2 = this->indexCarR2;
	}

	return this->_sameRoad;
}



/**
 * Create a new configuration based on this one, but continuing on a different road.
 */
Configuration * Configuration::continueOnDifferentRoad() {
	if (this->_diffRoad) 
		return this->_diffRoad;

	this->_diffRoad = new Configuration(road1, road2);
	this->_diffRoad->parent = this;
	this->_diffRoad->isRoadDiff = true;
	this->_diffRoad->indexRoad = (this->indexRoad == 1) ? 0 : 1;
  
	if (this->indexRoad == 1) {
	    this->_diffRoad->indexCarR1 = this->indexCarR1;
	    this->_diffRoad->indexCarR2 = this->indexCarR2 + 1;
	} else {
		this->_diffRoad->indexCarR2 = this->indexCarR2;
	    this->_diffRoad->indexCarR1 = this->indexCarR1 + 1;
	}

	return this->_diffRoad;
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
	accumulator += this->getTimeT();
	
	accumulator += remainInR1 * Configuration::PASS_TIME;
	accumulator += remainInR2 * Configuration::PASS_TIME;
	accumulator += this->getTimeLambda();

	// Uncomment to see the variable values at each step.
	//cout << "Remaining: " << remainInR1 << " in road 1 | " << remainInR2 << " in road 2" << endl;
	//cout << "Time T: " << this->getTimeT() << endl;
	//cout << "Passing Time: " << (remainInR1 + remainInR2) * Configuration::PASS_TIME << endl;
	//cout << "Waiting Time:" << this->getWaitTimeCurrentCar() << endl;
	//cout << "Lambda: " << getTimeLambda() << endl;

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
	
	if (this->parent && this->parent != this)
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

	int currentCarIndex = this->indexRoad ? this->indexCarR2 : this->indexCarR1;
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
	
	if (road->size() == index) {
		if (rIndex % 2) this->_carsLeftR1 = 0;
		else this->_carsLeftR2 = 0;
		return 0;
	} else {
		if (rIndex % 2) this->_carsLeftR1 = (road->size() - index);
		else this->_carsLeftR2 = (road->size() - index);
		return (road->size() - index);
	}
}



bool Configuration::isSolution() {
	if(getLeftOnRoad(1) > 0 && getLeftOnRoad(2) > 0)
		return false;
	else
		return true;
}



/**
 * Desctructor! Evil Genius Code Goes Here...
 */
Configuration::~Configuration() {}
