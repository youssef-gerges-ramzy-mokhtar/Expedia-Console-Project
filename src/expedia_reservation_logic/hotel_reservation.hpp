#include <sstream>
using namespace std;

#include "../internal_api_wrappers/internal_hotel_apis.hpp"
#include "expedia_reservation_logic.hpp"

#ifndef HOTEL_RESERVATION_ITEM_HPP_
#define HOTEL_RESERVATION_ITEM_HPP_

class HotelReservationHandler {
private:
	HotelRoomInfo hotelRoomInfo;
	HotelRequest hotelRequest;
	IHotelAPI* hotelAPI;

public:
	HotelReservationHandler(const HotelRoomInfo &hotelRoomInfo, const HotelRequest &hotelRequest) : hotelRoomInfo(hotelRoomInfo), hotelRequest(hotelRequest) {
		HotelAPIFactory hotelAPIFactory;
		this->hotelAPI = hotelAPIFactory.createHotelAPI(hotelRoomInfo);
	}

	HotelReservationHandler(const HotelReservationHandler &other) : HotelReservationHandler(other.hotelRoomInfo, other.hotelRequest) {
	}

	bool reserve() {
		return hotelAPI->reserve(hotelRoomInfo, hotelRequest);
	}

	bool cancelReservation() {
		return hotelAPI->cancelReservation(hotelRoomInfo, hotelRequest);
	}

	HotelReservationHandler& operator=(const HotelReservationHandler &other) {
		if (this != &other) {
			hotelRoomInfo = other.hotelRoomInfo;
			hotelRequest = other.hotelRequest;
		}

		return *this;
	}

	~HotelReservationHandler() {
		delete hotelAPI;
	}
};

class HotelReservationItem: public IReservationItem {
private:
	HotelRoomInfo hotelRoomInfo;
	HotelRequest hotelRequest;
	HotelReservationHandler reservationHandler;

public:
	HotelReservationItem(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) :
		hotelRoomInfo(hotelRoomInfo), hotelRequest(hotelRequest), reservationHandler(hotelRoomInfo, hotelRequest)
	{}

	virtual double totalCost() override {
		return hotelRequest.getNumberOfNights() * hotelRequest.getNeededRooms() * hotelRoomInfo.getPricePerNight();
	}

	virtual bool reserve() override {
		return reservationHandler.reserve();
	}

	virtual bool cancelReservation() override {
		return reservationHandler.cancelReservation();
	}

	virtual IReservationItem* clone() const override {
		return new HotelReservationItem(hotelRoomInfo, hotelRequest);
	}

	virtual string toString() override {
		ostringstream oss;
		oss << "Hotel reservation: " << hotelRoomInfo.getHotelName() << ": " << hotelRoomInfo.getCity() << " @ " << hotelRoomInfo.getCountry() << "\n";
		oss << "\t" << hotelRequest.getFromDate() << " to " << hotelRequest.getToDate() << " : " << hotelRequest.getNumberOfNights() << "\n";
		oss << "\tAdults: " << hotelRequest.getAdults() << " Children: " << hotelRequest.getChildren() << + "\n";
		oss << "\tTotal Cost: " << totalCost() << "\n";

		return oss.str();
	}
};

#endif