#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include "module.h"
#include <memory>

using namespace std;

class Processor
{

map<string, shared_ptr<class Module>> moduleMap{}; /* does not preserve insertion order */

public:
	Processor();
	~Processor();
	
	void addModule(string name, string operation);
	void connectModules(string name1, string name2);
	void processAll(vector<string> inputStrings);
	int numLastModules = 0;

private:

	void prepareForProcessing();
	void setModuleNetwork();
	void process(string input);
	bool checkForDelayModules();
	void checkAndLimitOutputStream(int numInputStrings);
	void printOutputStream();

	vector<string> inputStream;
	vector<string> outputStream;
	vector<shared_ptr<Module>> inputTo = {};
	vector<shared_ptr<Module>> outputFrom = {};
	vector<string> orderedModulesDefinition = {}; /* helps track order in which modules are defined. no duplicates */
	vector<string> orderedModulesConnection = {}; /* helps track order in which modules are connected. no duplicates */
	bool areAllHoldingBuffersEmpty();

};





