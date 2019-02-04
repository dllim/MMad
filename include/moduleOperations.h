#pragma once
#include <string>
#include <vector>

using namespace std;

void echo(vector<string> &inputBuffer, vector<string> &outputBuffer);
void reverse(vector<string> &inputBuffer, vector<string> &outputBuffer);
void delay(vector<string> &outputBuffer, vector<string> &previousInputBuffer);
void noop(vector<string> &inputBuffer, vector<string> &outputBuffer);
