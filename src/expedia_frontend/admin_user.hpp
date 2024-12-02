#include "user.hpp"

#ifndef ADMIN_USER_UI_HPP_
#define ADMIN_USER_UI_HPP_

class AdminUserUI: public UserUI {
public:
	AdminUserUI(const UserInfo &userInfo) : UserUI(userInfo) {
		if (userInfo.getUserType() != UserType::ADMIN)
			throw invalid_argument("Invalid User Type, expected ADMIN user");
	}

	virtual void runUI() override {
		cout << "Admin User doesn't have any operations at the moment\n";
	}
};

#endif