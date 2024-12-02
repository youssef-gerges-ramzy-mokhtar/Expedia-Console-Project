#include "../expedia_core_api/user_authentication_api.hpp"

#ifndef USER_UI_HPP_
#define USER_UI_HPP_

class UserUI {
private:
	UserInfo userInfo;

public:
	UserUI(const UserInfo &userInfo) : userInfo(userInfo) {}

	UserInfo getUserInfo() const {
		return userInfo;
	}

	virtual void viewProfile() const {
		string userType = "Customer";
		if (userInfo.getUserType() == UserType::ADMIN)
			userType = "Admin";

		cout << "UserName: " << userInfo.getUserId() << " | " << userType << "\n";
		cout << "Email: " << userInfo.getEmail() << "\n\n";
	}

	virtual void runUI() = 0;
};

#endif