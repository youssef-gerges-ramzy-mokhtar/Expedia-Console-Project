#include "../expedia_core_api/user_authentication_api.hpp"
#include "../utils/utils.hpp"
#include "user.hpp"
#include "customer_user.hpp"
#include "admin_user.hpp"

#ifndef USER_AUTH_UI_HPP_
#define USER_AUTH_UI_HPP_

class UserAuthLandingPageUI {
private:
	ExpediaAuthenticationAPI expediaAuthenticationAPI;

private:
	UserUI* convertToUserUI(const UserInfo &userInfo) {
		UserType userType = userInfo.getUserType();
		if (userType == UserType::CUSTOMER)
			return new CustomerUserUI(userInfo);
		else if (userType == UserType::ADMIN)
			return new CustomerUserUI(userInfo);
		else {
			cout << "Unrecognized User Type. Please contact technical support\n";
			return nullptr;
		}
	}

	UserUI* handleAuthenticationStatus(const AuthenticationStatus &status) {
		if (!status.authenticatedSuccessfully()) {
			cout << status.authenticationMsg() << "\n";
			return nullptr;
		}

		cout << "Authentication Success\n";
		return convertToUserUI(status.getUserInfo().value());
	}

	UserUI* login() {
		cout << "Enter username & password: ";
		string username, password;
		cin >> username >> password;

		AuthenticationStatus status = expediaAuthenticationAPI.login(username, password);
		return handleAuthenticationStatus(status);
	}

	UserUI* signup() {
		cout << "Enter username, email, password: ";
		string username, email, password;
		cin >> username >> email >> password;

		// when using the signup functionality by default its is a Customer (as Admins don't signup through the frontend)
		AuthenticationStatus status = expediaAuthenticationAPI.signup(email, username, password, CUSTOMER);
		return handleAuthenticationStatus(status);
	}

public:
	UserUI* accessSystem() {
		while (true) {
			int choice = readMenuOption({"Login", "Sign Up", "Exit"});

			UserUI* user = nullptr;
			if (choice == 1)
				user = login();
			else if (choice == 2)
				user = signup();
			else if (choice == 3)
				return nullptr;

			cout << "\n";

			if (user != nullptr)
				return user;
		}
	}
};

#endif