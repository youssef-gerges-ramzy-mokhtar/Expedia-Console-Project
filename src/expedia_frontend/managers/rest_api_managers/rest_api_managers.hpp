#include "../abstract_managers.hpp"
#include "../../../utils/json.hpp"
#include "../../../expedia_rest_api/expedia_flight_rest_api.hpp"
#include "../../../expedia_rest_api/expedia_hotel_rest_api.hpp"
#include "../../../expedia_rest_api/expedia_booking_rest_api.hpp"
#include "../../../expedia_rest_api/expedia_user_authentication_rest_api.hpp"

#ifndef REST_API_CUSTOMER_USER_MANAGER_HPP_
#define REST_API_CUSTOMER_USER_MANAGER_HPP_

static string userCredentialsToJson(const UserData &userData) {
	json::JSON userJson = {
		"username", userData.username,
		"password", userData.password
	};

	return userJson.dump();
}

class RestAPIFlightsManager: public IFlightsManager {
private:
	string toFlightRequestJson(const FlightData &flightData) {
		json::JSON flightRequestJson = {
			"flightRequest", {
				"departureDateTime", flightData.departureDateTime,
				"arrivalDateTime", flightData.arrivalDateTime,
				"origin", flightData.origin,
				"destination", flightData.destination,
				"adults", flightData.adultsReq,
				"children", flightData.childrenReq,
				"infants", flightData.infantsReq
			}
		};
	
		return flightRequestJson.dump();
	}

	vector<FlightData> jsonToFlightData(const string &json) {
		json::JSON flightsJson = json::JSON::Load(json)["flightInfo"];

		vector<FlightData> flightsData;
		for (int i = 0; i < flightsJson.size(); i++) {
			FlightData flightData;
			flightData.airlineName = flightsJson[i]["airlineName"].ToString();
			flightData.flightNumber = flightsJson[i]["flightNumber"].ToString();
			flightData.departureDateTime = flightsJson[i]["departureDateTime"].ToString();
			flightData.arrivalDateTime = flightsJson[i]["arrivalDateTime"].ToString();
			flightData.origin = flightsJson[i]["origin"].ToString();
			flightData.destination = flightsJson[i]["destination"].ToString();
			flightData.price = flightsJson[i]["price"].ToFloat();

			flightsData.push_back(flightData);
		}

		return flightsData;
	}

private:
	// private constructor (singelton)
	RestAPIFlightsManager() {}

public:
	// Disable copy constructor and assignment operator
	RestAPIFlightsManager(const RestAPIFlightsManager &other) = delete;
    RestAPIFlightsManager& operator=(const RestAPIFlightsManager &other) = delete;

    static RestAPIFlightsManager& getInstance() {
    	static RestAPIFlightsManager flightsManager;
    	return flightsManager;
    }

	virtual vector<FlightData> searchFlights(const FlightData &flightData) override {
		// call flight search REST api
		string flightRes = ExpediaFlightRestAPI::getInstance().search(toFlightRequestJson(flightData));
		return jsonToFlightData(flightRes);
	}
};

class RestAPIHotelsManager: public IHotelsManager {
private:
	string toHotelRequestJson(const HotelData &hotelData) {
		json::JSON hotelRequestJson = {
			"hotelRequest", {
				"fromDate", hotelData.fromDateReq,
				"toDate", hotelData.toDateReq,
				"country", hotelData.country,
				"city", hotelData.city,
				"adults", hotelData.adultsReq,
				"children", hotelData.childrenReq,
				"neededRooms", hotelData.neededRoomsReq
			}
		};
	
		return hotelRequestJson.dump();
	}

	vector<HotelData> jsonToHotelData(const string &json) {
		json::JSON hotelsJson = json::JSON::Load(json)["hotelRoomInfo"];

		vector<HotelData> hotelsData;
		for (int i = 0; i < hotelsJson.size(); i++) {
			HotelData hotelData;
			hotelData.hotelName = hotelsJson[i]["hotelName"].ToString();
			hotelData.fromAvailableDate = hotelsJson[i]["fromDate"].ToString();
			hotelData.toAvailableDate = hotelsJson[i]["toDate"].ToString();
			hotelData.country = hotelsJson[i]["country"].ToString();
			hotelData.city = hotelsJson[i]["city"].ToString();
			hotelData.roomType = hotelsJson[i]["roomType"].ToString();
			hotelData.availableRooms = hotelsJson[i]["availableRooms"].ToInt();
			hotelData.pricePerNight = hotelsJson[i]["pricePerNight"].ToFloat();
		
			hotelsData.push_back(hotelData);
		}

		return hotelsData;
	}

private:
	// private constructor (singelton)
	RestAPIHotelsManager() {}

public:
	// Disable copy constructor and assignment operator
	RestAPIHotelsManager(const RestAPIHotelsManager &other) = delete;
    RestAPIHotelsManager& operator=(const RestAPIHotelsManager &other) = delete;

    static RestAPIHotelsManager& getInstance() {
    	static RestAPIHotelsManager hotelsManager;
    	return hotelsManager;
    }

