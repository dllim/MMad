#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include "../include/processor.h"
#include "../include/module.h"
#include <memory>

using namespace std;

Processor::Processor() {};
Processor::~Processor() {};

void Processor::addModule(string name, string operation)
{
	/* check if any modules with the same name already exist */
	for (auto &elem : moduleMap)
	{
		if (elem.second->getModName() == name)
		{
			cout << "A module of the same name already exists. Please try again with a different name for the module" << endl;
			return;
		}
	}

	/* if not, create the module and add it to the moduleMap map*/
	shared_ptr<Module> p = make_shared<Module>(name, operation);
	moduleMap.insert(make_pair(name, p));

	/* vector of strings which tracks the order in which modules are defined. There are no duplicates. */
	orderedModulesDefinition.push_back(name);
}

void Processor::connectModules(string name1, string name2)
{
	/* check if name1 already has been defined as a module */
	if (moduleMap.find(name1) == moduleMap.end())
	{
		cout << "There is no module named " << name1 << endl;
		return;
	}
	/* check if name2 has already been defined as a module */
	else if (moduleMap.find(name2) == moduleMap.end())
	{
		cout << "There is no module named " << name2 << endl;
		return;
	}
	/* check if name1 and name2 are identical */
	else if (name1 == name2)
	{
		cout << "Cannot connect a module to itself" << endl;
		return;
	}
	/* check if module name1 was defined prior to module name2 */
	else if (distance(orderedModulesDefinition.begin(), find(orderedModulesDefinition.begin(), orderedModulesDefinition.end(), name1))
			> distance(orderedModulesDefinition.begin(), find(orderedModulesDefinition.begin(), orderedModulesDefinition.end(), name2)))
	{
		cout << "Cannot connect " << name1 << " to " << name2 << " as " << name1 << " was defined after " << name2 << endl;
		return;
	}
	else {
		/* connect module name2 to receive input from module name1 */
		moduleMap.find(name2)->second->setInputFrom(name1, moduleMap);
		/* connect module name1 to send output to module name2 */
		moduleMap.find(name1)->second->setOutputTo(name2, moduleMap);
		/* set isConnectedToNetwork to true for both modules */
		moduleMap.find(name1)->second->isConnectedToNetwork = true;
		moduleMap.find(name2)->second->isConnectedToNetwork = true;

		/* keeps orderedModulesConnection free of duplicates */
		if (find(orderedModulesConnection.begin(), orderedModulesConnection.end(), name1) == orderedModulesConnection.end())
			orderedModulesConnection.push_back(name1);
		if (find(orderedModulesConnection.begin(), orderedModulesConnection.end(), name2) == orderedModulesConnection.end())
			orderedModulesConnection.push_back(name2);
	}
}

void Processor::processAll(vector<string> inputStrings)
{
	int numInputStrings = (int)(inputStrings.size() - 1);

	/* processor resets any previous module network in case new connections were made, checks for the existing connection pattern between modules,
			  and establishes the module network for processing*/
	setModuleNetwork();

	/* clear any existing data in processor input and output stream buffers */
	prepareForProcessing();

	/* process each string from the input stream (except the "process" keyword) */
	for (int i = 1; i < (int)inputStrings.size(); i++)
		process(inputStrings[i]);

	/* check if any of the modules in the current network are of 'delay' type and if so, process 1 more time with no new input */
	if (checkForDelayModules())
		process("");

	/* check the number of strings in the output stream and limit it to sixteen times the number of input strings */
	checkAndLimitOutputStream(numInputStrings);

	/* print the final output stream */
	printOutputStream();
}

void Processor::prepareForProcessing()
{
	inputStream.clear();
	outputStream.clear();
	for (auto& elem : moduleMap)
	{
		elem.second->resetPreviousInput();
		elem.second->holdingBuffer.clear();
	}
}

