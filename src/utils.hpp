#include <string>
using namespace std;

bool containSpaces(const string &str) {
	for (auto chr: str)
		if (chr == ' ')
			return true;

	return false;
}