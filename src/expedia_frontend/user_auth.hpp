#include "../utils/utils.hpp"
#include "user.hpp"
#include "managers/abstract_managers.hpp"
#include "customer_user.hpp"
#include "admin_user.hpp"

#ifndef USER_AUTH_UI_HPP_
#define USER_AUTH_UI_HPP_

class UserAuthLandingPageUI {
private:
	IUserAuthenticationManager &userAuthManager;

private:
	UserUI* convertToUserUI(const UserData &userData) {
		string userType = userData.userType;
		if (userType == "CUSTOMER")
			return new CustomerUserUI(userData);
		else if (userType == "ADMIN")
			return new AdminUserUI(userData);
		else {
			cout << "Unrecognized User Type. Please contact technical support\n";
			return nullptr;
		}
	}

	UserUI* handleAuthenticationStatus(AuthStatusData &status, const string &password) {
		if (!status.authenticated) {
			cout << status.responseMessage << "\n";
			return nullptr;
		}

		cout << "Authentication Success\n";
		status.userDataResponse.password = password; // password is not returned by the backend that is why it is handled and stored here by the client-side (i.e. Frontend)
		return convertToUserUI(status.userDataResponse);
	}

	UserUI* login() {
		cout << "Enter username & password: ";
		string username, password;
		cin >> username >> password;

		AuthStatusData status = userAuthManager.login({.username = username, .password = password});
		return handleAuthenticationStatus(status, password);
	}

	UserUI* signup() {
		cout << "Enter username, email, password: ";
		string username, email, password;
		cin >> username >> email >> password;

		// when using the signup functionality by default its is a Customer (as Admins don't signup through the frontend)
		AuthStatusData status = userAuthManager.signup({.email = email, .username = username, .password = password, .userType = "CUSTOMER"});
		return handleAuthenticationStatus(status, password);
	}

public:
	UserAuthLandingPageUI() : userAuthManager(UserAuthManagerFactory::getUserAuthManager()) {}

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