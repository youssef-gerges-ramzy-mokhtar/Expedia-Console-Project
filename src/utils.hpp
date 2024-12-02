#include <string>
#include <iostream>
using namespace std;

#ifndef UTILS_H_
#define UTILS_H_

bool containSpaces(const string &str) {
	for (auto chr: str)
		if (chr == ' ')
			return true;

	return false;
}

#endif