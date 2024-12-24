#include <string>
#include <vector>
using namespace std;

#ifndef VIEW_DATA_OBJECTS_HPP_
#define VIEW_DATA_OBJECTS_HPP_

struct FlightData {
	string airlineName;
	string flightNumber;
	string departureDateTime;
	string arrivalDateTime;
	string origin;
	string destination;
	double price;

	int adultsReq;
	int infantsReq;
	int childrenReq;

	double cost;
};

struct HotelData {
	string hotelName;
	string fromAvailableDate;
	string toAvailableDate;
	string country;
	string city;
	string roomType;
	int availableRooms;
	double pricePerNight;

	string fromDateReq;
	string toDateReq;
	int adultsReq;
	int childrenReq;
	int neededRoomsReq;

	double cost;
};

struct ItineraryData {
	vector<FlightData> flights;
	vector<HotelData> hotels;
	vector<ItineraryData> subItineraries;

	bool empty() {
		return flights.empty() && hotels.empty() && subItineraries.empty();
	}
};

struct PaymentCardData {
	string name;
	string address;
	string id;
	string expiryDate;
	int cvv;
};

struct UserData {
	string id;
	string email;
	string username;
	string password;
	string userType;
};

struct AuthStatusData {
	bool authenticated;
	string responseMessage;
	UserData userDataResponse;
};

#endif