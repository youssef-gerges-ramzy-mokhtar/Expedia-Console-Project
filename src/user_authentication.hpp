#ifndef USER_MNGER_H_
#define USER_MNGER_H_

#include <string>
#include <optional>
#include <unordered_map>
#include "utils.hpp"
using namespace std;

enum UserType {
	CUSTOMER,
	ADMIN
};

class UserInfo {
private:
	string email;
	string username;
	string password;
	enum UserType user_type;

public:
	UserInfo() {}
	UserInfo(string email, string username, string password, enum UserType user_type) :
		email(email), username(username), password(password), user_type(user_type) {}

	string getUserId() const {
		return username; // for now the username is assumed to be unique
	}

	bool passwordMatch(const string &password) const {
		return this->password == password;
	}
};

class UserDAO {
private:
	// In real world users should be stored in a database or some permanant storage
	unordered_map<string, UserInfo> users;

public:
	bool addUser(UserInfo userInfo) {
		string id = userInfo.getUserId();
		if (userExist(id))
			return false;

		users[id] = userInfo;
		return true;
	}

	optional<UserInfo> getUser(const string &id) {
		if (!userExist(id))
			return {};

		return users[id];
	}

	bool userExist(const string &id) const {
		return users.count(id);
	}
};

class AuthenticationStatus {
private:
	optional<UserInfo> user_info;
	bool authenticated_successfully;
	string authentication_msg;

	AuthenticationStatus(optional<UserInfo> user_info, bool authenticated_successfully, string authentication_msg) :
		user_info(user_info), authenticated_successfully(authenticated_successfully), authentication_msg(authentication_msg) {}

public:
	static AuthenticationStatus failed(string reason) {
		return AuthenticationStatus(nullopt, false, reason);
	}

	static AuthenticationStatus success(UserInfo user_info, string reason) {
		return AuthenticationStatus(user_info, true, reason);
	}

	bool authenticatedSuccessfully() const {
		return authenticated_successfully;
	}

	string authenticationMsg() const {
		return authentication_msg;
	}
};

class Authentication {
private:
	UserDAO user_data_access_obj;

	bool strongPassword(const string &password) const {
		return (password.size() >= 8);
	}

public:
	AuthenticationStatus login(string username, string password) {
		if (!user_data_access_obj.userExist(username))
			return AuthenticationStatus::failed("user doesn't exist");

		optional<UserInfo> user = user_data_access_obj.getUser(username);
		if (!user->passwordMatch(password))
			return AuthenticationStatus::failed("incorrect password");

		return AuthenticationStatus::success(user.value(), "login success");
	}

	AuthenticationStatus signup(string email, string username, string password, UserType userType) {
		if (user_data_access_obj.userExist(username))
			return AuthenticationStatus::failed("user already exist");

		if (!strongPassword(password))
			return AuthenticationStatus::failed("password too weak");

		if (containSpaces(email) || containSpaces(username))
			return AuthenticationStatus::failed("email or username cannot contain spaces");

		UserInfo newUser(email, username, password, userType);
		bool added_successfully = user_data_access_obj.addUser(newUser);

		if (added_successfully)
			return AuthenticationStatus::success(newUser, "user created successfuclly");
		else
			return AuthenticationStatus::failed("internal server error couldn't store user. Try again");
	}
};

#endif