#include <bits/stdc++.h>
#include "user_mngr.hpp"
using namespace std;

int main() {
	Authentication authenticator;

	while (true) {
		cout << "Menu:\n";
		cout << "\t1: Login\n";
		cout << "\t2: Sign Up\n";

		cout << "\nEnter number in range 1 - 2: ";
		int choice;
		cin >> choice;

		if (choice == 1) {
			cout << "Enter username & password: ";
			string username, password;
			cin >> username >> password;
			AuthenticationStatus status = authenticator.login(username, password);
			if (status.authenticatedSuccessfully()) {
				cout << "Login Success";
			} else {
				cout << status.authenticationMsg() << "\n";
			}
		} else if (choice == 2) {
			cout << "Enter username, email, password: ";
			string username, email, password;
			cin >> username >> email >> password;
			AuthenticationStatus status = authenticator.signup(email, username, password, CUSTOMER);
			if (status.authenticatedSuccessfully())
				cout << "Account Crtead Success\n";
			else
				cout << status.authenticationMsg() << "\n";
		} else {
			cout << "Invalid Choice. Try Again\n";
		}

		cout << endl;
	}
	
	return 0;
}
