#include <vector>
using namespace std;

#include "../internal_api_wrappers/internal_hotel_apis.hpp"
#include "../internal_api_wrappers/internal_flight_apis.hpp"
#include "../internal_api_wrappers/internal_payment_apis.hpp"
#include "../user_authentication.hpp"
#include "../expedia_reservation_logic/expedia_reservation_logic.hpp"
#include "user_itineraries_manager.hpp"

#ifndef EXPEDIA_CORE_API_HPP_
#define EXPEDIA_CORE_API_HPP_

/*
Although you might find duplication between the search() methods in the ExpediaHoteAPI and the ExpediaFlightAPI, but that is just because we are similar interface for the Internal API Wrappers for the Hotel and the Flights which might not be the case in the future as different API Wrappers might need different logic for searching that is why I will not create some abstract class that contains a generic search method as the return type and parameters might be different for each Category in the future 
*/

class ExpediaHotelAPI {
private:
	vector<IHotelAPI*> hotelAPIs;

public:
	ExpediaHotelAPI() {
		HotelAPIFactory hotelAPIFactory;
		hotelAPIs = hotelAPIFactory.getAllAPIs();
	}

	vector<HotelRoomInfo> search(const HotelRequest &hotelRequest) {
		vector<HotelRoomInfo> searchResutls;
		for (auto &hotelAPI: hotelAPIs) {
			const vector<HotelRoomInfo> &subResults = hotelAPI->search(hotelRequest);
			// we append the results returned by the current hotel API to the list of results 
			searchResutls.insert(searchResutls.end(), subResults.begin(), subResults.end());
		}

		return searchResutls;
	}
};

class ExpediaFlightAPI {
private:
	vector<IFlightAPI*> flightAPIs;

public:
	ExpediaFlightAPI() {
		FlightAPIFactory flightAPIFactory;
		flightAPIs = flightAPIFactory.getAllAPIs();
	}

	vector<FlightInfo> search(const FlightRequest &flightRequest) {
		vector<FlightInfo> searchResutls;
		for (auto &flightAPI: flightAPIs) {
			const vector<FlightInfo> &subResults = flightAPI->search(flightRequest);
			// we append the results returned by the current flight API to the list of results 
			searchResutls.insert(searchResutls.end(), subResults.begin(), subResults.end());
		}

		return searchResutls;
	}
};

class ExpediaBookingAPI {
private:
	UserItinerariesDAO userItineraries;
	IPaymentAPI* paymentAPI;

public:
	ExpediaBookingAPI() {
		ActivePaymentAPIFactory paymentFactory;
		paymentAPI = paymentFactory.getActivePaymentAPI();
	}

	bool book(Itinerary itinerary, PaymentInfo paymentInfo, UserInfo userInfo) {
		// first try to reserve the itinerary
		bool reserved = itinerary.reserve();
		if (!reserved)
			return false;

		// if iteinerary reserved succesffully try to do the payment
		bool paid = paymentAPI->makePayment(paymentInfo);
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

	~ExpediaBookingAPI() {
		if (paymentAPI)
			delete paymentAPI;
		paymentAPI = nullptr;
	}
};

#endif