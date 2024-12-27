#include "../abstract_managers.hpp"
#include "../../../expedia_reservation_logic/flight_reservation.hpp"
#include "../../../expedia_reservation_logic/hotel_reservation.hpp"
#include "../../../expedia_core_api/expedia_flight_api.hpp"
#include "../../../expedia_core_api/expedia_hotel_api.hpp"
#include "../../../expedia_core_api/expedia_booking_api.hpp"
#include "../../../expedia_core_api/user_authentication_api.hpp"

#ifndef LOCAL_API_MANAGERS_HPP_
#define LOCAL_API_MANAGERS_HPP_

class LocalAPIFlightsManager: public IFlightsManager {
private:
    ExpediaFlightAPI flightAPI;

private:
    vector<FlightData> flightInfoToFlightData(const vector<FlightInfo> &flightInfo) {
        vector<FlightData> flightsData;
        for (const auto &flight: flightInfo) {
            FlightData flightData;
            flightData.airlineName = flight.getAirlineName();
            flightData.flightNumber = flight.getFlightNumber();
            flightData.departureDateTime = flight.getDepartureDateTime();
            flightData.arrivalDateTime = flight.getArrivalDateTime();
            flightData.origin = flight.getOrigin();
            flightData.destination = flight.getDestination();
            flightData.price = flight.getPrice();
            flightsData.push_back(flightData);
        }

        return flightsData;
    }

private:
    // private constructor (singleton)
    LocalAPIFlightsManager() {}

public:
    // Disable copy constructor and assignment operator
    LocalAPIFlightsManager(const LocalAPIFlightsManager&) = delete;
    LocalAPIFlightsManager& operator=(const LocalAPIFlightsManager&) = delete;

    // Singleton instance
    static LocalAPIFlightsManager& getInstance() {
        static LocalAPIFlightsManager instance;
        return instance;
    }

public:
    static FlightRequest flightDataToFlightRequest(const FlightData &flightData) {
        return FlightRequest(flightData.departureDateTime, flightData.arrivalDateTime, flightData.origin, flightData.destination, flightData.adultsReq, flightData.infantsReq, flightData.childrenReq);
    }

    static FlightInfo flightDataToFlightInfo(const FlightData &flightData) {
        return FlightInfo(flightData.airlineName, flightData.flightNumber, flightData.departureDateTime, flightData.arrivalDateTime, flightData.origin, flightData.destination, flightData.price);
    }

    static FlightData flightReservationItemToFlightData(FlightReservationItem &flightReservationItem) {
        const FlightInfo &flightInfo = flightReservationItem.getFlightInfo();
        const FlightRequest &flightRequest = flightReservationItem.getFlightRequest();

        return {
            flightInfo.getAirlineName(),
            flightInfo.getFlightNumber(),
            flightInfo.getDepartureDateTime(),
            flightInfo.getArrivalDateTime(),
            flightInfo.getOrigin(),
            flightInfo.getDestination(),
            flightInfo.getPrice(),
            
            flightRequest.getAdults(),
            flightRequest.getInfants(),
            flightRequest.getChildren(),
            
            flightReservationItem.totalCost()
        };
    }

    virtual vector<FlightData> searchFlights(const FlightData &flightData) override {
        return flightInfoToFlightData(
            flightAPI.search(flightDataToFlightRequest(flightData))
        );
    }
};

class LocalAPIHotelsManager: public IHotelsManager {
private:
    ExpediaHotelAPI hotelAPI;

private:
    vector<HotelData> hotelInfoToHotelData(const vector<HotelRoomInfo> &hotelsInfo) {
        vector<HotelData> hotelsData;
        for (const auto &hotel: hotelsInfo) {
            HotelData hotelData;
            hotelData.hotelName = hotel.getHotelName();
            hotelData.fromAvailableDate = hotel.getFromDate();
            hotelData.toAvailableDate = hotel.getToDate();
            hotelData.country = hotel.getCountry();
            hotelData.city = hotel.getCity();
            hotelData.roomType = hotel.getRoomType();
            hotelData.availableRooms = hotel.getAvailableRooms();
            hotelData.pricePerNight = hotel.getPricePerNight();
            hotelsData.push_back(hotelData);
        }

        return hotelsData;
    }

private:
    // private constructor (singleton)
    LocalAPIHotelsManager() {}

public:
    // Disable copy constructor and assignment operator
    LocalAPIHotelsManager(const LocalAPIHotelsManager&) = delete;
    LocalAPIHotelsManager& operator=(const LocalAPIHotelsManager&) = delete;

