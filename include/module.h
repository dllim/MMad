#pragma once
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include "processor.h"
#include "moduleOperations.h"

using namespace std;

class Module
{
public:
	Module();
	~Module();
	Module(string name, string type);

	void setInputFrom(string name, map<string, shared_ptr<class Module>> &map);
	void setOutputTo(string name, map<string, shared_ptr<class Module>> &map);

	string getModName() { return modName; };
	string getModType() { return modType; };
	
	bool isFirstModule = false;
	bool isLastModule = false;
	bool isConnectedToNetwork = false;

	bool hasNoInputs();
	bool hasNoOutputs();

	void process(string input, vector<string> &processorOutputBuffer, int numLastModules);
	void resetPreviousInput();

	vector<string> holdingBuffer = {};

private:

	string modName;
	string modType;
	vector<shared_ptr<Module>> inputFrom = {}; /* keeps track of the input connections to the module in the order they were made */
	vector<shared_ptr<Module>> outputTo = {}; /* keeps track of the output connections from the module in the order they were made */

	vector<string> inputBuffer = {};
	vector<string> outputBuffer = {};
	vector<string> previousInputBuffer = { "hello" };
};

