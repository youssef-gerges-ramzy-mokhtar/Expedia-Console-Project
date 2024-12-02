#include "user_auth.hpp"

#ifndef EXPEDIA_FRONTEND_HPP_
#define EXPEDIA_FRONTEND_HPP_

class ExpediaFrontend {
private:
	UserAuthLandingPageUI landingPageUI;

public:
	void run() {
		while (true) {
			UserUI* user = landingPageUI.accessSystem();
			if (user == nullptr)
				break;
			
			user->runUI();
			delete user;
		}
	}
};

#endif