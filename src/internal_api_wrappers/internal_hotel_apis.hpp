#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include "external_apis/external_hotel_apis.h"
#include "api_factory.hpp"

#ifndef INTERNAL_HOTEL_APIS_HPP_
#define INTERNAL_HOTEL_APIS_HPP_

class HotelRoomInfo {
private:
	string hotelName;
	string fromDate;
	string toDate;
	string country;
	string city;
	string roomType;
	int availableRooms;
	double pricePerNight;

public:
	HotelRoomInfo(
		const string &hotelName, const string &fromDate,
		const string &toDate, const string &country,
		const string &city, const string &roomType,
		int availableRooms, double pricePerNight)
		: hotelName(hotelName), fromDate(fromDate),
		toDate(toDate), country(country),
		city(city), roomType(roomType),
		availableRooms(availableRooms), pricePerNight(pricePerNight) {}

	// Getters
	string getHotelName() const { return hotelName; }
	string getFromDate() const { return fromDate; }
	string getToDate() const { return toDate; }
	string getCountry() const { return country; }
	string getCity() const { return city; }
	int getAvailableRooms() const { return availableRooms; }
	double getPricePerNight() const { return pricePerNight; }
};

class HotelRequest {
private:
	string fromDate;
	string toDate;
	string country;
	string city;
	int adults;
	int children;
	int neededRooms;

public:
	HotelRequest(
		const string &fromDate, const string &toDate,
		const string &country, const string &city,
		int adults, int children, int neededRooms)
		: fromDate(fromDate), toDate(toDate),
		  country(country), city(city),
		  adults(adults), children(children), neededRooms(neededRooms) {}

	// Getters
	string getFromDate() const { return fromDate; }
	string getToDate() const { return toDate; }
	string getCountry() const { return country; }
	string getCity() const { return city; }
	int getAdults() const { return adults; }
	int getChildren() const { return children; }
	int getNeededRooms() const { return neededRooms; }
};

class IHotelAPI {
public:
	virtual vector<HotelRoomInfo> search(HotelRequest hotelRequest) = 0;
	virtual bool reserve(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) = 0;
	virtual bool cancelReservation(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) = 0;
	virtual string getHotelName() = 0;
};

// Concrete Internal Hotel API Wrappers
class HiltonHotelAPI: public IHotelAPI {
public:
	virtual vector<HotelRoomInfo> search(HotelRequest hotelRequest) override {
		vector<HiltonRoom> availableRoomsExternal = HiltonHotelOnlineAPI::SearchRooms(
			hotelRequest.getCountry(),
			hotelRequest.getCity(),
			hotelRequest.getFromDate(),
			hotelRequest.getToDate(),
			hotelRequest.getAdults(),
			hotelRequest.getChildren(),
			hotelRequest.getNeededRooms()
		);

		// Convert all HiltonRoom objects to HotelRoomInfo objects
		vector<HotelRoomInfo> availableRoomsInternal;
		for (auto room: availableRoomsExternal)
			availableRoomsInternal.push_back({getHotelName(), room.date_from, room.date_to, hotelRequest.getCountry(), hotelRequest.getCity(), room.room_type, room.available, room.price_per_night});

		return availableRoomsInternal;
	}

	virtual bool reserve(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) override {
		return true; // dummy as the external api doesn't have reservation functionality yet
	}

	virtual bool cancelReservation(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) override {
		return true; // dummy as the external api doesn't have reservation functionality yet
	}

	virtual string getHotelName() override {
		return "hilton";
	}
};

class MarriottHotelAPI: public IHotelAPI {
public:
	virtual vector<HotelRoomInfo> search(HotelRequest hotelRequest) override {
		vector<MarriottFoundRoom> availableRoomsExternal = MarriottHotelOnlineAPI::FindRooms(
			hotelRequest.getFromDate(),
			hotelRequest.getToDate(),
			hotelRequest.getCountry(),
			hotelRequest.getCity(),
			hotelRequest.getNeededRooms(),
			hotelRequest.getAdults(),
			hotelRequest.getChildren()
		);

		// Convert all MarriottFoundRoom objects to HotelRoomInfo objects
		vector<HotelRoomInfo> availableRoomsInternal;
		for (auto room: availableRoomsExternal)
			availableRoomsInternal.push_back({getHotelName(), room.date_from, room.date_to, hotelRequest.getCountry(), hotelRequest.getCity(), room.room_type, room.available, room.price_per_night});

		return availableRoomsInternal;
	}

	virtual bool reserve(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) override {
		return true; // dummy as the external api doesn't have reservation functionality yet
	}

	virtual bool cancelReservation(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) override {
		return true; // dummy as the external api doesn't have reservation functionality yet
	}

	virtual string getHotelName() override {
		return "marriott";
	}
};

// Factory
class HotelAPIFactory: public APIFactory<IHotelAPI> {
private:
	// this is the best which I have could done it is not the best way because we need to modify the init() method each time a new implementation of the IHotelAPI interface is created. The issue is that C++ doesn't have mechanisms like Java Reflection which would have made this much more dynamic
	// another approach was to make this class a singelton and make every new IHoteAPI implementation register itself with the factory but that would be fine if we only have 1 type of factory but if we have different type of factories then that will be a bit challenging
	void init() {
		IHotelAPI* hilton = new HiltonHotelAPI();
		IHotelAPI* marriott = new MarriottHotelAPI();

		addAPI(hilton->getHotelName(), []() -> IHotelAPI* {return new HiltonHotelAPI();});
		addAPI(marriott->getHotelName(), []() -> IHotelAPI* {return new MarriottHotelAPI();});

		delete hilton;
		delete marriott;
	}

public:
	HotelAPIFactory() {
		init();
	}

	IHotelAPI* createHotelAPI(const HotelRoomInfo &hotelInfo) {
		return createAPI(hotelInfo.getHotelName());
	}
};

#endif