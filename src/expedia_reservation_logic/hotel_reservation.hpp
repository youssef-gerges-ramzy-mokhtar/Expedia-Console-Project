#include "../internal_api_wrappers/internal_hotel_apis.hpp"
#include "expedia_reservation_logic.hpp"

#ifndef HOTEL_RESERVATION_ITEM_HPP_
#define HOTEL_RESERVATION_ITEM_HPP_

class HotelReservationHandler {
private:
	IHotelAPI* hotelAPI;
	HotelRoomInfo hotelRoomInfo;
	HotelRequest hotelRequest;

public:
	HotelReservationHandler(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) : hotelRoomInfo(hotelRoomInfo), hotelRequest(hotelRequest) {
		HotelAPIFactory hotelAPIFactory;
		this->hotelAPI = hotelAPIFactory.createHotelAPI(hotelRoomInfo);
	}

	bool reserve() {
		return hotelAPI->reserve(hotelRoomInfo, hotelRequest);
	}

	bool cancelReservation() {
		return hotelAPI->cancelReservation(hotelRoomInfo, hotelRequest);
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
};

#endif