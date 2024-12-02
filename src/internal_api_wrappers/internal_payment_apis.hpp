#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

#include "external_apis/external_payments_api.h"
#include "api_factory.hpp"

#ifndef INTERNAL_PAYMENT_APIS_HPP_
#define INTERNAL_PAYMENT_APIS_HPP_

class CardInfo {
private:
	string name;
	string address;
	string id;
	string expiryDate;
	int cvv;

public:
	CardInfo(
		const string &name, const string &address,
		const string &id, const string &expiryDate, int cvv)
		: name(name), address(address),
		id(id), expiryDate(expiryDate), cvv(cvv)
	{}

	// Getters
	string getName() const { return name; }
	string getAddress() const { return address; }
	string getId() const { return id; }
	string getExpiryDate() const { return expiryDate; }
	int getCvv() const { return cvv; }
};

class IPaymentAPI {
public:
	virtual bool makePayment(CardInfo cardInfo, double amountToWithdraw) = 0;
	virtual string getPaymentProviderName() = 0;
};

// Concrete Internal Payment API Wrappers
class PayPaylAPI: public IPaymentAPI {
private:
	PayPalOnlinePaymentAPI payPalApi;

public:
	virtual bool makePayment(CardInfo cardInfo, double amountToWithdraw) override {
		PayPalCreditCard card = {cardInfo.getName(), cardInfo.getAddress(), cardInfo.getId(), cardInfo.getExpiryDate(), cardInfo.getCvv()};
		payPalApi.SetCardInfo(&card);
		return payPalApi.MakePayment(amountToWithdraw);
	}

	virtual string getPaymentProviderName() override {
		return "paypal";
	}
};

class StripeAPI: public IPaymentAPI {
public:
	virtual bool makePayment(CardInfo cardInfo, double amountToWithdraw) override {
		StripeUserInfo stripeUserInfo = {cardInfo.getName(), cardInfo.getAddress()};
		StripeCardInfo stripeCardInfo = {cardInfo.getId(), cardInfo.getExpiryDate()};
		return StripePaymentAPI::WithDrawMoney(stripeUserInfo, stripeCardInfo, amountToWithdraw);
	}

	virtual string getPaymentProviderName() override {
		return "stripe";
	}
};

class SquareAPI: public IPaymentAPI {
public:
	virtual bool makePayment(CardInfo cardInfo, double amountToWithdraw) override {
		JSON obj;
		obj["card_info"] = json::Object();
		obj["card_info"]["CCV"] = cardInfo.getCvv();
		obj["card_info"]["DATE"] = cardInfo.getExpiryDate();
		obj["card_info"]["ID"] = cardInfo.getId();
		obj["money"] = amountToWithdraw;
		obj["user_info"] = Array(cardInfo.getName(), cardInfo.getAddress());
	
		ostringstream oss;
		oss << obj;
		string jsonQuery = oss.str();
	
		return SquarePaymentAPI::WithDrawMoney(jsonQuery);
	}

	virtual string getPaymentProviderName() override {
		return "square";
	}
};

// Factory
class PaymentAPIFactory: public APIFactory<IPaymentAPI> {
private:
	// this is the best which I have could done it is not the best way because we need to modify the init() method each time a new implementation of the IPaymentAPI interface is created. The issue is that C++ doesn't have mechanisms like Java Reflection which would have made this much more dynamic
	// another approach was to make this class a singelton and make every new IPaymentAPI implementation register itself with the factory but that would be fine if we only have 1 type of factory but if we have different type of factories then that will be a bit challenging
	void init() {
		IPaymentAPI* paypal = new PayPaylAPI();
		IPaymentAPI* stripe = new StripeAPI();
		IPaymentAPI* square = new SquareAPI();

		addAPI(paypal->getPaymentProviderName(), []() -> IPaymentAPI* {return new PayPaylAPI();});
		addAPI(stripe->getPaymentProviderName(), []() -> IPaymentAPI* {return new StripeAPI();});
		addAPI(square->getPaymentProviderName(), []() -> IPaymentAPI* {return new SquareAPI();});

		delete paypal;
		delete stripe;
		delete square;
	}

public:
	PaymentAPIFactory() {
		init();
	}
};

// This factory is used to return a single Payment API to make all payments
class ActivePaymentAPIFactory {
public:
	// there should be a more complex logic for getting the active payment for example reading the provider name from a config file and returning the API object based on that name
	IPaymentAPI* getActivePaymentAPI() {
		PaymentAPIFactory paymentAPIFactory;
		return paymentAPIFactory.createAPI("stripe");
	}
};

#endif
