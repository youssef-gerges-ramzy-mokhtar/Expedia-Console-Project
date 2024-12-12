#include <stdexcept>
using namespace std;

#include "../expedia_core_api/expedia_booking_api.hpp"
#include "../expedia_core_api/expedia_flight_api.hpp"
#include "../expedia_core_api/expedia_hotel_api.hpp"
#include "../expedia_reservation_logic/flight_reservation.hpp"
#include "../expedia_reservation_logic/hotel_reservation.hpp"
#include "user.hpp"

#ifndef CUSTOMER_USER_UI_HPP_
#define CUSTOMER_USER_UI_HPP_

class ItineraryMakerUI {
private:
	ExpediaFlightAPI flightAPI;
	ExpediaHotelAPI hotelAPI;

private:
	FlightReservationItem* addFlight() {
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

		FlightRequest flightRequest = {fromDate, toDate, from, to, adults, children, infants};
		vector<FlightInfo> searchResults = flightAPI.search(flightRequest);

		for (int i = 0; i < searchResults.size(); i++) {
			const auto &flight = searchResults[i];
			cout << i + 1 << "-"
				<< " Airline: " << flight.getAirlineName()
				<< " Cost: " << flight.getPrice()
				<< " Departure Date: " << flight.getDepartureDateTime()
				<< " Arrival Date: " << flight.getArrivalDateTime() << "\n";
		}

		int choice;
		cout << "\nEnter -1 to cancel or number in range 1 - " << searchResults.size() << ": ";
		cin >> choice;
		cout << "\n";

		if (choice == -1)
			return nullptr;

		assert(1 <= choice && choice <= searchResults.size());
		return new FlightReservationItem(searchResults[choice -1], flightRequest);
	}

	HotelReservationItem* addHotel() {
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

		HotelRequest hotelRequest = {fromDate, toDate, country, city, adults, children, neededRooms};
		vector<HotelRoomInfo> searchResults = hotelAPI.search(hotelRequest);

		for (int i = 0; i < searchResults.size(); i++) {
			const auto &hotel = searchResults[i];
			cout << i + 1 << "-"
				<< " Hotel: " << hotel.getHotelName()
				<< " Room Type: " << hotel.getRoomType() << "(" << hotel.getAvailableRooms() << ")"
				<< " Price per night: " << hotel.getPricePerNight()
				<< " From Date: " << hotel.getFromDate()
				<< " to " << hotel.getToDate() << "\n";
		}

		int choice;
		cout << "\nEnter -1 to cancel or number in range 1 - " << searchResults.size() << ": ";
		cin >> choice;
		cout << "\n";

		if (choice == -1)
			return nullptr;

		assert(1 <= choice && choice <= searchResults.size());
		return new HotelReservationItem(searchResults[choice -1], hotelRequest);
	}

public:
	Itinerary createItinerary() {
		Itinerary itinerary;

		while (true) {
			int choice = readMenuOption({"Add Flight", "Add Hotel", "Done", "Cancel"});
			cout << "\n";

			IReservationItem* item = nullptr;
			if (choice == 1)
				item = addFlight();
			else if (choice == 2)
				item = addHotel();
			else if (choice == 3)
				break;
			else if (choice == 4)
				return Itinerary(); // empty Itinerary

			if (item != nullptr)
				itinerary.addReservationItem(item);
		}

		return itinerary;
	}
};

class PaymentCardUI {
private:
	ExpediaBookingAPI &expediaBookingAPI;
	UserInfo userInfo;

private:
	bool userHasPaymentCards() {
		return expediaBookingAPI.getUserPaymentCards(userInfo).size() > 0;
	}

	CardInfo getPaymentCardSelectedByUser() {
		vector<CardInfo> cards = expediaBookingAPI.getUserPaymentCards(userInfo);
		
		cout << "Select a payment choice:\n";
		for (int i = 0; i < cards.size(); i++) {
			auto &card = cards[i];
			cout << i+1 << ":" << " Owner: " << card.getName() << " Card number: " << card.getId() << "\n";
		}

		int choice;
		cout << "\nEnter number in range 1 - " << cards.size() << ": ";
		cin >> choice;

		assert(1 <= choice && choice <= cards.size());
		return cards[choice-1];
	}

	CardInfo addPaymentCard() {
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

		CardInfo cardInfo = {name, address, cardNumber, expiryDate, cvv};
		expediaBookingAPI.addPaymentCard(userInfo, cardInfo);
		return cardInfo;
	}

public:
	PaymentCardUI(ExpediaBookingAPI &expediaBookingAPI, const UserInfo &userInfo) : 
		expediaBookingAPI(expediaBookingAPI), userInfo(userInfo) 
	{}

	CardInfo getPaymentCard() {
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
	ExpediaBookingAPI expediaBookingAPI;
	ItineraryMakerUI itineraryMakerUI;
	PaymentCardUI paymentCardUI;

private:
	// still under implementation
	void makeItinerary() {
		Itinerary itinerary = itineraryMakerUI.createItinerary();
		if (itinerary.getAllReservation().size() == 0)
			return;

		CardInfo cardInfo = paymentCardUI.getPaymentCard();
		bool itineraryBooked = expediaBookingAPI.book(itinerary, cardInfo, getUserInfo());
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
		vector<Itinerary> itineraries = expediaBookingAPI.getUserBookedItineraries(getUserInfo());
		if (itineraries.size() == 0) {
			cout << "You don't have any itineraries\n\n";
			return;
		}

		for (auto &itinerary: itineraries)
			cout << itinerary.toString() << "\n";
	}

public:
	CustomerUserUI(const UserInfo &userInfo) : UserUI(userInfo), paymentCardUI(expediaBookingAPI, userInfo) {
		if (userInfo.getUserType() != UserType::CUSTOMER)
			throw invalid_argument("Invalid User Type, expected CUSTOMER user");
	}

	virtual void runUI() override {
		cout << "Hello " << getUserInfo().getUserId() << " | Customer View\n\n";
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