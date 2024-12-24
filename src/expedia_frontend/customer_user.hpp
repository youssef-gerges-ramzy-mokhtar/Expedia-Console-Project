#include <stdexcept>
using namespace std;

#include "user.hpp"
#include "managers/managers_factory.hpp"

#ifndef CUSTOMER_USER_UI_HPP_
#define CUSTOMER_USER_UI_HPP_

class ItineraryMakerUI {
private:
	IFlightsManager &flightsManager;
	IHotelsManager &hotelsManager;

private:
	void addFlight(ItineraryData &itinerary) {
		string from;
		cout << "Enter From: ";
		cin >> from;

		string to;
		cout << "Enter To: ";
		cin >> to;

		string fromDate;
		cout << "Enter From Date (dd-mm-yy): ";
		cin >> fromDate;

		string toDate;
		cout << "Enter To Date (dd-mm-yy): ";
		cin >> toDate;

		int adults, children, infants;
		cout << "Enter # of adults children (5-16) and infants: ";
		cin >> adults >> children >> infants;

		// construct the data needed for the request
		FlightData flightRequest;
		flightRequest.departureDateTime = fromDate; 
		flightRequest.arrivalDateTime = toDate; 
		flightRequest.origin = from; 
		flightRequest.destination = to; 
		flightRequest.adultsReq = adults; 
		flightRequest.childrenReq = children; 
		flightRequest.infantsReq = infants;

		vector<FlightData> searchResults = flightsManager.searchFlights(flightRequest);
		for (int i = 0; i < searchResults.size(); i++) {
			const auto &flight = searchResults[i];
			cout << i + 1 << "-"
				<< " Airline: " << flight.airlineName
				<< " Cost: " << flight.price
				<< " Departure Date: " << flight.departureDateTime
				<< " Arrival Date: " << flight.arrivalDateTime << "\n";
		}

		int choice;
		cout << "\nEnter -1 to cancel or number in range 1 - " << searchResults.size() << ": ";
		cin >> choice;
		cout << "\n";

		if (choice == -1)
			return;

		assert(1 <= choice && choice <= searchResults.size());

		// add the request info to the choosen search result
		searchResults[choice -1].adultsReq = adults;
		searchResults[choice -1].childrenReq = children;
		searchResults[choice -1].infantsReq = infants;
		itinerary.flights.push_back(searchResults[choice -1]);
	}

	void addHotel(ItineraryData &itinerary) {
		string fromDate;
		cout << "Enter From Date (dd-mm-yy): ";
		cin >> fromDate;

		string toDate;
		cout << "Enter To Date (dd-mm-yy): ";
		cin >> toDate;

		string country;
		cout << "Enter Country: ";
		cin >> country;

		string city;
		cout << "Enter City: ";
		cin >> city;

		int neededRooms;
		cout << "Enter number of needed rooms: ";
		cin >> neededRooms;

		int adults, children;
		cout << "Enter # of adults and childrne: ";
		cin >> adults >> children;

		// construct the data needed for the request
		HotelData hotelRequest;
		hotelRequest.fromDateReq = fromDate;
		hotelRequest.toDateReq = toDate;
		hotelRequest.country = country;
		hotelRequest.city = city;
		hotelRequest.neededRoomsReq = neededRooms;
		hotelRequest.adultsReq = adults;
		hotelRequest.childrenReq = children;
		
		vector<HotelData> searchResults = hotelsManager.searchHotels(hotelRequest);

		for (int i = 0; i < searchResults.size(); i++) {
			const auto &hotel = searchResults[i];
			cout << i + 1 << "-"
				<< " Hotel: " << hotel.hotelName
				<< " Room Type: " << hotel.roomType << "(" << hotel.availableRooms << ")"
				<< " Price per night: " << hotel.pricePerNight
				<< " From Date: " << hotel.fromAvailableDate
				<< " to " << hotel.toAvailableDate << "\n";
		}

		int choice;
		cout << "\nEnter -1 to cancel or number in range 1 - " << searchResults.size() << ": ";
		cin >> choice;
		cout << "\n";

		if (choice == -1)
			return;

		assert(1 <= choice && choice <= searchResults.size());
		
		// add the request info to the choosen search result
		searchResults[choice -1].adultsReq = adults;
		searchResults[choice -1].childrenReq = children;
		searchResults[choice -1].neededRoomsReq = neededRooms;
		itinerary.hotels.push_back(searchResults[choice -1]);
	}

public:
	// @todo use factory
	ItineraryMakerUI() : 
		flightsManager(FlightsManagerFactory::getFlightsManager()),
		hotelsManager(HotelsManagerFactory::getHotelsManager())
	{}