void Processor::setModuleNetwork()
{
	/*reset existing network*/
	inputTo.clear();
	outputFrom.clear();
	numLastModules = 0;

	for (auto &elem : orderedModulesConnection)
	{
		/* reset for each module since new connections may have taken place to change the network order */
		moduleMap.find(elem)->second->isFirstModule = false;
		moduleMap.find(elem)->second->isLastModule = false;

		/* if a module receives no inputs AND sends 1 or more outputs */
		if (moduleMap.find(elem)->second->hasNoInputs() == true && moduleMap.find(elem)->second->hasNoOutputs() == false)
		{
			/* the processor sends input to this/these module(s), i.e. it is first in the network */
			inputTo.push_back(moduleMap.find(elem)->second);
			moduleMap.find(elem)->second->isFirstModule = true;
		}
		/* if a module receives 1 or more inputs AND sends no outputs */
		else if (moduleMap.find(elem)->second->hasNoInputs() == false && moduleMap.find(elem)->second->hasNoOutputs() == true)
		{
			/* the processor receives output from this/these module(s), i.e. it is last in the network */
			outputFrom.push_back(moduleMap.find(elem)->second);
			moduleMap.find(elem)->second->isLastModule = true;
			numLastModules += 1;
		}
	}
}

void Processor::process(string input)
{
	/* clear any existing information in input buffer */
	inputStream.clear();
	
	/* fill the processor input buffer */
	inputStream.push_back(input);

	/* iterate through modules in the order they have been defined */
	for (auto &elem : orderedModulesDefinition)
	{
		/* check if the module is part of the network */
		if (moduleMap.find(elem)->second->isConnectedToNetwork)
		{
			/* if so, look up moduleMap for the module and process it */
			moduleMap.find(elem)->second->process(input, outputStream, numLastModules);
		}
	}
	/* below section is only applicable for when there is more than one last module in the network,
	   i.e. there are unmerged parallel chains */
	if (numLastModules > 1)
	{
		/* while at least 1 of the modules in outputFrom has a holding buffer which is not empty */
		while (!areAllHoldingBuffersEmpty()) 
		{
			/*create a new string*/
			outputStream.push_back("");
			/*iterate through the last modules in connection order*/
			for (auto &module : outputFrom) 
			{
				if (!module->holdingBuffer.empty()) 
				{
					/*send first string from holdingBuffer to the processor's outputStream */
					outputStream.back().append(module->holdingBuffer[0]);
					/* and then erase that string from holdingBuffer */
					module->holdingBuffer.erase(module->holdingBuffer.begin()); 
				}
			}
		}
	}
}

/* checks to see if any of the modules that are part of the current network are delay modules */
bool Processor::checkForDelayModules()
{
	bool anyDelay = false;
	for (auto &elem : orderedModulesConnection)
	{
		if (moduleMap.find(elem)->second->getModType() == "delay")
			anyDelay = true;
	}

	return anyDelay;
}

/* should only be called once, in processAll(), so that the number of output streams are
 "limited to sixteen times the number of input strings in the corresponding process line" */

void Processor::checkAndLimitOutputStream(int numInputStrings)
{
	int numOutputStrings = (int)outputStream.size();
	if (numOutputStrings > numInputStrings * 16)
	{
		outputStream.resize(numInputStrings * 16);
	}
}

void Processor::printOutputStream()
{
	/* should only be called once, in main(), after all processing is completed */
	if (!outputStream.empty())
	{
		/* print out each string in the output buffer, separated by a single space character */
		for (int i = 0; i < (int)outputStream.size() - 1; i++)
			cout << outputStream[i] << " ";
		cout << outputStream.back();
	}
}

/* iterates through all modules which are last in the network to check if their holding buffers are empty
   only used when there is more than one last module in the network (unmerged parallel chains) */

bool Processor::areAllHoldingBuffersEmpty()
{
	int emptyBuffers = 0;
	int numBuffers = (int)outputFrom.size();
	for (auto &elem : outputFrom)
	{
		if (elem->holdingBuffer.empty())
			emptyBuffers += 1;
	}

	if (emptyBuffers == numBuffers)
		return true;
	else
		return false;
};