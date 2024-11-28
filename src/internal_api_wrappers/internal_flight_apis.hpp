#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include "external_apis/external_flight_apis.h"
#include "api_factory.hpp"

#ifndef INTERNAL_FLIGHT_APIS_HPP_
#define INTERNAL_FLIGHT_APIS_HPP_

class FlightInfo {
private:
	string airlineName;
	string flightNumber;
	string departureDateTime;
	string arrivalDateTime;
	string origin;
	string destination;
	double price;

public:
	FlightInfo(
		const string &airlineName, const string &flightNumber, 
		const string &departureDateTime, const string &arrivalDateTime, 
		const string &origin, const string &destination, double price)
		: airlineName(airlineName), flightNumber(flightNumber),
		departureDateTime(departureDateTime), arrivalDateTime(arrivalDateTime),
		origin(origin), destination(destination), price(price) {}

	// Getters
	string getAirlineName() const { return airlineName; }
	string getFlightNumber() const { return flightNumber; }
    string getDepartureDateTime() const { return departureDateTime; }
    string getArrivalDateTime() const { return arrivalDateTime; }
    string getOrigin() const { return origin; }
    string getDestination() const { return destination; }
    double getPrice() const { return price; }
};

class FlightRequest {
private:
	string departureDateTime;
	string arrivalDateTime;
	string origin;
	string destination;
	int adults;
	int infants;
	int children;

public:
	FlightRequest(
        const string &departureDateTime, const string &arrivalDateTime,
        const string &origin, const string &destination,
        int adults, int infants, int children)
        : departureDateTime(departureDateTime), arrivalDateTime(arrivalDateTime),
          origin(origin), destination(destination),
          adults(adults), infants(infants), children(children) {}

    // Getters
    string getDepartureDateTime() const { return departureDateTime; }
    string getArrivalDateTime() const { return arrivalDateTime; }
    string getOrigin() const { return origin; }
    string getDestination() const { return destination; }
    int getAdults() const { return adults; }
    int getInfants() const { return infants; }
    int getChildren() const { return children; }
};

class IFlightAPI {
public:
	virtual vector<FlightInfo> search(FlightRequest flightRequest) = 0;
	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) = 0;
	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) = 0;
	virtual string getAirlineName() = 0;
}; 

// Concrete Internal Flight API Wrappers
class TurkishFlightAPI: public IFlightAPI {
private:
	TurkishAirlinesOnlineAPI turkishAPI;

	TurkishFlight getTurkishFlightObj(const FlightInfo &flightInfo) const {
		TurkishFlight turkishFlight = {flightInfo.getPrice(), flightInfo.getDepartureDateTime(), flightInfo.getArrivalDateTime()};
		return turkishFlight;
	}

public:
	virtual vector<FlightInfo> search(FlightRequest flightRequest) {
		// set info for external api
		turkishAPI.SetFromToInfo(
			flightRequest.getDepartureDateTime(),
			flightRequest.getOrigin(),
			flightRequest.getArrivalDateTime(),
			flightRequest.getDestination());

		turkishAPI.SetPassengersInfo(flightRequest.getInfants(), flightRequest.getChildren(), flightRequest.getAdults());

		// get available flights
		vector<TurkishFlight> availableFlightsExternal = turkishAPI.GetAvailableFlights();

		// Convert all TurkishFlight objects to FlightInfo objects
		vector<FlightInfo> availableFlightsInternal;
		for (auto flight: availableFlightsExternal)
			availableFlightsInternal.push_back({getAirlineName(), "n/a", flight.datetime_from, flight.datetime_to, flightRequest.getOrigin(), flightRequest.getDestination(), flight.cost});

		return availableFlightsInternal;
	}

	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return TurkishAirlinesOnlineAPI::ReserveFlight({}, getTurkishFlightObj(flightInfo));
	}

	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return TurkishAirlinesOnlineAPI::CancelReserveFlight({}, getTurkishFlightObj(flightInfo));
	}

	virtual string getAirlineName() {
		return "turkish-airline";
	}
};

class AirCanadaFlightAPI: public IFlightAPI {
private:
	AirCanadaFlight getAirCanadaFlightObj(const FlightInfo &flightInfo) const {
		AirCanadaFlight airCanadaFlight = {flightInfo.getPrice(), flightInfo.getDepartureDateTime(), flightInfo.getArrivalDateTime()};
		return airCanadaFlight;
	}

public:
	virtual vector<FlightInfo> search(FlightRequest flightRequest) {
		vector<AirCanadaFlight> availableFlightsExternal = AirCanadaOnlineAPI::GetFlights(
			flightRequest.getOrigin(), 
			flightRequest.getDepartureDateTime(), 
			flightRequest.getDestination(), 
			flightRequest.getArrivalDateTime(), 
			flightRequest.getAdults(), 
			flightRequest.getChildren() + flightRequest.getInfants()
		);

		// Convert all AirCanadaFlight objects to FlightInfo objects
		vector<FlightInfo> availableFlightsInternal;
		for (auto flight: availableFlightsExternal)
			availableFlightsInternal.push_back({getAirlineName(), "n/a", flight.date_time_from, flight.date_time_to, flightRequest.getOrigin(), flightRequest.getDestination(), flight.price});

		return availableFlightsInternal;
	}
	
	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return AirCanadaOnlineAPI::ReserveFlight(getAirCanadaFlightObj(flightInfo), {});
	}

	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return AirCanadaOnlineAPI::CancelReserveFlight(getAirCanadaFlightObj(flightInfo), {});
	}

	virtual string getAirlineName() {
		return "air-canada-airline";
	}
};


// Factory
class FlightAPIFactory: public APIFactory<IFlightAPI> {
private:
	// this is the best which I have could done it is not the best way because we need to modify the init() method each time a new implementation of the IFlightAPI interface is created. The issue is that C++ doesn't have mechanisms like Java Reflection which would have made this much more dynamic
	// another approach was to make this class a singelton and make every new IFlightAPI implementation register itself with the factory but that would be fine if we only have 1 type of factory but if we have different type of factories then that will be a bit challenging
	void init() {
		IFlightAPI* turkish = new TurkishFlightAPI();
		IFlightAPI* airCanada = new AirCanadaFlightAPI();
		addAPI(turkish->getAirlineName(), turkish);
		addAPI(airCanada->getAirlineName(), airCanada);
	}

public:
	FlightAPIFactory() {
		init();
	}

	IFlightAPI* createFlightAPI(const FlightInfo &flightInfo) {
		return createAPI(flightInfo.getAirlineName());
	}
};

#endif