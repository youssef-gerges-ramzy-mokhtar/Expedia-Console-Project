#include "rest_api_managers/rest_api_managers.hpp"
// #include ""

#ifndef MANAGERS_FACTORY_HPP_
#define MANAGERS_FACTORY_HPP_

/**
 * This module contains a bunch of factories to return a concerete type of Manager to be used by the View
 */

class FlightsManagerFactory {
public:
	static IFlightsManager& getFlightsManager() {
		return RestAPIFlightsManager::getInstance();
	}
};

class HotelsManagerFactory {
public:
	static IHotelsManager& getHotelsManager() {
		return RestAPIHotelsManager::getInstance();
	}
};

class BookingManagerFactory {
public:
	static IBookingManager& getBookingManger() {
		return RestAPIBookingManager::getInstance();
	}
};

class UserAuthManagerFactory {
public:
	static IUserAuthenticationManager& getUserAuthManager() {
		return RestAPIUserAuthenticationManager::getInstance();
	}
};

#endif