#ifndef CONFIGURATION_CLASS
#define CONFIGURATION_CLASS
#include "GlobalIncludes.h"
class Configuration
{

private:	
	// The lower bound of the courrent configuration.
	unsigned int value; // Value of solutions in this configuration
	vector<Vehicle> * road1; // The original order of vehicles on road 1
	vector<Vehicle> * road2; // the original order of vehicle on road 2

public:
	static const int CHANGE_TIME = 3;
	static const int PASS_TIME   = 2;
	unsigned int indexRoad; // last  considered.
	unsigned int indexCarR1; // last index on Road1
	unsigned int indexCarR2; // last index on Road2
	bool isRoadDiff;
	Configuration * parent; // The configuration before this one
	vector<Vehicle> ordering;
	

	Configuration(vector<Vehicle>*, vector<Vehicle>*);
	~Configuration(void);
	Configuration continueOnSameRoad();
	Configuration continueOnDifferentRoad();
	unsigned int lowerBound() const;
	unsigned int getTimeTCurrentCar() const;
	unsigned int getWaitTimeCurrentCar() const;
};


class ConfigComparator {
private:
	bool reverse;
public:
	ConfigComparator();
	~ConfigComparator();
	bool operator () (const Configuration & lhs, const Configuration & rhs) const;
};
#endif