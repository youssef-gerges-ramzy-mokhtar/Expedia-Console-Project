#include <vector>
using namespace std;

#include "../internal_api_wrappers/internal_flight_apis.hpp"

#ifndef EXPEDIA_FLIGHT_API_HPP_
#define EXPEDIA_FLIGHT_API_HPP_

class ExpediaFlightAPI {
private:
	vector<IFlightAPI*> flightAPIs;

public:
	ExpediaFlightAPI() {
		FlightAPIFactory flightAPIFactory;
		flightAPIs = flightAPIFactory.getAllAPIs();
	}

	// disabling copy constructor and assignment operator as no need to pass the ExpediaFlightAPI by value, and no need to pass it at all because the default constructor makes all ExpediaFlightAPI objects has the same exact data
	ExpediaFlightAPI(const ExpediaFlightAPI &other) = delete;
	ExpediaFlightAPI& operator=(const ExpediaFlightAPI &other) = delete;

	vector<FlightInfo> search(const FlightRequest &flightRequest) {
		vector<FlightInfo> searchResutls;
		for (auto &flightAPI: flightAPIs) {
			const vector<FlightInfo> &subResults = flightAPI->search(flightRequest);
			// we append the results returned by the current flight API to the list of results 
			searchResutls.insert(searchResutls.end(), subResults.begin(), subResults.end());
		}

		return searchResutls;
	}

	~ExpediaFlightAPI() {
		for (auto &flightAPI: flightAPIs)
			delete flightAPI;
		flightAPIs.clear();
	}
};

#endif