    // Singleton instance
    static LocalAPIHotelsManager& getInstance() {
        static LocalAPIHotelsManager instance;
        return instance;
    }

public:
    static HotelRequest hotelDataToHotelRequest(const HotelData &hotelData) {
        return HotelRequest(hotelData.fromDateReq, hotelData.toDateReq, hotelData.country, hotelData.city, hotelData.adultsReq, hotelData.childrenReq, hotelData.neededRoomsReq);
    }

    static HotelRoomInfo hotelDataToHotelInfo(const HotelData &hotelData) {
        return HotelRoomInfo(hotelData.hotelName, hotelData.fromAvailableDate, hotelData.toAvailableDate, hotelData.country, hotelData.city, hotelData.roomType, hotelData.availableRooms, hotelData.pricePerNight);
    }

    static HotelData hotelReservationItemToHotelData(HotelReservationItem &hotelReservationItem) {
        const HotelRoomInfo &hotelInfo = hotelReservationItem.getHotelRoomInfo();
        const HotelRequest &hotelRequest = hotelReservationItem.getHotelRequest();

        return {
            hotelInfo.getHotelName(),
            hotelInfo.getFromDate(),
            hotelInfo.getToDate(),
            hotelInfo.getCountry(),
            hotelInfo.getCity(),
            hotelInfo.getRoomType(),
            hotelInfo.getAvailableRooms(),
            hotelInfo.getPricePerNight(),

            hotelInfo.getFromDate(), // that is due to limited support from the backend HotelRequest data
            hotelInfo.getToDate(), // that is due to limited support from the backend HotelRequest data
            hotelRequest.getAdults(),
            hotelRequest.getChildren(),
            hotelRequest.getNeededRooms(),

            hotelReservationItem.totalCost()
        };
    }

    virtual vector<HotelData> searchHotels(const HotelData &hotelData) override {
        return hotelInfoToHotelData(
            hotelAPI.search(hotelDataToHotelRequest(hotelData))
        );
    }
};

class LocalAPIBookingManager: public IBookingManager {
private:
    ExpediaBookingAPI bookingAPI;

private:
    class ReservationToItineraryDataVisitor: public IReservationVisitor {
        private:
            ItineraryData itineraryData;

        public:
            virtual void visit(FlightReservationItem &flightReservationItem) override {
                itineraryData.flights.push_back(LocalAPIFlightsManager::flightReservationItemToFlightData(flightReservationItem));
            }

            virtual void visit(HotelReservationItem &hotelReservationItem) override {
                itineraryData.hotels.push_back(LocalAPIHotelsManager::hotelReservationItemToHotelData(hotelReservationItem));
            }

            virtual void visit(Itinerary &itinerary) override {
                vector<IReservationItem*> reservationsCopy;
                for (const auto &reservationItem: itinerary.getAllReservations())
                    reservationsCopy.push_back(reservationItem->clone());

                for (auto &reservationItem: reservationsCopy) {
                    // check if the sub-reservation item is an itinerary
                    if (Itinerary* subItinerary = dynamic_cast<Itinerary*>(reservationItem)) {
                        ReservationToItineraryDataVisitor subItineraryVisitor;
                        subItinerary->accept(subItineraryVisitor);
                        itineraryData.subItineraries.push_back(subItineraryVisitor.getItineraryData());
                    } else {
                        reservationItem->accept(*this);
                    }
                }

                itineraryData.cost = itinerary.totalCost();
            }

            ItineraryData getItineraryData () {
                return itineraryData;
            }
    };

private:
    Itinerary* itineraryDataToItinerary(const ItineraryData &itineraryData) {
        Itinerary* itinerary = new Itinerary();

        for (const auto &flight: itineraryData.flights)
            itinerary->addReservationItem(new FlightReservationItem(LocalAPIFlightsManager::flightDataToFlightInfo(flight), LocalAPIFlightsManager::flightDataToFlightRequest(flight)));

        for (const auto &hotel: itineraryData.hotels)
            itinerary->addReservationItem(new HotelReservationItem(LocalAPIHotelsManager::hotelDataToHotelInfo(hotel), LocalAPIHotelsManager::hotelDataToHotelRequest(hotel)));

        for (const auto &subItinerary: itineraryData.subItineraries)
            itinerary->addReservationItem(itineraryDataToItinerary(subItinerary));

        return itinerary;
    }

