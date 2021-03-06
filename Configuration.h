#ifndef CONFIGURATION_CLASS
#define CONFIGURATION_CLASS
#include "GlobalIncludes.h"


/**
 * A Configuration Represents a choice at any given time.
 */
class Configuration
{
private:	
	// Cache Variables
	int _timeT;
	int _waitTime;
	int _lambda;
	int _carsLeftR1;
	int _carsLeftR2;
	int _lowerBound;
	Configuration * _sameRoad;
	Configuration * _diffRoad;
	

public:
	vector<Vehicle> * road1; // The original order of vehicles on road 1
	vector<Vehicle> * road2; // the original order of vehicle on road 2
	static const int CHANGE_TIME = 3;
	static const int PASS_TIME   = 2;
	unsigned int indexRoad;		// last  considered.
	unsigned int indexCarR1;	// last index on Road1
	unsigned int indexCarR2;	// last index on Road2
	bool isRoadDiff;			// Is the current road different than the last road considered.
	Configuration * parent;		// The configuration before this one
	vector<Vehicle> ordering;
	

	Configuration(vector<Vehicle>*, vector<Vehicle>*);
	~Configuration(void);
	
	// Branch on Same Road
	Configuration * continueOnSameRoad();
	
	// Branch on a Different Road
	Configuration * continueOnDifferentRoad();
	
	// Calculate our Lower Bound Given this particular configuration!
	unsigned int calculateLowerBound();

	// Retreive the Cached lowerBound, must call calculate Lower Bound, before this!
	unsigned int lowerBound() const;
	
	unsigned int getTimeT();
	int Configuration::getTimeLambda();
	unsigned int getWaitTimeCurrentCar();

	// Other Helper Methods:
	int Configuration::getLeftOnRoad(int);
	bool isSolution();
};



/**
 * Comparator for our configurations.
 */
class ConfigComparator {
private:
	bool reverse;
public:
	ConfigComparator();
	~ConfigComparator();
	bool operator () (const Configuration * lhs, const Configuration * rhs) const;
};
#endif