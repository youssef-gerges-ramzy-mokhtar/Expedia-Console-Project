#include <unordered_map>
#include <vector>
using namespace std;

#include "../internal_api_wrappers/internal_payment_apis.hpp"
#include "user_authentication_api.hpp"
#include "../expedia_reservation_logic/expedia_reservation_logic.hpp"

#ifndef EXPEDIA_BOOKING_API_HPP_
#define EXPEDIA_BOOKING_API_HPP_

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

class ExpediaBookingAPI {
private:
	UserItinerariesDAO userItineraries;

	IPaymentAPI* getActivePaymentAPI() {
		ActivePaymentAPIFactory paymentFactory;
		return paymentFactory.getActivePaymentAPI();
	}

public:
	bool book(Itinerary itinerary, CardInfo cardInfo, UserInfo userInfo) {
		// first try to reserve the itinerary
		bool reserved = itinerary.reserve();
		if (!reserved)
			return false;

		// if iteinerary reserved succesffully try to do the payment
		IPaymentAPI* paymentAPI = getActivePaymentAPI();
		bool paid = paymentAPI->makePayment(cardInfo, itinerary.totalCost());
		delete paymentAPI;

		if (!paid) { // if the payment failed we cancel the reservation and return false
			itinerary.cancelReservation();
			return false;
		}

		// if payment success then register this itinerary to the user account and return true
		userItineraries.addItineraryToUser(userInfo.getUserId(), itinerary);
		return true;
	}

	vector<Itinerary> getUserBookedItineraries(const UserInfo &userInfo) {
		return userItineraries.getUserItineraries(userInfo.getUserId());
	}
};

#endif