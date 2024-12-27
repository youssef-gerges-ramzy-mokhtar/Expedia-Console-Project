#include "rest_api_managers/rest_api_managers.hpp"
#include "local_api_managers/local_api_managers.hpp"
// #include ""

#ifndef MANAGERS_FACTORY_HPP_
#define MANAGERS_FACTORY_HPP_

/**
 * This module contains a bunch of factories to return a concerete type of Manager to be used by the View
 */

enum ManagerType {
	REST_API,
	LOCAL_API
};

static ManagerType activeManagerType = ManagerType::LOCAL_API;

class FlightsManagerFactory {
public:
	static IFlightsManager& getFlightsManager() {
		if (activeManagerType == ManagerType::REST_API)
			return RestAPIFlightsManager::getInstance();
		else
			return LocalAPIFlightsManager::getInstance();
	}
};

class HotelsManagerFactory {
public:
	static IHotelsManager& getHotelsManager() {
		if (activeManagerType == ManagerType::REST_API)
			return RestAPIHotelsManager::getInstance();
		else
			return LocalAPIHotelsManager::getInstance();
	}
};

class BookingManagerFactory {
public:
	static IBookingManager& getBookingManger() {
		if (activeManagerType == ManagerType::REST_API)
			return RestAPIBookingManager::getInstance();
		else
			return LocalAPIBookingManager::getInstance();
	}
};

class UserAuthManagerFactory {
public:
	static IUserAuthenticationManager& getUserAuthManager() {
		if (activeManagerType == ManagerType::REST_API)
			return RestAPIUserAuthenticationManager::getInstance();
		else
			return LocalAPIUserAuthenticationManager::getInstance();
	}
};

#endif