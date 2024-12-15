using namespace std;

#include "../expedia_core_api/expedia_flight_api.hpp"
#include "../internal_api_wrappers/external_apis/json.hpp"

#ifndef EXPEDIA_FLIGHT_REST_API_HPP_
#define EXPEDIA_FLIGHT_REST_API_HPP_

class ExpediaFlightRestAPI {
private:
	ExpediaFlightAPI expediaFlightAPI;

private:
	FlightRequest convertJsonToFlightRequest(const string &flightRequestJson) {
		json::JSON req = json::JSON::Load(flightRequestJson)["flightRequest"];
		return {
			req["departureDateTime"].ToString(),
			req["arrivalDateTime"].ToString(),
			req["origin"].ToString(),
			req["destination"].ToString(),
			req["adults"].ToInt(),
			req["children"].ToInt(),
			req["infants"].ToInt()
		};
	}

	json::JSON convertFlightInfoToJson(const FlightInfo &flightInfo) {
		json::JSON flightInfoJson;
        flightInfoJson["airlineName"] = flightInfo.getAirlineName();
        flightInfoJson["flightNumber"] = flightInfo.getFlightNumber();
        flightInfoJson["departureDateTime"] = flightInfo.getDepartureDateTime();
        flightInfoJson["arrivalDateTime"] = flightInfo.getArrivalDateTime();
        flightInfoJson["origin"] = flightInfo.getOrigin();
        flightInfoJson["destination"] = flightInfo.getDestination();
        flightInfoJson["price"] = flightInfo.getPrice();

        return flightInfoJson;
	}

public:
	/**
	 * expected json input:
	 * 		"flightRequest": {departureDateTime, arrivalDateTime, origin, destination, adults, children, infants}
	 * 
	 * expected json output:
	 * 		"flightInfo": [
	 * 			{airlineName, flightNumber, departureDateTime, arrivalDateTime, origin, destination, price}
	 * 		]
	 */
	string search(const string &flightRequestJson) {
		// convert the flightRequestJson to FlightRequest object
		FlightRequest flightRequest = convertJsonToFlightRequest(flightRequestJson);

		// call the expedia flight api (aka. core business logic)
		vector<FlightInfo> searchResults = expediaFlightAPI.search(flightRequest);

		// convert the returned result to json
		json::JSON res;
		res["flightInfo"] = json::Array();
		for (const auto &result: searchResults)
			res["flightInfo"].append(convertFlightInfoToJson(result));

		return res.dump();
	}
};

#endif