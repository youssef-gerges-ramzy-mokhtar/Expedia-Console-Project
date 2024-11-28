#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
using namespace std;

#ifndef API_FACTORY_HPP_
#define API_FACTORY_HPP_

template <typename T>
class APIFactory {
private:
	unordered_map<string, function<T*()>> availableAPIs;

public:
	void addAPI(string name, function<T*()> apiConstructor) {
        availableAPIs[name] = apiConstructor;
    }

    // null is returnd if the API doesn't exist
    T* createAPI(const string &name) {
    	if (availableAPIs.count(name))
    		return availableAPIs[name]();

    	return nullptr;
    }

    vector<T*> getAllAPIs() const {
    	vector<T*> apis;
    	for (auto &entry: availableAPIs)
    		apis.push_back(entry.second());

    	return apis;
    }
};

#endif