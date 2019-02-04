#include "../include/processor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

Processor processor;

int main()
{
	string input;
	cout << "WELCOME TO MODULAR MADNESS!" << endl
		<< "To define a module: " << endl
		<< "module <name> <operation>" << endl
		<< "" << endl
		<< "To make a connection: " << endl
		<< "connect <module_name1> <module_name2>" << endl
		<< "" << endl
		<< "To process an input stream: " << endl
		<< "process <string> <string>" << endl
		<< "The number of output strings is limited to 16 times the number of input strings" << endl
		<< "" << endl;

	while (1)
	{
		getline(cin, input);
		istringstream iss(input);
		vector<string> inputStrings;
		string word;

		while (iss >> word)
			inputStrings.push_back(word);

		/* if "module" keyword is the first string, processor attempts to define a new module */
		if (inputStrings[0] == "module") {

			processor.addModule(inputStrings[1], inputStrings[2]);
		}
		/* if "connect" keyword is the first string, processor attempts to connect two modules */
		else if (inputStrings[0] == "connect") {

			processor.connectModules(inputStrings[1], inputStrings[2]);
		}
		/* if "process" keyword is the first string in the line, processor attempts to process the other strings of the line*/
		else if (inputStrings[0] == "process") {
			processor.processAll(inputStrings);
			cout << "" << endl;
		}
	}
}

