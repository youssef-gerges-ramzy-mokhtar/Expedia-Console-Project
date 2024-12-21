using namespace std;

#include "../expedia_core_api/expedia_booking_api.hpp"
#include "expedia_user_authentication_rest_api.hpp"
#include "../expedia_reservation_logic/flight_reservation.hpp"
#include "../expedia_reservation_logic/hotel_reservation.hpp"
#include "../utils/json.hpp"

#ifndef EXPEDIA_BOOKING_REST_API_HPP_
#define EXPEDIA_BOOKING_REST_API_HPP_

class ReservationToJsonVisitor: public IReservationVisitor {
private:
	string reservationJson;

private:
	// add common json keys for types of Reservation Items and saves the result
	void addCommonJsonKeys(IReservationItem &reservationItem, json::JSON &res, const string &category) {
		// add the total cost of the reservation
		res["cost"] = reservationItem.totalCost();
	
		// add the category of the reservation item
		res["category"] = category;

	    // Convert JSON to string and store it
		this->reservationJson = res.dump();
	}

public:
	/**
	 * {
	 * 		flightInfo: {airlineName: string, flightNumber: string, departureDateTime: string, arrivalDateTime: string, origin: string, destination: string, price: double}
	 * 		flightRequest: {adults: int, children: int, infants: int},
	 * 		cost: double,
	 * 		category: string
	 * }
	 */
	virtual void visit(FlightReservationItem &flightReservation) override {
		const FlightInfo &flightInfo = flightReservation.getFlightInfo();
    	const FlightRequest &flightRequest = flightReservation.getFlightRequest();

	    json::JSON res;

	    // add the Flight Info to the json
	    res["flightInfo"] = {
	        "airlineName", flightInfo.getAirlineName(),
	        "flightNumber", flightInfo.getFlightNumber(),
	        "departureDateTime", flightInfo.getDepartureDateTime(),
	        "arrivalDateTime", flightInfo.getArrivalDateTime(),
	        "origin", flightInfo.getOrigin(),
	        "destination", flightInfo.getDestination(),
	        "price", flightInfo.getPrice()
	    };

	    // add the Flight Request to the json
	    res["flightRequest"] = {
	        "adults", flightRequest.getAdults(),
	        "children", flightRequest.getChildren(),
	        "infants", flightRequest.getInfants()
	    };

	    addCommonJsonKeys(flightReservation, res, "flights");
	}

	/**
	 * {
	 * 		roomInfo: {hotelName: string, fromDate: string, toDate: string, country: string, city: string, roomType: string, availableRooms: int, pricePerNight: double},
	 * 		roomRequest: {adults: int, children: int, neededRooms: int},
	 * 		cost: double,
	 * 		category: string
	 * }
	 */
	virtual void visit(HotelReservationItem &hotelReservation) override {
		const HotelRoomInfo &roomInfo = hotelReservation.getHotelRoomInfo();
		const HotelRequest &roomRequest = hotelReservation.getHotelRequest();

		json::JSON res;
		
		// add the Hotel Info to the json
		res["roomInfo"] = {
			"hotelName", roomInfo.getHotelName(),
			"fromDate", roomInfo.getFromDate(),
			"toDate", roomInfo.getToDate(),
			"country", roomInfo.getCountry(),
			"city", roomInfo.getCity(),
			"roomType", roomInfo.getRoomType(),
			"availableRooms", roomInfo.getAvailableRooms(),
			"pricePerNight", roomInfo.getPricePerNight()
		};

		// add the Hotel Request to the json
		res["roomRequest"] = {
			"adults", roomRequest.getAdults(),
			"children", roomRequest.getChildren(),
			"neededRooms", roomRequest.getNeededRooms()
		};

		addCommonJsonKeys(hotelReservation, res, "hotels");
	}
	
	/**
	 *	{
	 * 		flights: FlightReservation[],
	 * 		hotels: HotelReservation[],
	 * 		subItineraries: ItineraryReservation[],
	 * 		cost: double,
	 * 		category: string
	 * 	}
	 * 
	 * Where:
	 * - FlightReservation is the json schema of Flight Reservations
	 * - HotelReservation is the json schema of Hotel Reservations
	 * - ItineraryReservation is the json schema of Itinerary Reservations (note this is a recursive structure for itineraries that are made up of sub-itineraries)
	 */
	virtual void visit(Itinerary &itinerary) override {
		// Retrieve all reservations (flights, hotels, sub-itineraries, etc.) in this itinerary
		const vector<IReservationItem*>& reservationItems = itinerary.getAllReservations();
		json::JSON res;

		// convert each reservation item to json (using the visitor design pattern)
		for (const auto &item: reservationItems) {
			ReservationToJsonVisitor jsonVisitor;
			item->accept(jsonVisitor);
			json::JSON reservationJson = json::JSON::Load(jsonVisitor.getJson());

			// extract the category of the reservation item (to group reservation items of the same category in a single array)
			string categoryName = reservationJson["category"].ToString();
			
			// if the current sub-item is an itinerary, rename the category name to subItineraries for clarity in nested JSON
			if (categoryName == "itinerary")
				categoryName = "subItineraries";

			// initialize the categoryName key with an arrary if it doesn't exist
			if (!res.hasKey(categoryName))
				res[categoryName] = json::Array();

			res[categoryName].append(reservationJson);
		}

		addCommonJsonKeys(itinerary, res, "itinerary");
	}

