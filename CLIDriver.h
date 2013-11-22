#ifndef CLIDRIVER_CLASS
#define CLIDRIVER_CLASS

class CLIDriver
{
public:
	CLIDriver(string, string);
	~CLIDriver();
	void showOptions();
	void askInput();
	void doTask();
	bool stop();
	
	// Shows how the Configurations Are Used.
	void doSimpleConfigurationExample();

	// Executed Branch And Bound on Sample Input
	void doBranchAndBoundSample();
private:
	string inputFile;
	string outputFile;
	unsigned int selection;
	CURRENT_SELECTION current;
};

#endif