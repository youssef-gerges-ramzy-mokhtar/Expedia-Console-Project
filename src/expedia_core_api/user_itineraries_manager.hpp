#include <unordered_map>
#include <vector>
using namespace std;

#include "../expedia_reservation_logic/expedia_reservation_logic.hpp"

#ifndef USER_ITINERARIES_MANAGER_HPP_
#define USER_ITINERARIES_MANAGER_HPP_

class UserItinerariesDAO {
private:
	unordered_map<string, vector<Itinerary>> userItineraries;

public:
	void addItineraryToUser(const string &userID, const Itinerary &itinerary) {
		userItineraries[userID].push_back(itinerary);
	}

	vector<Itinerary> getUserItineraries(const string &userID) {
		return userItineraries[userID];
	}
};

#endif