# Expedia-Console-Project

## Table of Contents
1. [Overview](#overview)
2. [Running the Project](#running-the-project)
3. [Rest API Support](#rest-api-support)
4. [Example Usage](#example-usage)
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

## Rest API Support
Probably the title may seem a bit misleading since there are no real network calls. However, the [rest-api-support](https://github.com/youssef-gerges-ramzy-mokhtar/Expedia-Console-Project/tree/rest-api-support) branch extends the main branch by simulating REST API functionality using JSON. The main changes include:
  1. __REST API Wrappers:__ Added wrappers over the ```expedia_expedia_api``` classes to simulate REST API functionality
  2. __Frontend Refactor:__ The ```expedia_frontend``` code is refactored into two layers for better separation of concerns:
      - View Layer: Handles user input and output, delegating backend communication to the Manager
      - Manager Layer: Abstracts the communication with the backend. Two implementations are provided:
          - REST API Manager: Communicates with the backend using the REST API classes
          - Local Library API Manager: Directly communicates with the backend using the ```expedia_core_api``` classes and backend objects

Design Notes:
  - The View now depends on an abstract manager interface, allowing flexibility in using either the REST or local library managers without needing to change the View code
  - To support this design, a large portion of the code in the Managers layer handles converting Data Transfer Objects (DTOs) to/from different formats:
      - JSON: used by the REST API Manager
      - Local Objects: Used by the Local Library API Manager to directly interact with backend objects
  - In real-world scenarios, such DTO conversions would be essential as the frontend and backend may use different languages and operate independently

Finally for working with JSON, we are using the [SimpleJSON Library](https://github.com/nbsdx/SimpleJSON)

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

## Final Remarks
As highlighted in the overview, this project primarily focuses on design rather than real functionality. It is highly recommended to review the initial high-level [UML class diagrams](https://github.com/youssef-gerges-ramzy-mokhtar/Expedia-Console-Project/tree/main/design) for the project before diving into the code.

__Some Design Notes:__
  - Several design patters were used throughout the project, and those are:
      - __Adapter/Wrapper Pattern:__ This pattern was used a lot in this project mainly to have a single interface for interacting with all the external APIs. For example, a single ```FlightAPI``` target interface was created for use by the Expedia code, with an adapter for each external flight API
      - __Visitor Pattern:__ Was used only to extend the functionality of the different Reservations available (e.g. Flights & Hotels) without modifying the Reservation classes. Although this pattern requires updates to the visitor interface whenever a new reservation type is added, the assumption was made that the number of reservation types would remain low for a project like Expedia
      - __Singleton Pattern:__ Used a few times across the project, mainly for classes responsible the API calls
  - Also, throughout the project I tried using Dependency Inversion as much as possible to have the project as extensible as possible. Examples are:
      - having a single interface for all Reservation Items
      - having a single interface for external APIs
      - having a single interface for the different Managers, to decouple the View from the differnet Manger implementations
