#include <string>
using namespace std;

bool containSpaces(const string &str) {
	for (auto chr: str)
		if (chr == ' ')
			return true;

	return false;
}

class Object {
public:
	virtual clone() = 0;
	virtual toString() = 0;
	virtual ~Object() {}
};