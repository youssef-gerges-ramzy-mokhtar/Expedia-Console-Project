#include <vector>
#include <iostream>
using namespace std;

#include "../internal_api_wrappers/internal_hotel_apis.hpp"

#ifndef EXPEDIA_HOTEL_API_HPP_
#define EXPEDIA_HOTEL_API_HPP_

/*
Although you might find duplication between the search() methods in the ExpediaHoteAPI and the ExpediaFlightAPI, but that is just because we are using similar interface for the Internal API Wrappers for the Hotel and the Flights which might not be the case in the future as different API Wrappers might need different logic for searching that is why I will not create some abstract class that contains a generic search method as the return type and parameters might be different for each Category in the future 
*/

class ExpediaHotelAPI {
private:
	vector<IHotelAPI*> hotelAPIs;

public:
	ExpediaHotelAPI() {
		HotelAPIFactory hotelAPIFactory;
		hotelAPIs = hotelAPIFactory.getAllAPIs();
	}

	// disabling copy constructor and assignment operator as no need to pass the ExpediaHotelAPI by value, and no need to pass it at all because the default constructor makes all ExpediaHotelAPI objects has the same exact data
	ExpediaHotelAPI(const ExpediaHotelAPI &other) = delete;
	ExpediaHotelAPI& operator=(const ExpediaHotelAPI &other) = delete;

	vector<HotelRoomInfo> search(const HotelRequest &hotelRequest) {
		vector<HotelRoomInfo> searchResutls;
		for (auto &hotelAPI: hotelAPIs) {
			const vector<HotelRoomInfo> &subResults = hotelAPI->search(hotelRequest);
			// we append the results returned by the current hotel API to the list of results 
			searchResutls.insert(searchResutls.end(), subResults.begin(), subResults.end());
		}

		return searchResutls;
	}

	~ExpediaHotelAPI() {
		cout << "~ExpediaHotelAPI() deleting Hotel APIs\n";
		for (auto &hotelAPI: hotelAPIs)
			delete hotelAPI;
		hotelAPIs.clear();
	}
};

#endif