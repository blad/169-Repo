#include "GlobalIncludes.h"

void fillCarsListSimple(vector<Vehicle> * road1, vector<Vehicle> * road2) {
	Vehicle v1r1 = {1,0,1};
	Vehicle v2r1 = {1,4,2};
	Vehicle v3r1 = {1,10,3};
	Vehicle v4r1 = {1,15,4};
	Vehicle v5r1 = {1,21,5};

	Vehicle v1r2 = {2,2,1};
	Vehicle v2r2 = {2,8,2};
	Vehicle v3r2 = {2,15,3};
	Vehicle v4r2 = {2,22,4};

	

	road1->push_back(v1r1);
	road1->push_back(v2r1);
	road1->push_back(v3r1);
	road1->push_back(v4r1);
	road1->push_back(v5r1);

	road2->push_back(v1r2);
	road2->push_back(v2r2);
	road2->push_back(v3r2);
	road2->push_back(v4r2);

	/*cout << "road1 stores " << int(road1->size()) << " vehicles.\n";
	cout << "road2 stores " << int(road2->size()) << " vehicles.\n";*/
}