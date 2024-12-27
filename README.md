# Expedia-Console-Project

## Table of Contents
1. [Overview](#overview)
2. [Running the Project](#running-the-project)
3. [Example Usage](#example-usage)
4. [Rest API Support](#rest-api-support)
5. [Final Remarks](#final-remarks)

## Overview
The Project is a simple C++ implementation of Expedia functionalities without providing real communication with other services or making API calls. The main focus is on designing software that models the functionalities provided by [Expedia.com](https://www.expedia.com/). There is no real searching functionality; the same data is returned from the Expedia backend regardless of the search query/request. There is no persistent storage of data; all data is stored in memory.
<br><br>

__Functional Requirements Provided:__
  - User login and sign up
  - Booking and paying for itineraries; itineraries can consist of different items, but for this project we only consider flights and hotels
  - Customer Users adding payment cards to pay for itineraries
  - Listing all itineraries associated with a Customer User
  - Supporting differnet user types (Admins & Customers), but in the project we only provide functionality for Customer Users
<br><br>

__The Idea of External APIs:__
  - Expedia relies on external APIs for flight, hotel, and payment operations. Currently supported APIs:
      - Flights: TurkishAirlinesOnlineAPI, AirCanadaOnlineAPI.
      - Hotels: HiltonHotelOnlineAPI, MarriottHotelOnlineAPI.
      - Payments: PayPalOnlinePaymentAPI, StripePaymentAPI, SquarePaymentAPI.
  - NOTE: All those APIs are stubs and do not provide real functionality
<br><br>

__Project Idea:__<br>
The project idea and requirements are based on Dr. Mostafa Saad's udemy [C++ course](https://www.udemy.com/course/cpp-4skills/?couponCode=ST12MT122624)

> [!NOTE]
> Implementation is directly placed inside header files for fast development. That is not a good practice especially when needing to change implementations without modifying the definations/interface of a header file. Also, it increase the compilation time as the preprecossor just copies the whole header file when being included, but that is not a big issue with this small project.

## Running the Project

## Example Usage

## Rest API Support
Probably the title is a bit catchy :)

## Final Remarks




Requirement C++ 17


// Side Remark if you are using windows sometimes you might got a compiler error that a header file doesn't exist that only happens if you clone the project in a path with a lot of characters (~256 characters), due to windows path length limits the C++ preprocessor fails to open the file on such a long path (If interested to know more about this topic you can have a look at the below links)
  - https://answers.launchpad.net/gcc-arm-embedded/+question/237311
  - https://gcc.gnu.org/legacy-ml/gcc-help/2013-05/msg00155.html
