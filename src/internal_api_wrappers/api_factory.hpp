#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#ifndef API_FACTORY_HPP_
#define API_FACTORY_HPP_

template <typename T>
class APIFactory {
private:
	unordered_map<string, T*> availableAPIs;

public:
	void addAPI(string name, T* api) {
        availableAPIs[name] = api; // this assignment might cause a memory leak
    }

    // null is returnd if the API doesn't exist
    T* createAPI(const string &name) {
    	if (availableAPIs.count(name))
    		return availableAPIs[name];

    	return nullptr;
    }

    vector<T*> getAllAPIs() const {
    	vector<T*> apis;
    	for (auto &entry: availableAPIs)
    		apis.push_back(entry.second);

    	return apis;
    }

    virtual ~APIFactory() {
        cout << "~APIFactory() deleting " << typeid(T).name() << " objects\n";
    	for (auto &entry: availableAPIs)
    		delete entry.second;
    }
};

#endif