	string getJson() const {
		return reservationJson;
	}
};

class JsonToReservation {
private:
	vector<FlightReservationItem*> toFlights(json::JSON req) {
		if (!req.hasKey("flights"))
			return {};

		vector<FlightReservationItem*> flights;
		for (int i = 0; i < req["flights"].size(); i++) {
			json::JSON flightInfoJson = req["flights"][i]["flightInfo"];
			json::JSON flightRequestJson = req["flights"][i]["flightRequest"];

			FlightInfo flightInfo = {
				flightInfoJson["airlineName"].ToString(),
				flightInfoJson["flightInfoJson"].ToString(),
				flightInfoJson["departureDateTime"].ToString(),
				flightInfoJson["arrivalDateTime"].ToString(),
				flightInfoJson["origin"].ToString(),
				flightInfoJson["destination"].ToString(),
				flightInfoJson["price"].ToFloat()
			};

			FlightRequest flightRequest = {
				flightInfoJson["departureDateTime"].ToString(),
				flightInfoJson["arrivalDateTime"].ToString(),
				flightInfoJson["origin"].ToString(),
				flightInfoJson["destination"].ToString(),
				flightRequestJson["adults"].ToInt(),
				flightRequestJson["infants"].ToInt(),
				flightRequestJson["children"].ToInt()
			};

			flights.push_back(new FlightReservationItem(flightInfo, flightRequest));
		}

		return flights;
	}

	vector<HotelReservationItem*> toHotels(json::JSON req) {
		if (!req.hasKey("hotels"))
			return {};

		vector<HotelReservationItem*> hotels;
		for (int i = 0; i < req["hotels"].size(); i++) {
			json::JSON roomInfoJson = req["hotels"][i]["roomInfo"];
			json::JSON roomRequestJson = req["hotels"][i]["roomRequest"];

			HotelRoomInfo roomInfo = {
				roomInfoJson["hotelName"].ToString(),
				roomInfoJson["fromDate"].ToString(),
				roomInfoJson["toDate"].ToString(),
				roomInfoJson["country"].ToString(),
				roomInfoJson["city"].ToString(),
				roomInfoJson["roomType"].ToString(),
				roomInfoJson["availableRooms"].ToInt(),
				roomInfoJson["pricePerNight"].ToFloat()
			};

			HotelRequest roomRequest = {
				roomInfoJson["fromDate"].ToString(),
				roomInfoJson["toDate"].ToString(),
				roomInfoJson["country"].ToString(),
				roomInfoJson["city"].ToString(),
				roomRequestJson["adults"].ToInt(),
				roomRequestJson["children"].ToInt(),
				roomRequestJson["neededRooms"].ToInt()
			};

			hotels.push_back(new HotelReservationItem(roomInfo, roomRequest));
		}

		return hotels;
	}

public:
	Itinerary* toItinerary(json::JSON req) {
		Itinerary* itinerary = new Itinerary();

		// get all the different categories of reservation items from the json
		vector<FlightReservationItem*> flightReservations = toFlights(req);
		vector<HotelReservationItem*> hotelReservations = toHotels(req);
		vector<Itinerary*> subItineraries;

		// we call ourself recursively to convert sub-itineraries json to an Itinerary
		if (req.hasKey("subItineraries"))
			for (int i = 0; i < req["subItineraries"].size(); i++)
				subItineraries.push_back(toItinerary(req["subItineraries"][i]));

		// add all the reservation items to the itinerary object
		vector<IReservationItem*> reservations;
		reservations.insert(reservations.end(), flightReservations.begin(), flightReservations.end());
		reservations.insert(reservations.end(), hotelReservations.begin(), hotelReservations.end());
		reservations.insert(reservations.end(), subItineraries.begin(), subItineraries.end());

		for (auto &item: reservations)
			itinerary->addReservationItem(item);

		return itinerary;
	}
};

// uri endpoint: expedia/itinerary
class ExpediaBookingRestAPI {
private:
	ExpediaBookingAPI expediaBookingAPI;

private:
	// private constructor (singelton)
	ExpediaBookingRestAPI() {}

	CardInfo jsonToCardInfo(json::JSON paymentCard) {
		return {
			paymentCard["nameOnCard"].ToString(),
			paymentCard["address"].ToString(),
			paymentCard["id"].ToString(),
			paymentCard["expiryDate"].ToString(),
			paymentCard["cvv"].ToInt()
		};
	}

	string cardInfoToJson(const CardInfo &cardInfo) {
		json::JSON res;
		res["nameOnCard"] = cardInfo.getName();
		res["address"] = cardInfo.getAddress();
		res["id"] = cardInfo.getId();
		res["expiryDate"] = cardInfo.getExpiryDate();
		res["cvv"] = cardInfo.getCvv();

		return res.dump();
	}

public:
	// Disable copy constructor and assignment operator
	ExpediaBookingRestAPI(const ExpediaBookingRestAPI &other) = delete;
    ExpediaBookingRestAPI& operator=(const ExpediaBookingRestAPI &other) = delete;

