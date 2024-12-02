#include <stdexcept>
using namespace std;

#include "../expedia_core_api/expedia_booking_api.hpp"
#include "user.hpp"

#ifndef CUSTOMER_USER_UI_HPP_
#define CUSTOMER_USER_UI_HPP_

class CustomerUserUI: public UserUI {
private:
	ExpediaBookingAPI expediaBookingAPI;

public:
	CustomerUserUI(const UserInfo &userInfo) : UserUI(userInfo) {
		if (userInfo.getUserType() != UserType::CUSTOMER)
			throw invalid_argument("Invalid User Type, expected CUSTOMER user");
	}

	virtual void runUI() override {
		cout << "Customer User still under development\n";
	}
};

#endif