	virtual vector<HotelData> searchHotels(const HotelData &hotelData) override {
		// call hotel search REST api
		string hotelRes = ExpediaHotelRestAPI::getInstance().search(toHotelRequestJson(hotelData));
		return jsonToHotelData(hotelRes);
	}
};

class RestAPIBookingManager: public IBookingManager {
private:
	string itineraryToJson(const ItineraryData &itinerary) {
		json::JSON itineraryJson;

		itineraryJson["flights"] = json::Array();
		for (const auto &flight: itinerary.flights) {
			json::JSON flightJson = {
				"flightInfo", {
					"airlineName", flight.airlineName,
					"flightNumber", flight.flightNumber,
					"departureDateTime", flight.departureDateTime,
					"arrivalDateTime", flight.arrivalDateTime,
					"origin", flight.origin,
					"destination", flight.destination,
					"price", flight.price
				},
				"flightRequest", {
					"adults", flight.adultsReq,
					"children", flight.childrenReq,
					"infants", flight.infantsReq
				}
			};
			itineraryJson["flights"].append(flightJson);
		}

		itineraryJson["hotels"] = json::Array();
		for (const auto &hotel: itinerary.hotels) {
			json::JSON hotelJson = {
				"roomInfo", {
					"hotelName", hotel.hotelName,
					"fromDate", hotel.fromAvailableDate,
					"toDate", hotel.toAvailableDate,
					"country", hotel.country,
					"city", hotel.city,
					"roomType", hotel.roomType,
					"availableRooms", hotel.availableRooms,
					"pricePerNight", hotel.pricePerNight
				},
				"roomRequest", {
					"adults", hotel.adultsReq,
					"children", hotel.childrenReq,
					"neededRooms", hotel.neededRoomsReq
				}
			};
			itineraryJson["hotels"].append(hotelJson);
		}

		itineraryJson["subItineraries"] = json::Array();
		for (const auto &subItinerary: itinerary.subItineraries)
			itineraryJson["subItineraries"].append(json::JSON::Load(itineraryToJson(subItinerary)));

		return itineraryJson.dump();
	}

	ItineraryData jsonToItinerary(const string &json) {
		json::JSON itineraryJson = json::JSON::Load(json);
		ItineraryData itinerary;
		
		if (itineraryJson.hasKey("flights")) {
			for (int i = 0; i < itineraryJson["flights"].size(); i++) {
				json::JSON flightJson = itineraryJson["flights"][i];
				itinerary.flights.push_back({
					flightJson["flightInfo"]["airlineName"].ToString(),
					flightJson["flightInfo"]["flightNumber"].ToString(),
					flightJson["flightInfo"]["departureDateTime"].ToString(),
					flightJson["flightInfo"]["arrivalDateTime"].ToString(),
					flightJson["flightInfo"]["origin"].ToString(),
					flightJson["flightInfo"]["destination"].ToString(),
					flightJson["flightInfo"]["price"].ToFloat(),
					flightJson["flightRequest"]["adults"].ToInt(),
					flightJson["flightRequest"]["children"].ToInt(),
					flightJson["flightRequest"]["infants"].ToInt(),
					flightJson["cost"].ToFloat()
				});
			}
		}

		if (itineraryJson.hasKey("hotels")) {
			for (int i = 0; i < itineraryJson["hotels"].size(); i++) {
				json::JSON hotelJson = itineraryJson["hotels"][i];
				itinerary.hotels.push_back({
					hotelJson["roomInfo"]["hotelName"].ToString(),
					hotelJson["roomInfo"]["fromDate"].ToString(),
					hotelJson["roomInfo"]["toDate"].ToString(),
					hotelJson["roomInfo"]["country"].ToString(),
					hotelJson["roomInfo"]["city"].ToString(),
					hotelJson["roomInfo"]["roomType"].ToString(),
					hotelJson["roomInfo"]["availableRooms"].ToInt(),
					hotelJson["roomInfo"]["pricePerNight"].ToFloat(),
					hotelJson["roomInfo"]["fromDate"].ToString(),
					hotelJson["roomInfo"]["toDate"].ToString(),
					hotelJson["roomRequest"]["adults"].ToInt(),
					hotelJson["roomRequest"]["children"].ToInt(),
					hotelJson["roomRequest"]["neededRooms"].ToInt(),
					hotelJson["cost"].ToFloat()
				});
			}
		}

		if (itineraryJson.hasKey("subItineraries"))
			for (int i = 0; i < itineraryJson["subItineraries"].size(); i++)
				itinerary.subItineraries.push_back(jsonToItinerary(itineraryJson["subItineraries"][i].dump()));

		return itinerary;
	}

	string paymentCardToJson(const PaymentCardData &paymentCardData) {
		json::JSON paymentCardJson = {
			"nameOnCard", paymentCardData.name,
			"address", paymentCardData.address,
			"id", paymentCardData.id,
			"expiryDate", paymentCardData.expiryDate,
			"cvv", paymentCardData.cvv
		};

		return paymentCardJson.dump();
	}

