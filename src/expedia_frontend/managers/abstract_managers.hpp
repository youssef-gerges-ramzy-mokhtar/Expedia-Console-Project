#include "manager_data_objects.hpp"

#ifndef ABSTRACT_MANAGERS_HPP_
#define ABSTRACT_MANAGERS_HPP_

class IFlightsManager {
public:
	virtual vector<FlightData> searchFlights(const FlightData &flightData) = 0;
};

class IHotelsManager {
public:
	virtual vector<HotelData> searchHotels(const HotelData &hotelData) = 0;
};

class IBookingManager {
public:
	virtual bool book(const ItineraryData &itinerary, const PaymentCardData &card, const UserData &user) = 0;
	virtual vector<ItineraryData> getItineraries(const UserData &user) = 0;
	virtual bool addPaymentCard(const PaymentCardData &card, const UserData &user) = 0;
	virtual vector<PaymentCardData> getPaymentCards(const UserData &user) = 0;
};

class IUserAuthenticationManager {
public:
	virtual AuthStatusData login(const UserData &userData) = 0;
	virtual AuthStatusData signup(const UserData &userData) = 0;
};

#endif