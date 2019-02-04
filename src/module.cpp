#pragma once
#include <string>
#include <iostream>
#include <memory>
#include "../include/processor.h"
#include "../include/module.h"

using namespace std;

Module::Module() {};
Module::~Module() {};

Module::Module(string name, string type)
{
	modName = name;
	modType = type;
}

/* keeps track of the input connections to the module, in the order they were made */
void Module::setInputFrom(string name, map<string, shared_ptr<class Module>> &map)
{
	inputFrom.push_back(map.find(name)->second);
}

/* keeps track of the output connections from the module, in the order they were made */
void Module::setOutputTo(string name, map<string, shared_ptr<class Module>> &map)
{
	outputTo.push_back(map.find(name)->second);
}

/* checks if this module receives zero inputs from other modules */
bool Module::hasNoInputs()
{
	return inputFrom.empty();
}

/* checks if this module sends output to zero other modules */
bool Module::hasNoOutputs()
{
	return outputTo.empty();
}

void Module::process(string input, vector<string> &processorOutputBuffer, int numLastModules)
{
	inputBuffer.clear();
	outputBuffer.clear();

	/* if this module is the first module(s) in the network, fill input buffer with input variable */
	if (isFirstModule)
	{
		inputBuffer.push_back(input);
	}
	/* otherwise , fill input buffer with the contents of the output buffer
		belonging to the module(s) it takes inputs from */
	else
	{
		/* if a module takes input from more than 1 source, append the inputs in order of connection */
		if (inputFrom.size() > 1)
		{
			inputBuffer.push_back("");
			for (int i = 0; i < inputFrom.size(); i++)
			{
				for (auto &elem : inputFrom[i]->outputBuffer)
					inputBuffer[0].append(elem);
			}
		}
		else /* otherwise, fill input buffer with the output buffer of the previous module in the network*/
		{
			for (int i = 0; i < inputFrom.size(); i++)
			{
				for (auto &elem : inputFrom[i]->outputBuffer)
					inputBuffer.push_back(elem);
			}
		}
	}
 
	/* process inputBuffer according to the module's operation type */
	if (modType == "reverse")
	{
		reverse(inputBuffer, outputBuffer);
	}
	else if (modType == "echo")
	{
		echo(inputBuffer, outputBuffer);
	}
	else if (modType == "delay")
	{
		delay(outputBuffer, previousInputBuffer);
	}
	else if (modType == "noop")
	{
		noop(inputBuffer, outputBuffer);
	}

	/* clear out previousInputBuffer and fill it with contents of inputBuffer */
	previousInputBuffer.clear();
	for (auto &elem : inputBuffer)
		previousInputBuffer.push_back(elem);


	/* if this module is THE ONLY last module in the network chain, send contents of the module's output buffer to the processor's output buffer */
	if (isLastModule && numLastModules == 1)
	{

		for (auto &elem : outputBuffer)
		{
			processorOutputBuffer.push_back(elem);
		}
	}

	/* if there is MORE THAN ONE last module in the network due to unmerged parallel chains, 
	send contents of this module's output buffer to a holding buffer.

	the processor output buffer will draw from the holding buffer later.
	this is to preserve the string summing hierarchy of the modules according to connnection
	since module.process() is iterated through according to module definition hierarchy. */

	else if (isLastModule && numLastModules > 1) 
	{
		for (auto &elem : outputBuffer)
		{
			holdingBuffer.push_back(elem);
		}
	}
}

/* resets the previousInputBuffer to "hello", so that condition "if there are several process lines,
  each of them should behave the same way as if input alone" can be met.
  Called in prepareForProcessing() */

void Module::resetPreviousInput()
{
	previousInputBuffer = { "hello" };
}