    static ExpediaBookingRestAPI& getInstance() {
    	static ExpediaBookingRestAPI bookingRestAPI;
    	return bookingRestAPI;
    }

	/**
	 * expected json input:
	 * 		{
	 * 			"itinerary": ItineraryReservation,
	 * 			"userInfo": {username: string, password: string},
	 * 			"paymentCard": {nameOnCard: string, address: string, id: string, expiryDate: string, cvv: int}
	 * 		}
	 *
	 * expected json output:
	 * 		{"success": boolean, "msg": string}
	 * 
	 * Where:
	 * - ItineraryReservation is the json schema of Itinerary Reservation (you can find it defined in the ReservationToJsonVisitor class)
	 */
	string book(string bookingInfoJson) {
		json::JSON req = json::JSON::Load(bookingInfoJson);
		
		// check if the request is authentic
		auto [authenticated, authJson] = ExpediaAuthenticationRestAPI::getInstance().authenticateRequest(bookingInfoJson);
		if (!authenticated)
			return authJson.dump();

		// convert to Itinerary and CardInfo from JSON
		Itinerary *itinerary = JsonToReservation().toItinerary(req["itinerary"]);
		CardInfo paymentCard = jsonToCardInfo(req["paymentCard"]);

		// call the expedia book() api
		bool bookingSuccess = expediaBookingAPI.book(*itinerary, paymentCard, authJson["userInfo"]["id"].ToString());
	
		// create the response
		json::JSON res;
		res["success"] = bookingSuccess;
		res["msg"] = bookingSuccess ? "itinerary booked successfully" : "failed to book itinerary";

		// clean up memory and return response
		delete itinerary; 
		return res.dump();
	}

	/**
	 *  expected json input:
	 * 		{"userInfo": {"username": string, "password": string}}
	 * 
	 * expected json output:
	 * 		ItineraryReservation[]
	 * 
	 * Where:
	 * - ItineraryReservation is the json schema of Itinerary Reservation (you can find it defined in the ReservationToJsonVisitor class)
	 */
	string getUserItineraries(string userCredentialsJson) {
		// check if the request is authentic
		auto [authenticated, authJson] = ExpediaAuthenticationRestAPI::getInstance().authenticateRequest(userCredentialsJson);
		if (!authenticated)
			return authJson.dump();

		// call the expedia getUserBookedItineraries() api
		vector<Itinerary> itineraries = expediaBookingAPI.getUserBookedItineraries(authJson["userInfo"]["id"].ToString());

		// convert the vector of Itinerary to json
		json::JSON res = json::Array();
		for (auto &itinerary: itineraries) {
			ReservationToJsonVisitor jsonVisitor;
			itinerary.accept(jsonVisitor);
			res.append(json::JSON::Load(jsonVisitor.getJson()));
		}

		// return response
		return res.dump();
	}

	/**
	 * expected json input:
	 * 		{
	 * 			"userInfo": {username: string, password: string},
	 * 			"paymentCard": {nameOnCard: string, address: string, id: string, expiryDate: string, cvv: int}
	 * 		}
	 *
	 * expected json output:
	 * 		{"success": boolean}
	 */
	string addPaymentCard(string cardInfoJson) {
		json::JSON req = json::JSON::Load(cardInfoJson);

		// check if the request is authentic
		auto [authenticated, authJson] = ExpediaAuthenticationRestAPI::getInstance().authenticateRequest(cardInfoJson);
		if (!authenticated)
			return authJson.dump();

		// convert to CardInfo from JSON
		CardInfo paymentCard = jsonToCardInfo(req["paymentCard"]);

		// call the expedia addPaymentCard() api
		expediaBookingAPI.addPaymentCard(authJson["userInfo"]["id"].ToString(), paymentCard);

		// return response
		return R"({"success": true})";
	}

	/**
	 *  expected json input:
	 * 		{"userInfo": {"username": string, "password": string}}
	 * 
	 * expected json output:
	 * 		CardInfo[]
	 * 
	 * Where:
	 * - CardInfo is the json schema of Payment Cards (you can find it defined in the book() method)
	 */
	string getUserPaymentCards(string userCredentialsJson) {
		// check if the request is authentic
		auto [authenticated, authJson] = ExpediaAuthenticationRestAPI::getInstance().authenticateRequest(userCredentialsJson);
		if (!authenticated)
			return authJson.dump();

		// call the expedia getUserPaymentCards() api
		vector<CardInfo> paymentCards = expediaBookingAPI.getUserPaymentCards(authJson["userInfo"]["id"].ToString());

		// convert the vector of CardInfo to json
		json::JSON res = json::Array();
		for (auto &card: paymentCards)
			res.append(json::JSON::Load(cardInfoToJson(card)));

		// return response
		return res.dump();
	}
};

#endif