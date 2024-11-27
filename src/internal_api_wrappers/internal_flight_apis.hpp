#include <string>
#include <vector>

#include "external_apis/external_flight_apis.h"
using namespace std;

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
	string getAirlineName() const {
		return airlineName;
	}

	string getFlightNumber() const {
        return flightNumber;
    }

    string getDepartureDateTime() const {
        return departureDateTime;
    }

    string getArrivalDateTime() const {
        return arrivalDateTime;
    }

    string getOrigin() const {
        return origin;
    }

    string getDestination() const {
        return destination;
    }

    double getPrice() const {
        return price;
    }
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
    string getDepartureDateTime() const {
        return departureDateTime;
    }

    string getArrivalDateTime() const {
        return arrivalDateTime;
    }

    string getOrigin() const {
        return origin;
    }

    string getDestination() const {
        return destination;
    }

    int getAdults() const {
        return adults;
    }

    int getInfants() const {
        return infants;
    }

    int getChildren() const {
        return children;
    }
};

class IFlightAPI {
public:
	virtual vector<FlightInfo> search(FlightRequest flightRequest) = 0;
	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) = 0;
	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) = 0;
}; 

// Concrete Internal Flight Wrappers
class TurkishFlightAPI: IFlightAPI {
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
			availableFlightsInternal.push_back({"turkish-airline", "n/a", flight.datetime_from, flight.datetime_to, flightRequest.getOrigin(), flightRequest.getDestination(), flight.cost});

		return availableFlightsInternal;
	}

	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return TurkishAirlinesOnlineAPI::ReserveFlight({}, getTurkishFlightObj(flightInfo));
	}

	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return TurkishAirlinesOnlineAPI::CancelReserveFlight({}, getTurkishFlightObj(flightInfo));
	}
};

class AirCanadaFlightAPI: IFlightAPI {
private:
	AirCanadaFlight getAirCanadaFlightObj(const FlightInfo &flightInfo) const {
		AirCanadaFlight airCanadaFlight = {flightInfo.getPrice(), flightInfo.getDepartureDateTime(), flightInfo.getArrivalDateTime()};
		return airCanadaFlight;
	}

public:
	virtual vector<FlightInfo> search(FlightRequest flightRequest) const {
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
			availableFlightsInternal.push_back({"air-canada-airline", "n/a", flight.date_time_from, flight.date_time_to, flightRequest.getOrigin(), flightRequest.getDestination(), flight.price});

		return availableFlightsInternal;
	}
	
	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return AirCanadaOnlineAPI::ReserveFlight(getAirCanadaFlightObj(flightInfo), {});
	}

	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return AirCanadaOnlineAPI::CancelReserveFlight(getAirCanadaFlightObj(flightInfo), {});
	}
};