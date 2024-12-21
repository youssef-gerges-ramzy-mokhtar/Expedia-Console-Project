#include <vector>
#include <string>
#include <sstream>
using namespace std;

#ifndef EXPEDIA_RESERVATION_LOGIC_HPP_
#define EXPEDIA_RESERVATION_LOGIC_HPP_

#include "reservation_visitor.hpp"

class IReservationItem {
public:
	virtual double totalCost() = 0;
	virtual bool reserve() = 0;
	virtual bool cancelReservation() = 0;
	virtual IReservationItem* clone() const = 0;
	virtual string toString() = 0;
	virtual void accept(IReservationVisitor &visitor) = 0;
	virtual ~IReservationItem() {}
};

class Itinerary: public IReservationItem {
private:
	vector<IReservationItem*> reservations;

	// note we should handle a uncancelled reservation handler method in each type of Reservation Item, but for now we keep it simple just for illustration purposes
	// example flights might require trying to call the cancellation API multiple times for the cancellation to work, while the hotels might require logging the room that failed to be cancelled in a databases and internal employees in expedia calling each hotel separately to cancel the reservation. So each reservation item might have a different cancellation logic
	void stub_handle_uncancelled_reservation(IReservationItem* reservationItem) {
		// some logic to handle reservations that wasn't able to be cancelled
		return;
	}

	void deleteReservationItems() {
		for (auto &item: reservations)
			delete item;

		reservations.clear();
	}

public:
	Itinerary() {}
	
	// copy constructor
	Itinerary(const Itinerary &other) {
		for (const auto &reservation: other.getAllReservations())
			addReservationItem(reservation->clone());
	}

	virtual double totalCost() override {
		double cost = 0;
		for (auto &item: reservations)
			cost += item->totalCost();

		return cost;
	}

	// if any of the reservation items was not reserved then all reservation items that have been succesfully reserved will be cancelled
	virtual bool reserve() override {
		vector<IReservationItem*> reservedItems;
		for (auto &item: reservations) {
			bool reserved = item->reserve();
			if (!reserved)
				break;

			reservedItems.push_back(item);
		}

		// all items have been reserved successfully
		if (reservedItems.size() == reservations.size())
			return true;

		// cancel the reservation of the booked items
		for (auto &item: reservedItems)
			item->cancelReservation();

		return false;
	}

	virtual bool cancelReservation() override {
		bool allCancelled = true;
		for (auto &item: reservations) {
			bool itemCancelled = item->cancelReservation();
			if (!itemCancelled) {
				allCancelled = false;
				stub_handle_uncancelled_reservation(item);
			}
		}

		return allCancelled;
	}

	virtual IReservationItem* clone() const override {
		return new Itinerary(*this);
	}

	virtual string toString() override {
		ostringstream oss;
		oss << "************************************\n";
		oss << "Itinerary of " << reservations.size() << " sub-reservations\n";
		for (const auto &reservation: reservations)
			oss << reservation->toString() << "\n";

		oss << "Total Itinerary cost: " << totalCost() << "\n";
		oss << "************************************\n";
	
		return oss.str();
	}

	void addReservationItem(IReservationItem* reservationItem) {
		reservations.push_back(reservationItem);
	}

	const vector<IReservationItem*>& getAllReservations() const {
		return reservations;
	}

	virtual void accept(IReservationVisitor &visitor) override {
		visitor.visit(*this);
	}

	Itinerary& operator=(const Itinerary &other) {
		if (this != &other) {
			deleteReservationItems(); // we need to removed any old reservation items stored in this object
			for (const auto &reservation: other.getAllReservations())
				addReservationItem(reservation->clone());
		}

		return *this;
	}

	virtual ~Itinerary() override {
		// cout << "~Itinerary() deleting Reservation Items\n";
		deleteReservationItems();
	}
};

#endif