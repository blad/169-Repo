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
private:
	string inputFile;
	string outputFile;
	unsigned int selection;
	CURRENT_SELECTION current;
};

#endif