	PaymentCardData jsonToPaymentCard(const string &json) {
		json::JSON paymentCardJson = json::JSON::Load(json);
		return {
			.name = paymentCardJson["nameOnCard"].ToString(),
			.address = paymentCardJson["address"].ToString(),
			.id = paymentCardJson["id"].ToString(),
			.expiryDate = paymentCardJson["expiryDate"].ToString(),
			.cvv = paymentCardJson["cvv"].ToInt()
		};
	}

private:
	// private constructor (singelton)
	RestAPIBookingManager() {}

public:
	// Disable copy constructor and assignment operator
	RestAPIBookingManager(const RestAPIBookingManager &other) = delete;
    RestAPIBookingManager& operator=(const RestAPIBookingManager &other) = delete;
	static RestAPIBookingManager& getInstance() {
    	static RestAPIBookingManager bookingManager;
    	return bookingManager;
    }

    virtual bool book(const ItineraryData &itinerary, const PaymentCardData &card, const UserData &user) override {
    	json::JSON bookingReq = {
    		"itinerary", json::JSON::Load(itineraryToJson(itinerary)),
    		"userInfo", json::JSON::Load(userCredentialsToJson(user)),
    		"paymentCard", json::JSON::Load(paymentCardToJson(card))
    	};

    	json::JSON res = json::JSON::Load(ExpediaBookingRestAPI::getInstance().book(bookingReq.dump()));
    	return res["success"].ToBool();
    }

	virtual vector<ItineraryData> getItineraries(const UserData &user) override {
		// call booking getUserItineraries REST api
		json::JSON userCredentialsJson = {
			"userInfo", json::JSON::Load(userCredentialsToJson(user))
		};
		json::JSON itinerariesJson = json::JSON::Load(
			ExpediaBookingRestAPI::getInstance().getUserItineraries(userCredentialsJson.dump())
		);

		cout << "itinerariesJson: " << itinerariesJson << endl;
		// convert the json to a vector of ItineraryData
		vector<ItineraryData> itineraries;
		for (int i = 0; i < itinerariesJson.size(); i++)
			itineraries.push_back(jsonToItinerary(itinerariesJson[i].dump()));

		return itineraries;
	}

	virtual bool addPaymentCard(const PaymentCardData &card, const UserData &user) override {
		json::JSON paymentCardJson = {
			"userInfo", json::JSON::Load(userCredentialsToJson(user)),
			"paymentCard", json::JSON::Load(paymentCardToJson(card))
		};

		json::JSON res = json::JSON::Load(ExpediaBookingRestAPI::getInstance().addPaymentCard(paymentCardJson.dump()));
		return res["success"].ToBool();
	}

	virtual vector<PaymentCardData> getPaymentCards(const UserData &user) override {
		json::JSON userCredentialsJson = {
			"userInfo", json::JSON::Load(userCredentialsToJson(user))
		};

		json::JSON paymentCardsJson = json::JSON::Load(ExpediaBookingRestAPI::getInstance().getUserPaymentCards(userCredentialsJson.dump()));
		
		// convert the json to a vector of PaymentCardData
		vector<PaymentCardData> paymentCards;
		for (int i = 0; i < paymentCardsJson.size(); i++)
			paymentCards.push_back(jsonToPaymentCard(paymentCardsJson[i].dump()));

		return paymentCards;
	}
};

class RestAPIUserAuthenticationManager: public IUserAuthenticationManager {
private:
	AuthStatusData jsonToAuthStatus(const string &json) {
		json::JSON authStatusJson = json::JSON::Load(json);
		
		AuthStatusData authStatus;
		authStatus.authenticated = authStatusJson["success"].ToBool();
		authStatus.responseMessage = authStatusJson["msg"].ToString();
		
		if (!authStatusJson["userInfo"].IsNull()) {
			authStatus.userDataResponse = {
				.id = authStatusJson["userInfo"]["id"].ToString(),
				.email = authStatusJson["userInfo"]["email"].ToString(),
				.username = authStatusJson["userInfo"]["username"].ToString(),
				.userType = authStatusJson["userInfo"]["userType"].ToString()
			};
		}

		return authStatus;
	}

	string userDataToJson(const UserData &userData) {
		json::JSON userJson = {
			"username", userData.username,
			"password", userData.password,
			"email", userData.email,
			"userType", userData.userType
		};

		return userJson.dump();
	}

private:
	// private constructor (singelton)
	RestAPIUserAuthenticationManager() {}

public:
	// Disable copy constructor and assignment operator
	RestAPIUserAuthenticationManager(const RestAPIUserAuthenticationManager &other) = delete;
    RestAPIUserAuthenticationManager& operator=(const RestAPIUserAuthenticationManager &other) = delete;

	static RestAPIUserAuthenticationManager& getInstance() {
    	static RestAPIUserAuthenticationManager userAuthManager;
    	return userAuthManager;
    }

	virtual AuthStatusData login(const UserData &userData) override {
		return jsonToAuthStatus(
			ExpediaAuthenticationRestAPI::getInstance().login(userCredentialsToJson(userData))
		);
	}

	virtual AuthStatusData signup(const UserData &userData) override{
		return jsonToAuthStatus(
			ExpediaAuthenticationRestAPI::getInstance().signup(userDataToJson(userData))
		);
	}
};

#endif