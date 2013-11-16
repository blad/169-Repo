class BranchandBoundforIntersection{
private:
	int carsInRoad1[10]; //maximum 10 cars in road containing arrival time
	int carsInRoad2[10];
	int r1,r2; //r1=road1, r2=road2
	int n1, n2; // n1=# of cars in road1, n2=#of cars in road2 
	int q1, q2; // q1=# of cars passed road1, q2=#of cars passed road2
	int passTime; // time to pass road, default 2s
	int changeTime; // time to change light, default 3s
	int waitingTime; // waiting time for a car 
	int T; // spending time for q1, q2 cars pass the road
	int lb; // lower bound
	int firstBestLB; // first best so far LB
	int currentRoad; // indicating current road where cars passing
	int currentCar;// indicating current car passing the road
	int nextCar; //indicating next passing car after current car
	int [] activeNode; // node which do not have a complete solution in other branches
	int entireActiveNode; //#of activeNode
	int *carPassingSequence; // to store cars passing sequence 

public:
	BranchandBoundforIntersection(){}
	~BranchandBoundforIntersection(){}
	int  LB(int  currentCar)
	{
		int C;
		if(n1!=q1||n2!=q2) C=changeTime;
		else C=0;
		lb=GetT(currentCar)+(n1+n2-q1-q2) * passTime +C;
		return lb;
	}


	int GetT(int currentCar)
	{
		int C;
		if(!isRoadChange) C=0; //if there is no light change
		else C=changTime; //if light change to another road 
		T = T + WaitingTime(currentCar, T) +passtime + C;
		return T;
	}



	int GetWaitingTime(int currentCar, int T)
	{
		int A=currentCar;
		if(!isRoadChange) waitingTime=Max(0, A- T);//if there is no light change
		else waitingTime=Max(0,A-T-changTime); // if there is light change
		return waitingTime;
	}


	void BranchandBoundforIntersection(int Num1, int Num2, int *Car1, int *Car2)
	{
		//input #s of car in each road and Arrival times
		n1=Num1;
		n2=Num2;
		carsInRoad1=Car1;
		carsInRoad2=Car2;
		//set root starting with road1 and first car in road1
		currentRoad=r1;
		currentCar=carsInRoad1[0];
		q1=1;
		q2=0;
		T=0;
	}
	


	void BranchandBound(int currentCar, int currentRoad)
	{
		int root=currentCar;//current car is carsInRoad1[0]
		lb=LB(root);
		Branching(root);
		//after first for-loop, get first best so far solution
		for(int i=2;i++;i<=(n1+n2-1))
		{
			nextCar=min(LB(CarinRoad1[q1]), LB(CarinRoad2[q2]));			   
			currentCar=nextCar;//chose smaller LB car as currentCar
			lb= LB(currentCar);	
			carPassingSequence[i]=currentCar;//store sequence
			Branching(currentCar);	
		}
		firstbestLB=lb;
		//compare best for far solution with another active node which not completed yet
		for(int j=0;j++;j<entireActiveNode)
		{
			currentCar=activeNode[j];
			//Here we need information about activeNode[j] route like q1, q2 and carPassingSequence until activeNode[j]
			Branching(currentCar);
			nextCar=min(LB(CarinRoad1[q1]), LB(CarinRoad2[q2]));	
			if(LB(nextCar)>=firstbestLB) prune this branch
			else 
			{
				for(int k=(n1+n2-q1-q2); k<(n1+n2-1); k++)
				{
					currentCar=nextCar;
					//keep branching but if found branch node LB is bigger than firstbestLB(the first best so far solution) then prune 
				}
			}	
		}
	}



	int Branching(int currentCar)
	{	
		//during get first best for far solution, store active nodes which we will branch later after we get first best for far solution)
		if(i<(n1+n2-1))
		{ 
			//active node is the one we do not branch during we get first best for far solution, but will branch later after we get first best for far solution
			activeNode[entireActiveNode]=max{ LB(CarinRoad1[q1]), LB(CarinRoad2[q2]));
			entireActiveNode++;

			if (LB(CarinRoad1[q1]) < LB(CarinRoad2[q2])) q1++;//update # of passing cars
			else q2++;
		} else {
			if(max{ LB(CarinRoad1[q1]), LB(CarinRoad2[q2]))<lb) {
				activeNode[entireActiveNode]=max{ LB(CarinRoad1[q1]), LB(CarinRoad2[q2]));
				entireActiveNode++;
			}
		}
	}
}  
