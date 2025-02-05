# Hydro-Shepard

# An IoT based hydration system

A hydration tracking and dispensing system designed for assisted living facilities

Author details: Yehuda Taylor

## Abstract

The goal for this project is to develop an IoT solution for assisted living. People living in assisted living facilities face a variety of challenges. Many of these have been effectively addressed, with commercial solutions readily available. However some problems do not have optimal solutions. One such challenge is fluid intake. 

There are two sides to fluid intake. Dehydration and overhydration. Dehydration happens when a person's body does not have as much fluid as it requires[\[1\], \[2\]](https://www.zotero.org/google-docs/?SKKfWm). Dehydration occurs at all ages, however in assisted living facilities, individuals or healthcare workers may not be able to easily prevent dehydration[\[3\]](https://www.zotero.org/google-docs/?7okHqj). Overhydration [\[4\]](https://www.zotero.org/google-docs/?yFB5QH), also called water toxicity[\[5\]](https://www.zotero.org/google-docs/?Hmu7OK), occurs when an individual consumes too much liquid. This condition can lead to seizures, coma and can even be fatal if left untreated[\[5\]](https://www.zotero.org/google-docs/?GSd9og). This is also an issue in assisted living facilities where individuals may suffer from memory related illnesses, such as dementia, and may not be aware of how much liquid they are consuming. 

There are several solutions for preventing dehydration, but ultimately fall short when considering the users adoption and use of technology, and environments that the devices operate in. They require the user to purchase and use an expensive specific container, cannot accurately track how much liquid was consumed, do not dynamically adapt to incorporate additional factors, have privacy concerns, and do not prevent overhydration.

Therefore the following proposal for an IoT based tracking and water dispensing system, is suitable for addressing these issues. The solution works because it can dispense water into any container, can accurately measure the amount consumed, dynamically adapts to other factors, such as weather and health issues, is private, and can prevent overhydration. 

To date, the following progress has been made. Research into the assisted living challenges, and more specifically into dehydration and existing solutions. Based on the research, a solution was designed. Subsequently, the required hardware has been purchased, and development on the project has begun. 

## Introduction

