#include <string>
#include <vector>
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
	virtual vector<FlightInfo> search(FlightRequest flightRequest) const = 0;
	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) = 0;
	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) = 0;
}; 

// Concrete Internal Flight Wrappers
class TurkishFlightAPI: IFlightAPI {
public:
	virtual vector<FlightInfo> search(FlightRequest flightRequest) const {
		return {};// dummy
	}
	
	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return false;
	}

	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return false;
	}
};

class AirCanadaFlightAPI: IFlightAPI {
public:
	virtual vector<FlightInfo> search(FlightRequest flightRequest) const {
		return {};// dummy
	}
	
	virtual bool reserve(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return false;
	}

	virtual bool cancelReservation(FlightInfo flightInfo, FlightRequest FlightRequest) {
		return false;
	}
};