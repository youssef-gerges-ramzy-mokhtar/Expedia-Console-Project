#include <string>
#include <vector>
#include <sstream>
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
	double cost;

	bool empty() const {
		return flights.empty() && hotels.empty() && subItineraries.empty();
	}

	int itinerarySize() const {
		return flights.size() + hotels.size() + subItineraries.size();
	}

	string toString() const {
		ostringstream oss;
		oss << "************************************\n";
		oss << "Itinerary of " << itinerarySize() << " sub-reservations\n";

		if (!flights.empty())
			oss << "FLIGHTS\n";
		for (const auto &flight: flights) {
			oss << "\tAirline reservation with reservation: " << flight.airlineName << " Airlines: " << "From " << flight.origin << " on " << flight.departureDateTime << "\n";
			oss << "\t\tTo " << flight.destination << " on " << flight.arrivalDateTime << "\n";
			oss << "\t\tAdults: " << flight.adultsReq << " children: " << flight.childrenReq << " infants: " << flight.infantsReq << "\n";
			oss << "\t\tTotal Cost: " << flight.cost << "\n\n";
		}

		if (!hotels.empty())
			oss << "HOTELS\n";
		for (const auto &hotel: hotels) {
			oss << "\tHotel reservation: " << hotel.hotelName << ": " << hotel.city << " @ " << hotel.country << "\n";
			oss << "\t\t" << hotel.fromDateReq << " to " << hotel.toDateReq << " : " << hotel.neededRoomsReq << "\n";
			oss << "\t\tAdults: " << hotel.adultsReq << " Children: " << hotel.childrenReq << "\n";
			oss << "\t\tTotal Cost: " << hotel.cost << "\n\n";
		}

		if (!subItineraries.empty())
			oss << "SUB-ITINERARIES\n";
		for (const auto &subItinerary: subItineraries) {
			// I want to add a tab for every line of the subItinerary.toString()
			string subItineraryString = subItinerary.toString();
			stringstream ss(subItineraryString);
			string line;
			while (getline(ss, line))
				oss << "\t" << line << "\n";
		}

		oss << "Total Itinerary cost: " << cost << "\n";
		oss << "************************************\n\n";
		return oss.str();
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