Assisted living comes with a variety of challenges. Dehydration is one such problem. It is common[\[3\]](https://www.zotero.org/google-docs/?C54qOS), and has severe consequences[\[6\], \[7\]](https://www.zotero.org/google-docs/?5NIKY7). 

The aims and objectives for this project are as follows. Research existing solutions to the problem.  
Find a gap in the problem for a new solution. Develop an IoT based solution to help solve hydration issues in assisted living facilities. The solution will be built with ESP8266 microcontrollers, and assorted sensors. It will help prevent dehydration and overhydration. In doing so it improves quality of life, making it easy for everyone to use. It will be economical, easy to scale, and effective.

The system will, accurately and non invasively, track individual users (via RFID).  
It will dispense water for them using a pump at a water station (they do not need to measure the amount). It will dynamically change the amount dispensed based on the weather (temperature and humidity sensor). It will communicate with healthcare workers to update the amount of water dispensed based on health changes (not dependent on various departments communicating with each other).

## Proposal

The following system will help prevent dehydration, more effectively than existing solutions. It has better usability, is more cost effective, dynamic, scalable, accurate and privacy aware. Users do not need to carry around a specific drinking container. RFID tags are cheap, so it is easy to scale the system. It dynamically updates drinking requirements based on the weather and health factors. It accurately dispenses the right amount of water. And preserves privacy by not using audio or camera sensors.

The system can be broken down into two main components.

1. Water dispensing stations.  
2. Server for brains and Weather station.

The water dispensing station will accurately dispense a specific amount of water, into any container, based on the user. These stations can be placed anywhere around the facility, much like other water dispensers. 

The server and weather station. This component keeps track of the weather, to dynamically adjust drinking requirements. It also allows health workers to connect to the system, where they can update a given user's health info and drinking requirements. The server also sends out alerts when users have not drunk enough.

To identify users, each user will be assigned a small RFID tag (it could be placed in a piece of jewellery, in a phone or smart watch). When they want water they go to one of the dispensers located around the facility, and scan their tag. The system picks up their profile, calculates how much water they need to drink, and dispenses the required amount.

## Implementation Details

## System development life cycle 

As per the Agile methodology[\[8\]](https://www.zotero.org/google-docs/?JNhxXh), there are several stages to implementing a project. Starting with Requirements Analysis.

### Requirements analysis 

While some requirements were determined based on the outcomes of the literature review, and midterm work, others were derived through different methods. There are several methods of conducting Requirements Elicitation. One such method is through the use of Use Case Diagrams [\[9\]](https://www.zotero.org/google-docs/?JgGLiB).

**Use case diagrams**:  
Users will interact with the system in the following ways.  
A user will want to place some water in their container.  
An admin will add users to the system.  
Health (or other department), will feed data into the system.

![][image1]  
Figure: use case diagram

Based on the above diagram we can deduce some of the following requirements.

**Functional and non-functional requirements**  
The system should meet the following requirements.  
Those that are deduced from the use case diagrams above, as well as from additional places, such as the project brief requirements

|  | Functional requirements. |
| ----- | :---- |
| 1 | The system should keep track of and record the environment temperature |
| 2 | The system should dispense water into a container |
| 3 | The system should measure amounts of water dispensed |
| 4 | The system should accurately identify an individual using a RFID tag |
| 5 | The system should keep information and profiles of users in a database. |
| 6 | The nodes should communicate with each other via a networking protocol |
| 7 | All microcontrollers nodes should have a dashboard and respond to REST-based HTTP requests in a JSON format |
|  | Non functional requirements. |
| 1 | The system should be easy to use in an assisted living facility |
| 2 | The system should be cost effective to build |
| 3 | The system should be easy to scale |
| 4 | The system should communicate with different departments |
| 5 | The system should be secure |

Table: functional and nonfunctional requirements 

### Design 

The system was designed from a requirements down methodology. Architecture is derived from requirements, hardware from architecture, circuits from hardware, and so on.

**System architecture diagram**:  
The following architecture diagram illustrates how the system will function as a whole.  
The water dispenser node can be seen on the left, and the server and weather station node, on the right. The figures in the diagram illustrate the users, admin and healthcare workers. Lines and arrows indicate dataflow and input actions.

![][image2]  
Figure: system architecture diagram

In order to implement this architecture, hardware components are required. Most were chosen based on the scope of the project, which included using the hardware from the kit purchased for the course.

**Hardware components**:

* NodeMCU ESP8266 microcontroller, as required in the project brief  
* Rfid sensor: a) scan and identify users. b) program new tags  
* Pump: dispense water  
* Water level sensor: accurately track amount dispensed  
* OLED screen: general usability \- show relevant information  
* Temp and humidity sensor: track the temperature   
* Motion detection sensor: stop reading values from temperature sensor, as the person's body heat may influence the readings

**Software components**:

* Libraries: as required by the components, WiFi, other networking libraries.  
* IDE: Arduino IDE  
* Programming languages  
  * C++ : general purpose development  
  * HTML/CSS: dashboards

**Communication protocols**:

* HTTP (REST): as required by coursework  
* WiFi: connect nodes together  
* RFID: cheap and scalable identification tags

**Additional tools**:

* Breadboard, power supply, other kit parts.

**Hardware circuit diagrams**:  
The hardware needs to be assembled in the following way. (Further details are included later on in the  report, as well as in a PDF file which can zoom in better to see the pin connections clearly).  
![][image3]  
Figure: Circuit diagram for the project design

**UI diagrams**:  
As per the project requirements for a basic dashboard. This is a basic overview of what the dashboard interface requires, in order to interact with the nodes.

Figure: basic wireframes

As per the requirements, each node should transmit data either directly or indirectly to the rest of the system. A model for the following data looks as follows.

**Data flow between nodes diagrams**:  
Data will flow between nodes in the following manner. Nodes can be accessed via REST.

Figure: Data flow between nodes

### Implementation/Development 

As per the design of the project there are two main hardware components to implement.

1. The Server/Weather Station Node  
2. Water Dispenser Station Node

Both of these components have sub components themselves, and will be detailed below within each component.

#### Server-Node code and hardware

The requirements for this node are as follows.

* Keep track of the weather, by recording the temperature and humidity   
* Add new users to the system  
* Have a basic dashboard  
* Respond to HTTP based requests with JSON 

As mentioned above these requirements dictated the choices for the design.

The hardware is implemented as follows, as per the circuit diagram design. 

![][image4]  
Figure: Server Node assembled 

The node has a basic dashboard that can be accessed via the Node’s IP address.   
The dashboard displays the weather data, as recorded by the nodes sensor.  
![][image5]  
Figure: Server Node Dashboard 

Some of the code for implementing the JSON response functionality can be seen below.  
![][image6]  
Figure: Server-node code for JSON response

The JSON response can be seen in the following Figure.  
![][image7]  
Figure: Server Node HTTP \` JSON response 

#### 

#### Dispenser-Node code and hardware

The requirements for this node are as follows.

* Identify users via RFID tags  
* Dispense a given amount of water to each user  
* Keep track of how much water each user has consumed  
* Basic dashboard   
* Respond to HTTP based requests in JSON format.

As mentioned above these requirements dictated the choices for the design.

The hardware is implemented as follows, as per the circuit diagram design.   
![][image8]  
Figure: Dispenser Node assembled 

The node has a basic dashboard that can be accessed via the Node’s IP address.   
The dashboard displays the current water level, current user, and an option to turn the pump on for 5 seconds.  
![][image9]  
Figure: Dispenser Node dashboard

Some of the code for implementing the JSON response functionality can be seen below.

![][image10]  
Figure: Dispensor-node JSON response code

The JSON response can be seen in the following Figure.  
![][image11]  
Figure: Dispenser Node JSON response 

In addition to the hardware and software components, there is also the database component.   
There are two main types of databases, relational and non relational [\[10\]](https://www.zotero.org/google-docs/?aP8EUa). The choice to use a NoSQL database for this project is due to the nature of having data available fast, in order to update the database with the amount of water consumed and dispense it then and there. Firebase has a solution for this, with their Realtime Database[\[11\]](https://www.zotero.org/google-docs/?2BF9lR).  
![][image12]  
Figure: Firebase database of water consumption history.

## 

### Testing

As identified by the system design, there are several core aspects of the system that are essential to its functionality. As such there needs to be testing, in order to make sure the system works. This means testing that all functional and non-functional requirements are satisfied. 

#### Performance Testing  

In order to test how the system might behave with multiple users, without having multiple stations set up it was necessary to simulate multiple dispenser nodes communicating with the server node and Firebase Realtime Database[\[11\]](https://www.zotero.org/google-docs/?mYATSJ). The current free tier supports 100 simultaneous connections, while the paid tier supports up to 200k per database[\[12\]](https://www.zotero.org/google-docs/?zDPYnZ), making it suitable for a production system.

#### Functional Testing  

Testing plan set out in Project Proposal, based on the system requirements.  
The following table outlines the test strategies, input data/action, expected results, vs actual results, and evaluation method for each test.   
There is also a column for the location of the documented test results. 

| requirement number | Input data/action | evaluation methodology | expected results | test outcome documentation | start condition | finish condition |
| :---- | :---- | :---- | :---- | :---- | :---- | :---- |
| 1 | place heat source in temperature and humidity sensor | compare sensor readings. before vs after | sensor reading values increase according to heat source | video demonstration | 29 | 30 |
| 2 | place container by system, send command to dispense water | visually check if water appears in container | water is dispensed into container | video demonstration | empty container | water dispensed into container |
| 3 | place container by system, send command to dispense water | measure dispensed amount, check if within range of requested amount | specified amount of water dispensed into container | video demonstration | empty container | 250 ml dispensed into container |
| 4 | place tag by system RFID reader | compare actual to expected value | system reads the expected value from RFID tag | video demonstration | tag user:0001 | system displays user:0001 |
| 5 | query database for user information | check if the correct value is returned | respective user information is retrieved | video demonstration | query user:0001 for daily amount drunk | respective amount for user:0001 returned |
| 6 | send communication message between nodes via chosen protocol | check if message is received | receiving node receives message | report | send http GET request to server node | responds with JSON weather data |
| 7 | send HTTP requests to all nodes | check for responses | all nodes respond to request. actions visible on the dashboard | report | send http GET /json request to both nodes | returns JSON response |

Table linking requirement to test, along with strategy for test and evaluating result.

#### Security Testing  

Database is not secure, as anyone can update the database with the current implementation.  
Based on this a new feature is required: add user authentication so that only authenticated nodes can modify the database.

#### Unit Testing

Given more time I would have incorporated a unit testing framework called “arduinounit”[\[13\]](https://www.zotero.org/google-docs/?Yo0g6O). It is specifically designed to work with the Arduino libraries, making it suitable for this project.

#### Usability Testing  

Comprehensive plan required for further development 

#### Acceptance Testing 

Given the scope of this project, this was beyond the timeframe, however it is essential for further development of the project

### Deployment  

The system is yet to be deployed in an assisted living facility.

### Maintenance 

Routinely check on the hardware to check for failures. Check the database to ensure no anomalies are occuring.  
Get user feedback and develop further requirements/modifications based on the analysis of the feedback.  
Include error logging in the code, through Crashlytics[\[14\]](https://www.zotero.org/google-docs/?FaWGcf) or Sentry [\[15\]](https://www.zotero.org/google-docs/?3Y6Yon).

### Planning 

The plan was to work in sprints, with work broken down and assigned to a given sprint.  
![][image13]  
Figure: Gantt Chart of Sprints.   
![][image14]  
Figure: Agile Tasks board

## Evaluation Results 

The project successfully implements an Internet of Things solution, for a real world problem in Assisted Living Facilities. However through the development of the project several design flaws in the architecture showed up. They are detailed in the following critical reflection section.

The system tracks individuals via RFID tags, keeps their information in a database, and updates the database to track their liquid consumption. It also scales well, as RFID tags are cheap, and the database is capable of handling a high number of simultaneous connections. 

## Critical Analysis and Reflection

During the development of this system some of the following issues and insights occurred.

Firstly, there is the assumption that an IoT based solution is the best solution to the problem.  
There are no additional factors that could affect the implementation (laws, regulations, etc). 

Hardware: 

* The circuit boards could have been arranged neater

Code: 

* There is a fair amount of duplication between the nodes. This could be refactored for better modularity and maintainability.  
* Checks for errors could be more comprehensive (e.g. check for nulls)

Design:

* User feedback for user interaction   
  * If I had more time would have added a buzzer or LED to indicate that the user action was successful 

Architecture.

* Sending requests to the other nodes takes time, which slows down the rest of the flow

Implementation. 

* The hardware is a bit messy and could be cleaned up given more time.

Use cases. 

* The system only tracks water dispensing, this is a large part of liquid consumption, however hot beverages such as Tea and Coffee also contribute and are not tracked by this solution. 

Value.

* The system has value in preventing overhydration and dehydration, however it may not be as effective as initially assumed during the research phase of the project. 

Reach objectives.

* Due to time constraints I did not manage to implement the   
  * sending out alerts functionality  
  * Add a new user tag to the system   
    * Requirements  
      * code   
        * Add html code for dashboard with input form   
        * Add route on server for the dashboard   
        * And code to write to RFID tag 

Assumptions:

* The user will drink all the water dispensed to them.

Practical issues.

* Response time between scanning RFID tag and dispensing water is slow, due to the request sent to other node and database

Testing:

* Unit tests could have been added.

## Conclusion

To summarise, this project helps individuals living in assisted care facilities, by tracking their hydration needs. It was developed using the agile software methodology, and based on research conducted in a literature review.

While this is only a prototype there is definitely real world application, and the scope of work can easily be expanded to include additional features.

As detailed in the critical reflection section, there are several challenges that still need to be solved with the implementation itself, and the solution as a whole needs further features before deploying it into an assisted living facility. 

To conclude, this course has really opened up a whole new world. I'm really happy that I took it, and got to implement a real world solution in this project. 

## References

[\[1\]	“Dehydration,” nhs.uk. Accessed: Jan. 06, 2024\. \[Online\]. Available: https://www.nhs.uk/conditions/dehydration/](https://www.zotero.org/google-docs/?rkmK0e)  
[\[2\]	W. E. Contributor, “What is Dehydration? What Causes It?,” WebMD. Accessed: Jan. 06, 2024\. \[Online\]. Available: https://www.webmd.com/a-to-z-guides/dehydration-adults](https://www.zotero.org/google-docs/?rkmK0e)  
[\[3\]	M. Nagae *et al.*, “Chronic Dehydration in Nursing Home Residents,” *Nutrients*, vol. 12, no. 11, p. 3562, Nov. 2020, doi: 10.3390/nu12113562.](https://www.zotero.org/google-docs/?rkmK0e)  
[\[4\]	W. E. Contributors, “What Is Too Much Water Intake?,” WebMD. Accessed: Jan. 06, 2024\. \[Online\]. Available: https://www.webmd.com/diet/what-is-too-much-water-intake](https://www.zotero.org/google-docs/?rkmK0e)  
[\[5\]	B. V. Peechakara and M. Gupta, “Water Toxicity,” in *StatPearls*, Treasure Island (FL): StatPearls Publishing, 2023\. Accessed: Jan. 06, 2024\. \[Online\]. Available: http://www.ncbi.nlm.nih.gov/books/NBK537231/](https://www.zotero.org/google-docs/?rkmK0e)  
[\[6\]	R. Cohen, G. Fernie, and A. Roshan Fekr, “Fluid Intake Monitoring Systems for the Elderly: A Review of the Literature,” *Nutrients*, vol. 13, no. 6, p. 2092, Jun. 2021, doi: 10.3390/nu13062092.](https://www.zotero.org/google-docs/?rkmK0e)  
[\[7\]	A. M. El-Sharkawy, O. Sahota, R. J. Maughan, and D. N. Lobo, “The pathophysiology of fluid and electrolyte balance in the older adult surgical patient,” *Clin. Nutr.*, vol. 33, no. 1, pp. 6–13, Feb. 2014, doi: 10.1016/j.clnu.2013.11.010.](https://www.zotero.org/google-docs/?rkmK0e)  
[\[8\]	“Agile SDLC | Software Development Life Cycle \- Javatpoint.” Accessed: Feb. 15, 2024\. \[Online\]. Available: https://www.javatpoint.com/agile-sdlc](https://www.zotero.org/google-docs/?rkmK0e)  
[\[9\]	“UseCaseRequirements.pdf.” Accessed: Mar. 10, 2024\. \[Online\]. Available: https://sites.cs.ucsb.edu/\~mikec/cs48/project/UseCaseRequirements.pdf](https://www.zotero.org/google-docs/?rkmK0e)  
[\[10\]	B. Anderson, “SQL vs. NoSQL Databases: What’s the Difference?,” IBM Blog. Accessed: Mar. 10, 2024\. \[Online\]. Available: https://www.ibm.com/blog/sql-vs-nosql/www.ibm.com/blog/sql-vs-nosql](https://www.zotero.org/google-docs/?rkmK0e)  
[\[11\]	“Firebase Realtime Database | Store and sync data in real time.” Accessed: Mar. 01, 2024\. \[Online\]. Available: https://firebase.google.com/products/realtime-database](https://www.zotero.org/google-docs/?rkmK0e)  
[\[12\]	“Firebase Pricing.” Accessed: Mar. 06, 2024\. \[Online\]. Available: https://firebase.google.com/pricing](https://www.zotero.org/google-docs/?rkmK0e)  
[\[13\]	M. Murdoch, “mmurdoch/arduinounit.” Feb. 16, 2024\. Accessed: Mar. 06, 2024\. \[Online\]. Available: https://github.com/mmurdoch/arduinounit](https://www.zotero.org/google-docs/?rkmK0e)  
[\[14\]	“Crashlytics App Crash & Stability Reporting | Firebase.” Accessed: Mar. 10, 2024\. \[Online\]. Available: https://firebase.google.com/products/crashlytics](https://www.zotero.org/google-docs/?rkmK0e)  
[\[15\]	“Application Performance Monitoring & Error Tracking Software,” Sentry. Accessed: Mar. 10, 2024\. \[Online\]. Available: https://sentry.io/welcome/](https://www.zotero.org/google-docs/?rkmK0e)  
[\[16\]	“Welcome to Fritzing.” Accessed: Mar. 10, 2024\. \[Online\]. Available: https://fritzing.org/](https://www.zotero.org/google-docs/?rkmK0e)

## Appendices

### Build Instructions  

1. Assemble hardware according to the following circuit diagrams. (The required hardware parts can be seen in the components list below).  
2. Install ArduinoIDE if not already installed.   
3. Connect nodes to the computer via the USB cable.  
4. Create a new file in the IDE, and copy the source code for the respective hardware node into the IDE.   
5. Install the required libraries   
6. There are some values to input for each installation. They are as follows.  
   1. WiFi SSID  
   2. WiFi password  
   3. Firebase database reference URL (instructions to set up the Firebase projects, and generate the URL, can be found here: https://github.com/Rupakpoddar/ESP8266Firebase).  
7. Upload the code to the node.  
8. Repeat for the remaining nodes.

### Circuit diagrams 

The following circuits were built using fritzing[\[16\]](https://www.zotero.org/google-docs/?MAdvFu).   
![][image15]  
Figure: Circuit diagrams for all nodes.  

Water dispenser circuit. Pump, power supply, water level sensor, OLED screen, RFID sensor.  
Server and weather sensor circuit. Temp/Humidity sensor, RFID sensor, motion sensor.

### Components list 

Description of the components.

* USB cable \* 2  
* ESP8266 \* 2  
* OLED screen \* 2  
* RFID scanner \* 2  
  * At least one RFID tag  
* DHT11 sensor   
* PIR motion sensor   
* Relay  
* Water pump  
* Water level sensor   
* Breadboard \* 2  
* Jumper cables \- assorted

## 

#### **Assembly List** 

| Label | Part Type | Properties |
| :---- | :---- | :---- |
| HC-SR1 | HC-SR501 Body Sensor Module | manufacturer D-SUN; variant variant 1 |
| Part1 | KY-015 Temperature & Humidity Sensor Module | operating voltage 3.3V to 5.5V; chip label KY-015; humidity measurement range 20% to 90% RH; pins 3; humidity measurement resolution 1% RH; editable pin labels false; temperature measurement resolution 1ºC; signal transmission range 20m; temperature measurement accuracy ±2ºC; temperature measurement range 0ºC to 50ºC \[32ºF to 122ºF\]; variant variant 2; package Breakout board; dimensions 18.5mm x 16.5mm \[0.728in x 0.650in\]; pin spacing 300mil; humidity measuring accuracy ±5% RH |
| Part3 | OLED Monochrome 128x64 1.3 inch | variant Mono 128x64 1.3 inch; part \# 938 |
| Part6 | OLED Monochrome 128x64 1.3 inch | variant Mono 128x64 1.3 inch; part \# 938 |
| Part7 | KY-019 5V Relay Module | maximum dc 10A 30VDC; editable pin labels false; operating voltage 5V \~ 12V; pins 3; dimensions 27mm x 34mm \[1.063in x 1.338in\]; chip label KY-019; pin spacing 300mil; variant variant 1; maximum ac 10A 250VAC; package Breakout board |
| RFID2 | RFID RC522 Module | vcc 3v3; editable pin labels false; pins 8; chip label MFRC522; pin spacing 300mil; package THT |
| RFID3 | RFID RC522 Module | vcc 3v3; editable pin labels false; pins 8; chip label MFRC522; pin spacing 300mil; package THT |
| U1 | NodeMCU V1.0 | chip ESP8266; variant NodeMCUV1 |
| U3 | NodeMCU V1.0 | chip ESP8266; variant NodeMCUV1 |
| Water Level Sensor2 | Water Level Sensor | pins 3; variant variant 1; package board; spacing 300mil |
| WATER PUMP2 | water pump/motor | variant variant 3 |

#### Shopping List

| Amount | Part Type | Properties |
| :---- | :---- | :---- |
| 1 | HC-SR501 Body Sensor Module | manufacturer D-SUN; variant variant 1 |
| 1 | KY-015 Temperature & Humidity Sensor Module | operating voltage 3.3V to 5.5V; chip label KY-015; humidity measurement range 20% to 90% RH; pins 3; humidity measurement resolution 1% RH; editable pin labels false; temperature measurement resolution 1ºC; signal transmission range 20m; temperature measurement accuracy ±2ºC; temperature measurement range 0ºC to 50ºC \[32ºF to 122ºF\]; variant variant 2; package Breakout board; dimensions 18.5mm x 16.5mm \[0.728in x 0.650in\]; pin spacing 300mil; humidity measuring accuracy ±5% RH |
| 2 | OLED Monochrome 128x64 1.3 inch | variant Mono 128x64 1.3 inch; part \# 938 |
| 1 | KY-019 5V Relay Module | maximum dc 10A 30VDC; editable pin labels false; operating voltage 5V \~ 12V; pins 3; dimensions 27mm x 34mm \[1.063in x 1.338in\]; chip label KY-019; pin spacing 300mil; variant variant 1; maximum ac 10A 250VAC; package Breakout board |
| 2 | RFID RC522 Module | vcc 3v3; editable pin labels false; pins 8; chip label MFRC522; pin spacing 300mil; package THT |
| 2 | NodeMCU V1.0 | chip ESP8266; variant NodeMCUV1 |
| 1 | Water Level Sensor | pins 3; variant variant 1; package board; spacing 300mil |
| 1 | water pump/motor | variant variant 3 |

Exported with Fritzing 0.9.6- http://fritzing.org

### Source code

Some of the source code that has been compiled and deployed to microcontrollers.   
There are over 1100 lines of code for the project, so I did not include screenshots of all the code. The rest of the code is included in the accompanying zip file.

![][image16]  
Figure: Dispsensor Node some code

Server Node code:

![][image17]  
Figure: server node code  
![][image18]  
Figure: Dispenser  \- Node code

![][image19]  
Figure: Dispsensor Node code
