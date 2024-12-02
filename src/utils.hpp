#include <string>
#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

#ifndef UTILS_H_
#define UTILS_H_

bool containSpaces(const string &str) {
	for (auto chr: str)
		if (chr == ' ')
			return true;

	return false;
}

int readMenuOption(const vector<string> &menuOptions) {
	cout << "Menu:\n";
	for (int i = 0; i < menuOptions.size(); i++)
		cout << "\t" << i+1 << ". " << menuOptions[i] << "\n";
	
	cout << "\n";

	int choice;
	cout << "Enter number in range 1 - " << menuOptions.size() << ": ";
	cin >> choice;

	assert(1 <= choice && choice <= menuOptions.size());
	return choice;
}

#endif