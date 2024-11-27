#include <string>
#include <sstream>
using namespace std;

#include "external_apis/external_payments_api.h"

class PaymentInfo {
private:
	string name;
	string address;
	string id;
	string expiryDate;
	int cvv;
	double amountToWithdraw;

public:
	PaymentInfo(
		const string &name, const string &address,
		const string &id, const string &expiryDate,
		int cvv, double amountToWithdraw)
		: name(name), address(address),
		id(id), expiryDate(expiryDate),
		cvv(cvv), amountToWithdraw(amountToWithdraw) {}

	// Getters
	string getName() const { return name; }
	string getAddress() const { return address; }
	string getId() const { return id; }
	string getExpiryDate() const { return expiryDate; }
	int getCvv() const { return cvv; }
	double getAmountToWithdraw() const { return amountToWithdraw; }
};

class IPaymentAPI {
public:
	virtual bool makePayment(PaymentInfo paymentInfo) = 0;
};

// Concrete Internal Payment API Wrappers
class PayPaylAPI: IPaymentAPI {
private:
	PayPalOnlinePaymentAPI payPalApi;

public:
	virtual bool makePayment(PaymentInfo paymentInfo) {
		PayPalCreditCard card = {paymentInfo.getName(), paymentInfo.getAddress(), paymentInfo.getId(), paymentInfo.getExpiryDate(), paymentInfo.getCvv()};
		payPalApi.SetCardInfo(&card);
		return payPalApi.MakePayment(paymentInfo.getAmountToWithdraw());
	}
};

class StripeAPI: IPaymentAPI {
public:
	virtual bool makePayment(PaymentInfo paymentInfo) {
		StripeUserInfo userInfo = {paymentInfo.getName(), paymentInfo.getAddress()};
		StripeCardInfo cardInfo = {paymentInfo.getId(), paymentInfo.getExpiryDate()};
		return StripePaymentAPI::WithDrawMoney(userInfo, cardInfo, paymentInfo.getAmountToWithdraw());
	}
};

class SquareAPI: IPaymentAPI {
public:
	virtual bool makePayment(PaymentInfo paymentInfo) {
		JSON obj;
		obj["card_info"] = Object();
		obj["card_info"]["CCV"] = paymentInfo.getCvv();
		obj["card_info"]["DATE"] = paymentInfo.getExpiryDate();
		obj["card_info"]["ID"] = paymentInfo.getId();
		obj["money"] = paymentInfo.getAmountToWithdraw();
		obj["user_info"] = Array(paymentInfo.getName(), paymentInfo.getAddress());
	
		ostringstream oss;
		oss << obj;
		string jsonQuery = oss.str();
	
		return SquarePaymentAPI::WithDrawMoney(jsonQuery);
	}
};