using namespace std;

#include "../expedia_core_api/expedia_hotel_api.hpp"
#include "../utils/json.hpp"

#ifndef EXPEDIA_REST_API_H_
#define EXPEDIA_REST_API_H_

class ExpediaHotelRestAPI {
private:
	ExpediaHotelAPI expediaHotelAPI;

private:
	HotelRequest convertJsonToHotelRequest(const string &hotelRequestJson) {
        json::JSON req = json::JSON::Load(hotelRequestJson)["hotelRequest"];
        return {
            req["fromDate"].ToString(),
            req["toDate"].ToString(),
            req["country"].ToString(),
            req["city"].ToString(),
            req["adults"].ToInt(),
            req["children"].ToInt(),
            req["neededRooms"].ToInt()
        };
    }

    json::JSON convertHotelRoomInfoToJson(const HotelRoomInfo &hotelRoomInfo) {
        json::JSON hotelRoomInfoJson;
        hotelRoomInfoJson["hotelName"] = hotelRoomInfo.getHotelName();
        hotelRoomInfoJson["fromDate"] = hotelRoomInfo.getFromDate();
        hotelRoomInfoJson["toDate"] = hotelRoomInfo.getToDate();
        hotelRoomInfoJson["country"] = hotelRoomInfo.getCountry();
        hotelRoomInfoJson["city"] = hotelRoomInfo.getCity();
        hotelRoomInfoJson["roomType"] = hotelRoomInfo.getRoomType();
        hotelRoomInfoJson["availableRooms"] = hotelRoomInfo.getAvailableRooms();
        hotelRoomInfoJson["pricePerNight"] = hotelRoomInfo.getPricePerNight();

        return hotelRoomInfoJson;
    }

public:
	/**
	 * expected json input:
	 * 		"hotelRequest": {fromDate, toDate, country, city, adults, children, neededRooms}
	 * 
	 * expected json output:
	 * 		"hotelRoomInfo": [
	 * 			{hotelName, fromDate, toDate, country, city, roomType, availableRooms, pricePerNight}
	 * 		]
	 */
	string search(const string &hotelRequestJson) {
		// convert the hotelRequestJson to HotelRequest object
		HotelRequest hotelRequest = convertJsonToHotelRequest(hotelRequestJson);
	
		// call the expedia hotel api (aka. core business logic)
        vector<HotelRoomInfo> searchResults = expediaHotelAPI.search(hotelRequest);

		// convert the returned result to json
		json::JSON res;
        res["hotelRoomInfo"] = json::Array();
        for (const auto &result: searchResults)
			res["hotelRoomInfo"].append(convertHotelRoomInfoToJson(result));

        return res.dump();
	}
};

#endif