	ItineraryData createItinerary() {
		ItineraryData itinerary;

		while (true) {
			int choice = readMenuOption({"Add Flight", "Add Hotel", "Done", "Cancel"});
			cout << "\n";

			if (choice == 1)
				addFlight(itinerary);
			else if (choice == 2)
				addHotel(itinerary);
			else if (choice == 3)
				break;
			else if (choice == 4)
				return ItineraryData(); // empty Itinerary
		}

		return itinerary;
	}
};

class PaymentCardUI {
private:
	IBookingManager &bookingManager;
	UserData userData;

private:
	bool userHasPaymentCards() {
		return bookingManager.getPaymentCards(userData).size() > 0;
	}

	PaymentCardData getPaymentCardSelectedByUser() {
		vector<PaymentCardData> cards = bookingManager.getPaymentCards(userData);
		assert(!cards.empty());

		cout << "Select a payment choice:\n";
		for (int i = 0; i < cards.size(); i++) {
			auto &card = cards[i];
			cout << i+1 << ":" << " Owner: " << card.name << " Card number: " << card.id << "\n";
		}

		int choice;
		cout << "\nEnter number in range 1 - " << cards.size() << ": ";
		cin >> choice;

		assert(1 <= choice && choice <= cards.size());
		return cards[choice-1];
	}

	PaymentCardData addPaymentCard() {
		string name;
		cout << "Enter Name on Card: ";
		cin >> name;

		string cardNumber;
		cout << "Enter Debit/Credit card number: ";
		cin >> cardNumber;

		string expiryDate;
		cout << "Enter Expiration date (mm/yy): ";
		cin >> expiryDate;

		int cvv;
		cout << "Enter Security code (cvv): ";
		cin >> cvv;

		string address;
		cout << "Enter Billing Address: ";
		cin >> address;

		PaymentCardData card = {name, address, cardNumber, expiryDate, cvv};
		bookingManager.addPaymentCard(card, userData);
		return card;
	}

public:
	PaymentCardUI(const UserData &userData) : 
		bookingManager(BookingManagerFactory::getBookingManger()),
		userData(userData) 
	{}

	PaymentCardData getPaymentCard() {
		while (true) {
			int choice = readMenuOption({"Pick from available cards", "Add new payment card"});
			cout << "\n";

			if (choice == 1) {
				if (!userHasPaymentCards())
					cout << "You don't have any registered payment cards, please add a new payment card\n\n";
				else
					return getPaymentCardSelectedByUser();
			} else if (choice == 2)
				return addPaymentCard();
		}
	}
};

class CustomerUserUI: public UserUI {
private:
	IBookingManager &bookingManager;
	ItineraryMakerUI itineraryMakerUI;
	PaymentCardUI paymentCardUI;

private:
	void makeItinerary() {
		ItineraryData itinerary = itineraryMakerUI.createItinerary();
		if (itinerary.empty())
			return;

		PaymentCardData cardInfo = paymentCardUI.getPaymentCard();
		bool itineraryBooked = bookingManager.book(itinerary, cardInfo, getUserData());
		cout << "\n";

		if (itineraryBooked) {
			cout << "Itinerary reserved\n";
			cout << "Money withdraw successfully\n";
			cout << "Reservation confirmed\n";
		} else {
			cout << "Couldn't Reserve Itinerary\n";
		}

		cout << "\n";
	}

	void listUserItineraries() {
		vector<ItineraryData> itineraries = bookingManager.getItineraries(getUserData());
		if (itineraries.empty()) {
			cout << "You don't have any itineraries\n\n";
			return;
		}

		for (const auto &itinerary: itineraries)
			cout << itinerary.toString() << endl;
	}

public:
	CustomerUserUI(const UserData &userData) : 
		UserUI(userData),
		bookingManager(BookingManagerFactory::getBookingManger()),
		paymentCardUI(userData)
	{
		if (userData.userType != "CUSTOMER")
			throw invalid_argument("Invalid User Type, expected CUSTOMER user");
	}

	virtual void runUI() override {
		cout << "Hello " << getUserData().id << " | Customer View\n\n";
		while (true) {
			int choice = readMenuOption({"View Profile", "Make Itinerary", "List my itineraries", "Logout"});
			cout << "\n";

			if (choice == 1)
				viewProfile();
			else if (choice == 2)
				makeItinerary();
			else if (choice == 3)
				listUserItineraries();
			else if (choice == 4)
				break;
		}
	}
};

#endif