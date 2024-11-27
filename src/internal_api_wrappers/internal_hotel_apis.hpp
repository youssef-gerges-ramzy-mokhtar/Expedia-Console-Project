#include <string>
#include <vector>

#include "external_apis/external_hotel_apis.h"
using namespace std;

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
};

// Concrete Internal Hotel API Wrappers
class HiltonHotelAPI: IHotelAPI {
public:
	virtual vector<HotelRoomInfo> search(HotelRequest hotelRequest) {
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
			availableRoomsInternal.push_back({"hilton", room.date_from, room.date_to, hotelRequest.getCountry(), hotelRequest.getCity(), room.room_type, room.available, room.price_per_night});

		return availableRoomsInternal;
	}

	virtual bool reserve(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) {
		return true; // dummy as the external api doesn't have reservation functionality yet
	}

	virtual bool cancelReservation(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) {
		return true; // dummy as the external api doesn't have reservation functionality yet
	}
};

class MarriottHotelAPI: IHotelAPI {
public:
	virtual vector<HotelRoomInfo> search(HotelRequest hotelRequest) {
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
			availableRoomsInternal.push_back({"marriott", room.date_from, room.date_to, hotelRequest.getCountry(), hotelRequest.getCity(), room.room_type, room.available, room.price_per_night});

		return availableRoomsInternal;
	}

	virtual bool reserve(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) {
		return true; // dummy as the external api doesn't have reservation functionality yet
	}

	virtual bool cancelReservation(HotelRoomInfo hotelRoomInfo, HotelRequest hotelRequest) {
		return true; // dummy as the external api doesn't have reservation functionality yet
	}
};