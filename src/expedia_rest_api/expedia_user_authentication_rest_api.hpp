using namespace std;

#include "../expedia_core_api/user_authentication_api.hpp"
#include "../utils/json.hpp"

#ifndef EXPEDIA_USER_AUTHENTICATION_REST_API_HPP_
#define EXPEDIA_USER_AUTHENTICATION_REST_API_HPP_

// uri endpoint: expedia/auth
class ExpediaAuthenticationRestAPI {
private:
	ExpediaAuthenticationAPI expediaAuthenticationAPI;

private:
	string convertAuthenticationStatusToJson(const AuthenticationStatus &status) {
		using namespace json;
		JSON res;
	    res["msg"] = status.authenticationMsg();
	    res["success"] = status.authenticatedSuccessfully();
	    res["userInfo"] = JSON::Make(JSON::Class::Null);

	    if (status.authenticatedSuccessfully()) {
	        optional<UserInfo> userInfo = status.getUserInfo();
            JSON userJson;
            userJson["email"] = userInfo->getEmail();
            userJson["username"] = userInfo->getUserId();
            userJson["userType"] = (userInfo->getUserType() == UserType::CUSTOMER) ? "CUSTOMER" : "ADMIN";
            res["userInfo"] = userJson;
	    }

	    return res.dump();
	}

public:
	/**
	 *  expected json input:
	 * 		{"username": string, "password": string}
	 * 
	 * expected json output:
	 * 		{
	 * 			"msg": string,
	 * 			"success": boolean,
	 * 			"userInfo": {"email": string, "username": string, "userType": enum} | null
	 * 		}
	 * 
	 * note: userInfo is set to null if the authentication failed
	 */
	string login(string loginCredentialsJson) {
		// parse the login credentials json and call the expedia authentication api login method
		json::JSON loginReq = json::JSON::Load(loginCredentialsJson);
		AuthenticationStatus status = expediaAuthenticationAPI.login(loginReq["username"].ToString(), loginReq["password"].ToString());

		// convert the AuthenticationStatus object returned to json
		return convertAuthenticationStatusToJson(status);
	}

	/**
	 *  expected json input:
	 * 		{"email": string, "username": string, "password": string, "userType": string}
	 * 
	 * expected json output:
	 * 		{
	 * 			"msg": string,
	 * 			"success": boolean,
	 * 			"userInfo": {"email": string, "username": string, "userType": enum} | null
	 * 		}
	 * 
	 * note: userInfo is set to null if the authentication failed
	 */
	string signup(string signupCredentialsJson) {
		// parse the signup credentials json and call the expedia authentication api signup method
		json::JSON signupReq = json::JSON::Load(signupCredentialsJson);
		AuthenticationStatus status = expediaAuthenticationAPI.signup(
			signupReq["email"].ToString(),
			signupReq["username"].ToString(),
			signupReq["password"].ToString(),
			signupReq["userType"].ToString() == "CUSTOMER" ? UserType::CUSTOMER : UserType::ADMIN
		);

		// conver the AuthenticationStatus object returned to json
		return convertAuthenticationStatusToJson(status);
	}
};

#endif