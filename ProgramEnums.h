#ifndef CURRENT_SEL_ENUMS
#define CURRENT_SEL_ENUMS

enum CURRENT_SELECTION { WAIT, BRANCH_BOUND, SET_INPUT, SET_OUTPUT, SHOW_SETTINGS, QUIT};
/**
* This struct represents a single vehicle, and encapsulate the information about the vehicle.
**/
struct Vehicle {
	unsigned int road;
	unsigned int arrival;
	unsigned int order;
};

#endif