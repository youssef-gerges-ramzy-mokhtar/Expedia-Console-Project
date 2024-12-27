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
	// private constructor (singelton)
	ExpediaAuthenticationRestAPI() {}

	string convertAuthenticationStatusToJson(const AuthenticationStatus &status) {
		using namespace json;
		JSON res;
	    res["msg"] = status.authenticationMsg();
	    res["success"] = status.authenticatedSuccessfully();
	    res["userInfo"] = JSON::Make(JSON::Class::Null);

	    if (status.authenticatedSuccessfully()) {
	        optional<UserInfo> userInfo = status.getUserInfo();
            JSON userJson;
            userJson["id"] = userInfo->getUserId();
            userJson["email"] = userInfo->getEmail();
            userJson["username"] = userInfo->getUserId();
            userJson["userType"] = (userInfo->getUserType() == UserType::CUSTOMER) ? "CUSTOMER" : "ADMIN";
            res["userInfo"] = userJson;
	    }

	    return res.dump();
	}

public:
	// Disable copy constructor and assignment operator
	ExpediaAuthenticationRestAPI(const ExpediaAuthenticationRestAPI &other) = delete;
    ExpediaAuthenticationRestAPI& operator=(const ExpediaAuthenticationRestAPI &other) = delete;

    static ExpediaAuthenticationRestAPI& getInstance() {
    	static ExpediaAuthenticationRestAPI userAuthRestAPI;
    	return userAuthRestAPI;
    }

	/**
	 *  expected json input:
	 * 		{"username": string, "password": string}
	 * 
	 * expected json output:
	 * 		{
	 * 			"msg": string,
	 * 			"success": boolean,
	 * 			"userInfo": {"id": string, "email": string, "username": string, "userType": enum} | null
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
	 * 			"userInfo": {"id": string, "email": string, "username": string, "userType": enum} | null
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

	// used only by the REST API classes and not to respond to external API requests
	pair<bool, json::JSON> authenticateRequest(const string &req) {
		json::JSON jsonReq = json::JSON::Load(req);
		json::JSON loginStatusJson = json::JSON::Load(login(jsonReq["userInfo"].dump()));
		return {loginStatusJson["success"].ToBool(), loginStatusJson};
	}

	ExpediaAuthenticationAPI& getUserAuthApi() {
		return expediaAuthenticationAPI;
	}
};


// TEMP: QUICK CHECKS FOR THE REST API
/*
	int main() {
	    ExpediaAuthenticationRestAPI &authApi = ExpediaAuthenticationRestAPI::getInstance();

	    // Test signup
	    cout << "Testing signup:" << endl;
	    string signupInput = R"({"email": "johndoe@example.com", "username": "johndoe", "password": "password123", "userType": "CUSTOMER"})";
	    cout << "Signup Response: " << authApi.signup(signupInput) << endl;

	    // Test login
	    cout << "\nTesting login:" << endl;
	    string loginInput = R"({"username": "johndoe", "password": "password123"})";
	    cout << "Login Response: " << authApi.login(loginInput) << endl;

	    // Test invalid login
	    cout << "\nTesting invalid login:" << endl;
	    string invalidLoginInput = R"({"username": "johndoe", "password": "wrongpassword"})";
	    cout << "Invalid Login Response: " << authApi.login(invalidLoginInput) << endl;

	    // Test invalid signup
	    cout << "\nTesting invalid signup:" << endl;
	    string invalidSignupInput = R"({"email": "abc", "username": "johndoe", "password": "abcabcabc", "userType": "CUSTOMER"})";
	    cout << "Invalid Signup Response: " << authApi.signup(invalidSignupInput) << endl;

	    return 0;
	}
*/

#endif