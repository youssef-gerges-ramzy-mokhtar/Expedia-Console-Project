#ifndef RESERVATION_VISITOR_HPP_
#define RESERVATION_VISITOR_HPP_

// forward declaration to avoid cyclic dependencies when including header files
class FlightReservationItem;
class HotelReservationItem;
class Itinerary;

/**
 * The IReservationVisitor is mainly used to extend the functinality of the different ReservationItem classes without modifying those classes
 * this is especially useful when we want to extend the functionality that is not completely related to the the main pure logic of those classes
 * 		- e.g. doing a binary serialization, conversion to json, conversion to xml, etc...
 * 
 * This approach promotes the open-closed principle, and the single responsibility principle to not allow modifications to the ReservationItem classes and to not allow a lot of unrelated functionalities in those classes
 * The main need of the visitor pattern arisese from having the ability to add new functionality to different classes, and having the polymorohpic advantage of running the same operation on differnet types of classes
 * 
 * The only disadvantage of this pattern is that we will need to modify the IReservationVisitor to handle new types of reservation items
 * 		- but the assumption is that in an application like Expedia we would assume that we will have a limited number of items that you could have in your itinerary like (flights, hotels, cars, cruises, activities)
 * 		- we wouldn't assume that a user would have access to hundereds of different items he could add to his itinerary
 */
class IReservationVisitor {
public:
	virtual void visit(FlightReservationItem &flightReservation) = 0;
	virtual void visit(HotelReservationItem &hotelReservation) = 0;
	virtual void visit(Itinerary &itinerary) = 0;
	virtual ~IReservationVisitor() {}
};

#endif