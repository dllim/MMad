#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

void echo(vector<string> &inputBuffer, vector<string> &outputBuffer)
{
	for (auto &elem : inputBuffer) {
		outputBuffer.push_back(elem + elem);
	}
}

void reverse(vector<string> &inputBuffer, vector<string> &outputBuffer)
{
	string input = inputBuffer.at(0);
	int inputLength = (int)input.length();
	
	for (int i = 0; i < inputLength / 2; i++) {
		swap(input[i], input[inputLength - i - 1]);
	}
	
	outputBuffer.push_back(input);

	/* using reverse() causes compile errors on GNU*/
	
}

void delay(vector<string> &outputBuffer, vector<string> &previousInputBuffer)
{
		for (auto &elem : previousInputBuffer)
			outputBuffer.push_back(elem);
}

void noop(vector<string> &inputBuffer, vector<string> &outputBuffer)
{
	for (auto & elem : inputBuffer)
		outputBuffer.push_back(elem);
}