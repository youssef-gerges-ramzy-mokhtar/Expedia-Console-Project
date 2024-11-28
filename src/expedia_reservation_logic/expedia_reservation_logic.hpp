#include <vector>
#include <string>
#include <iostream>
using namespace std;

#ifndef EXPEDIA_RESERVATION_LOGIC_HPP_
#define EXPEDIA_RESERVATION_LOGIC_HPP_

class IReservationItem {
public:
	virtual double totalCost() = 0;
	virtual bool reserve() = 0;
	virtual bool cancelReservation() = 0;
	virtual ~IReservationItem() = 0;
};

class Itinerary: public IReservationItem {
private:
	vector<IReservationItem*> reservations;

public:
	virtual double totalCost() override {
		double cost = 0;
		for (auto &item: reservations)
			cost += item->totalCost();

		return cost;
	}

	// if any of the reservation items was not reserved then all reservation items that have been succesfully reserved will be cancelled
	virtual bool reserve() override {
		vector<IReservationItem*> successfulReservations;
		for (auto &item: reservations) {
			bool itemReserved = item->reserve();
			if (!itemReserved)
				break;

			successfulReservations.push_back(item);
		}

		// all items have been reserved successfully
		if (successfulReservations.size() == reservations.size())
			return true;

		// cancel the reservation of the booked items
		for (auto &item: successfulReservations)
			item->cancelReservation();

		return false;
	}

	virtual bool cancelReservation() override {
		bool allCancelled = true;
		for (auto &item: reservations) {
			bool itemCancelled = item->cancelReservation();
			if (!itemCancelled);
				allCancelled = false;
		}

		return allCancelled;
	}

	void addReservationItem(IReservationItem* reservationItem) {
		reservations.push_back(reservationItem);
	}

	virtual ~Itinerary() override {
		cout << "~Itinerary() deleting Reservation Items\n";
		for (auto &item: reservations) {
			delete item;
			item = nullptr;
		}

		reservations.clear();
	}
};

#endif