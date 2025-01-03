#include <sstream>
using namespace std;

#include "../internal_api_wrappers/internal_flight_apis.hpp"
#include "expedia_reservation_logic.hpp"

#ifndef FLIGHT_RESERVATION_ITEM_HPP_
#define FLIGHT_RESERVATION_ITEM_HPP_

class FlightReservationHandler {
private:
	FlightInfo flightInfo;
	FlightRequest flightRequest;
	IFlightAPI* flightAPI;

public:
	FlightReservationHandler(const FlightInfo &flightInfo, const FlightRequest &flightRequest) : flightInfo(flightInfo), flightRequest(flightRequest) {
		FlightAPIFactory flightAPIFactory;
		this->flightAPI = flightAPIFactory.createFlightAPI(flightInfo);
	}

	FlightReservationHandler(const FlightReservationHandler &other) : FlightReservationHandler(other.flightInfo, other.flightRequest) {
	}

	bool reserve() {
		return flightAPI->reserve(flightInfo, flightRequest);
	}

	bool cancelReservation() {
		return flightAPI->cancelReservation(flightInfo, flightRequest);
	}

	FlightReservationHandler& operator=(const FlightReservationHandler &other) {
		if (this != &other) {
			flightInfo = other.flightInfo;
			flightRequest = other.flightRequest;
		}

		return *this;
	}

	~FlightReservationHandler() {
		delete flightAPI;
	}
};

class FlightReservationItem: public IReservationItem {
private:
	FlightInfo flightInfo;
	FlightRequest flightRequest;
	FlightReservationHandler reservationHandler;

public:
	FlightReservationItem(FlightInfo flightInfo, FlightRequest flightRequest) :
		flightInfo(flightInfo), flightRequest(flightRequest), reservationHandler(flightInfo, flightRequest)
	{}

	virtual double totalCost() override {
		return flightInfo.getPrice();
	}

	virtual bool reserve() override {
		return reservationHandler.reserve();
	}

	virtual bool cancelReservation() override {
		return reservationHandler.cancelReservation();
	}

	virtual IReservationItem* clone() const override {
		return new FlightReservationItem(flightInfo, flightRequest);
	}

	virtual string toString() override {
		ostringstream oss;
		oss << "Airline reservation with reservation: " << flightInfo.getAirlineName() << " Airlines: " << "From " << flightInfo.getOrigin() << " on " << flightInfo.getDepartureDateTime() << "\n";
		oss << "\tTo " << flightInfo.getDestination() << " on " << flightInfo.getArrivalDateTime() << "\n";
		oss << "\tAdults: " << flightRequest.getAdults() << " children: " << flightRequest.getChildren() << " infants: " << flightRequest.getInfants() << "\n";
		oss << "\tTotal Cost: " << totalCost() << "\n";
		return oss.str();
	}
};

#endif