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
<br><br>

> [!NOTE]
> Implementation is directly placed inside header files for fast development. That is not a good practice especially when needing to change implementations without modifying the definations/interface of a header file. Also, it increase the compilation time as the preprecossor just copies the whole header file when being included, but that is not a big issue with this small project.

## Running the Project
- You need a C++ 17 compiler (currently I am using the GNU compiler)
- Clone and run the ``expedia_main.cpp`` file, as following:
  ```bash
  g++ expedia_main.cpp
  Windows: a.exe    Linux: ./a.out
  ```
> [!CAUTION]
> On Windows, you might encounter a compiler error that a header file does not exist. This happens when the project is cloned into a directory with a very long path (~256 characters). Due to Windows path length limits, the C++ preprocessor may fail to open such files. If you're interested in learning more about this limitation, refer to the links below:
> - https://answers.launchpad.net/gcc-arm-embedded/+question/237311
> - https://gcc.gnu.org/legacy-ml/gcc-help/2013-05/msg00155.html

## Example Usage
- Start Menu, Signup & Viewing Profile <br>
  ![image](https://github.com/user-attachments/assets/ffad74a9-21b8-42fc-bbac-d0fa115ec104)
- Making an Itinerary, Searching & adding a flight to the itinerary <br>
  ![image](https://github.com/user-attachments/assets/f3bdd8d6-dad6-4b3a-98e0-ef9eee1ac3c2)
- Searching & adding a hotel to the itinerary <br>
  ![image](https://github.com/user-attachments/assets/ed39f0f4-306b-4589-9366-0b58c278275c)
- Booking the Itinerary <br>
  ![image](https://github.com/user-attachments/assets/bcde348c-d976-4f2c-8c1e-938030272f90)
- Listing the User Booked Itineraries <br>
  ![image](https://github.com/user-attachments/assets/353f6da2-39bc-41e2-bd5f-88b4429a5264)


## Rest API Support
Probably the title is a bit catchy :)

## Final Remarks
