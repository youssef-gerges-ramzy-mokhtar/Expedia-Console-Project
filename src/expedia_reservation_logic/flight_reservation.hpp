#include "../internal_api_wrappers/internal_flight_apis.hpp"
#include "expedia_reservation_logic.hpp"

#ifndef FLIGHT_RESERVATION_ITEM_HPP_
#define FLIGHT_RESERVATION_ITEM_HPP_

class FlightReservationHandler {
private:
	IFlightAPI* flightAPI;
	FlightInfo flightInfo;
	FlightRequest flightRequest;

public:
	FlightReservationHandler(FlightInfo flightInfo, FlightRequest flightRequest) : flightInfo(flightInfo), flightRequest(flightRequest) {
		FlightAPIFactory flightAPIFactory;
		this->flightAPI = flightAPIFactory.createFlightAPI(flightInfo);
	}

	bool reserve() {
		return flightAPI->reserve(flightInfo, flightRequest);
	}

	bool cancelReservation() {
		return flightAPI->cancelReservation(flightInfo, flightRequest);
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
};

#endif