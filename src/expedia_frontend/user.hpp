#include <iostream>
using namespace std;

#include "managers/manager_data_objects.hpp"

#ifndef USER_UI_HPP_
#define USER_UI_HPP_

class UserUI {
private:
	UserData userData;

public:
	UserUI(const UserData &userData) : userData(userData) {}

	UserData getUserData() const {
		return userData;
	}

	virtual void viewProfile() const {
		cout << "UserName: " << userData.id << " | " << userData.userType << "\n";
		cout << "Email: " << userData.email << "\n\n";
	}

	virtual void runUI() = 0;
};

#endif