    CardInfo paymentCardDataToCardInfo(const PaymentCardData &paymentCardData) {
        return CardInfo(paymentCardData.name, paymentCardData.address, paymentCardData.id, paymentCardData.expiryDate, paymentCardData.cvv);
    }

    PaymentCardData cardInfoToPaymentCardData(const CardInfo &cardInfo) {
        return {cardInfo.getName(), cardInfo.getAddress(), cardInfo.getId(), cardInfo.getExpiryDate(), cardInfo.getCvv()};
    }

private:
    // private constructor (singleton)
    LocalAPIBookingManager() {}

public:
    // Disable copy constructor and assignment operator
    LocalAPIBookingManager(const LocalAPIBookingManager&) = delete;
    LocalAPIBookingManager& operator=(const LocalAPIBookingManager&) = delete;

    // Singleton instance
    static LocalAPIBookingManager& getInstance() {
        static LocalAPIBookingManager instance;
        return instance;
    }

public:
    virtual bool book(const ItineraryData &itinerary, const PaymentCardData &card, const UserData &user) override {
        Itinerary* itineraryPtr = itineraryDataToItinerary(itinerary);
        bool bookingStatus = bookingAPI.book(*itineraryPtr, paymentCardDataToCardInfo(card), user.id);
        delete itineraryPtr;
        return bookingStatus;
    }

	virtual vector<ItineraryData> getItineraries(const UserData &user) override {
        vector<Itinerary> itineraries = bookingAPI.getUserBookedItineraries(user.id);
        
        vector<ItineraryData> itinerariesData;
        for (auto &itinerary: itineraries) {
            ReservationToItineraryDataVisitor itineraryVisitor;
            itinerary.accept(itineraryVisitor);
            itinerariesData.push_back(itineraryVisitor.getItineraryData());
        }

        return itinerariesData;
    }
	
    virtual bool addPaymentCard(const PaymentCardData &card, const UserData &user) override {
        bookingAPI.addPaymentCard(user.id, paymentCardDataToCardInfo(card));
        return true;
    }
	
    virtual vector<PaymentCardData> getPaymentCards(const UserData &user) override {
        vector<CardInfo> cardsInfo = bookingAPI.getUserPaymentCards(user.id);

        vector<PaymentCardData> paymentCardsData;
        for (const auto &cardInfo: cardsInfo)
            paymentCardsData.push_back(cardInfoToPaymentCardData(cardInfo));

        return paymentCardsData;
    }
};

class LocalAPIUserAuthenticationManager: public IUserAuthenticationManager {
private:
    ExpediaAuthenticationAPI userAuthAPI;

private:
    AuthStatusData userInfoToAuthStatusData(const AuthenticationStatus &authenticationStatus) {
        AuthStatusData authStatusData;
        authStatusData.authenticated = authenticationStatus.authenticatedSuccessfully();
        authStatusData.responseMessage = authenticationStatus.authenticationMsg();
        
        if (authenticationStatus.authenticatedSuccessfully()) {
            const UserInfo &userInfo = authenticationStatus.getUserInfo().value();
            authStatusData.userDataResponse = {
                .id = userInfo.getUserId(),
                .email = userInfo.getEmail(),
                .username = userInfo.getUserId(),
                .userType = userInfo.getUserType() == UserType::CUSTOMER ? "CUSTOMER" : "ADMIN"
            };
        }

        return authStatusData;
    }

private:
    // private constructor (singleton)
    LocalAPIUserAuthenticationManager() {}

public:
    // Disable copy constructor and assignment operator
    LocalAPIUserAuthenticationManager(const LocalAPIUserAuthenticationManager&) = delete;
    LocalAPIUserAuthenticationManager& operator=(const LocalAPIUserAuthenticationManager&) = delete;

    // Singleton instance
    static LocalAPIUserAuthenticationManager& getInstance() {
        static LocalAPIUserAuthenticationManager instance;
        return instance;
    }

public:
    virtual AuthStatusData login(const UserData &userData) override {
        return userInfoToAuthStatusData(userAuthAPI.login(userData.username, userData.password));
    }
    
	virtual AuthStatusData signup(const UserData &userData) override {
        UserType userType = userData.userType == "CUSTOMER" ? UserType::CUSTOMER : UserType::ADMIN;
        return userInfoToAuthStatusData(userAuthAPI.signup(userData.email, userData.username, userData.password, userType));
    }
};

#endif