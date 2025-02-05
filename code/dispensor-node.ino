/**
 * Water dispensor node.
 * See report documentation for "Dispensor Node"
 *
 * Requirements for this node:
 * Accurately identify users (via RFID tags).
 * Accurately dispense a given amount of water .
 * Respond to REST requests with JSON.
 * Dashboard for this node.
 */

/**
 *  libraries
 */

// JSON documents
#include <ArduinoJson.h>

// Firebase database
#include <ESP8266Firebase.h>

// WiFi connection
#include <ESP8266WiFi.h>

// REST requests
#include <ESP8266HTTPClient.h>

// web server
#include <ESP8266WebServer.h>

// RFID scanner
#include <MFRC522.h>
#include <SPI.h>

// OLED screen
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>

// NTP time
#include "NTP.h"
#include "WiFiUdp.h"

// pump
#include "Relay.h"

/**
 * constants
 */

#define _SSID "SSIDNAME"     // Your WiFi SSID
#define _PASSWORD "password" // Your WiFi Password
// Firebase project reference url
#define REFERENCE_URL "https://ulr.firebaseio.com/"

// RFID
#define RST_PIN D3
#define SS_PIN D4

/**
 * global variables
 */

// keep track of the user
String username = "";

// water level of dispensor tank
int waterLevel = 0;

// scale factor to adjust the amount to dispense by
int weatherScale = 1;

// amount to dispense - (250 ml is one standard cup)
int dispenseAmount = 250;

// scale factor to be adjusted for the size of pump
int pumpRate = 50;

/**
 * Instances
 */

// NTP time
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);

// firebase database
Firebase firebase(REFERENCE_URL);

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// dynamic JSON documents
DynamicJsonDocument doc(1024);

// webserver
ESP8266WebServer server(80);

// relay - water pump
// pin 16 = D0
Relay pump(16, true);

/**
 * RFID reader
 * @authors Initial Author: ryand1011 (https://github.com/ryand1011),
 * https://github.com/miguelbalboa
 * @cite
 * https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 */

// setup RFID reader
void MFRC522setup() {
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
}

// loop for RFID reader
void MFRC522loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the
  // factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  // some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Reset the loop if no new card present on the sensor/reader. This saves the
  // entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(
      &(mfrc522.uid)); // dump some details about the card

  //-------------------------------------------

  Serial.print(F("Name: "));

  byte buffer1[18];

  block = 4;
  len = 18;

  //------------------------------------------- GET FIRST NAME
  status =
      mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key,
                               &(mfrc522.uid)); // line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++) {
    if (buffer1[i] != 32) {
      Serial.write(buffer1[i]);
      // add firstname to username
      username += (char)buffer1[i];
    }
  }
  Serial.print(" ");
  username += " ";

  //---------------------------------------- GET LAST NAME

  byte buffer2[18];
  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key,
                                    &(mfrc522.uid)); // line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i]);
    // add last name to username
    username += (char)buffer2[i];
  }

  //----------------------------------------

  Serial.println(F("\n**End Reading**\n"));

  delay(10); // change value if you want to read cards faster

  //---------------------------

  // if RFID tag scans succesfully - dispense water
  if (status == MFRC522::STATUS_OK) {
    Serial.println("Card scanned succesfully");

    // get the lastest weather scale from server node
    severNodeTempJSON();

    /**
     * get amount to dispense based on weather.
     * update the database records for the current user.
     * turn the pump on for the amount to dispense.
     */

    int amountToDispense = getAmountToDispense(username);
    turnPumpOn(amountToDispense);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

/**
 * OLED screen
 * @copyright see "OLED license"
 */
#define i2c_Address 0x3c

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO
Adafruit_SH1106G display =
    Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void Adafruit_SH1106Gsetup() {
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true);

  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(username);
  display.clearDisplay();
}

void Adafruit_SH1106Gloop() {
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Current user: ");
  display.println(username);
  display.display();
  display.clearDisplay();
}

/**
 * REST requests with JSON response
 * @cite 5.105 Sample solution for Smart Door exercise
 */

// setup function for server
void Serversetup() {
  server.on("/", get_index);    // Get the index page on root route
  server.on("/json", get_json); // Get the json data on the '/json' route
  server.on("/relayon",
            relay_on); // turn pump on via the relay on for a few seconds

  server.begin(); // Start the server
  Serial.println("Server listening");
}

// loop function for server
void Serverloop() {
  // Handling of incoming client requests
  server.handleClient();
}

// server "/" route
void get_index() {
  // HTML for basic dashboard
  String html = "<!DOCTYPE html> <html> ";
  html += "<head><meta http-equiv=\"refresh\" content=\"2\"><meta "
          "name=\"viewport\" content=\"width=device-width, "
          "initial-scale=1.0\"></head>";
  html += "<body> <h1>The Dispensor Node Dashboard</h1>";
  html += "<p> Welcome to the Dispensor Node dashboard </p>";
  html += "<div>current user: " + username + "</div>";
  html += "<div>current water level: " + String(waterLevel) + "</div>";
  html += "<div><a href='http://192.168.0.108/relayon'> Turn pump on for 5 "
          "seconds </a></div>";
  html += "</body> </html>";

  // Print a welcoming message on the index page
  server.send(200, "text/html", html);
}

// server "/json" route.
// Utility function to send JSON data
void get_json() {
  // Create JSON data
  jsonUserInfo();

  // Make JSON data ready for the http request
  String jsonStr;
  serializeJsonPretty(doc, jsonStr);

  // Send the JSON data
  server.send(200, "application/json", jsonStr);
}

// get the user info.
// create JSON document with info
void jsonUserInfo() {
  // Add JSON request data
  doc["Content-Type"] = "application/json";
  doc["Status"] = 200;

  // RFID user
  JsonObject user = doc.createNestedObject("User");
  user["username"] = username;
  user["health-conditions"] = "None";
  user["intake"] = 2.0;

  // user history
  JsonObject history = doc.createNestedObject("History");
  history["user-id"] = username;
  history["date"] = "date";
  history["amount"] = 2.0;
}

// server "/relay" route.
void relay_on() {
  // Add a header to respond with a new location for the browser to go to the
  // home page again
  server.sendHeader("Location", "/");
  server.send(303);
  Serial.println("HTTP switch relay on");
  // turn pump on for 5000 milliseconds
  turnPumpOn(5000);
}

/**
 * control the pump using a relay
 * @cite relay library example code
 */

// setup for relay
void Relaysetup() {
  pump.begin();
  pump.turnOff();
}

// turn pump on to dispense the given volume of water
void turnPumpOn(int volume) {
  Serial.println("TurnPumpOn start");
  pump.turnOn();
  // adjust pump rate according to size of pump
  delay(volume * pumpRate);
  pump.turnOff();
  Serial.println("TurnPumpOn finish");
}

/**
 * get the amount of water to dispense
 * for the current user
 */

int getAmountToDispense(String userId) {
  // query the database for the current user
  // and the amount dispense, and track
  int amount = queryDatabase(userId);

  return amount;
}

/**
 * connect to Firebase Realtime Database
 * For more details- https://github.com/Rupakpoddar/ESP8266Firebase
 */

// setup firebase database connection
void Firebasesetup() {
  firebase.json(true); // Make sure to add this line.
}

/**
 * query database
 * @param userId user id linked to RFID tag
 * @return amount of milliseconds to switch pump on for
 */

int queryDatabase(String userId) {
  // path in document database to current user
  String dbPath = "history/" + String(ntp.formattedTime("%F")) + "/" +
                  String(userId) + "/" + String(ntp.utc());

  // push amount consumed at given time for given user
  firebase.setInt(dbPath + "/amount", dispenseAmount);
  firebase.setString(dbPath + "/time", String(ntp.formattedTime("%F %T")));

  // update the total amount consumed for the day
  String dbAmountPath = "history/" + String(ntp.formattedTime("%F")) + "/" +
                        String(userId) + "/amount";
  int userAmount = firebase.getInt(dbAmountPath);
  firebase.setInt(dbAmountPath, userAmount + dispenseAmount);

  return dispenseAmount;
}

/**
 * water level sensor
 * used to determine if the water source is running low
 */

// check that there is water is the supply container
void WLevelloop() {
  int level = analogRead(A0);
  waterLevel = level;
  // Serial.print("water level: ");
  // Serial.println(level);

  // if water level is low send alert
  //  if(level<5){
  // send alert to staff
  //  lowWaterAlert();
  // }
}

/**
 * send request to server node for weather data.
 * @returns JSON response
 * @cite based on:
 * https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/
 */

void severNodeTempJSON() {
  WiFiClient client;
  HTTPClient http;

  // ip address for server node
  String serverNodeIP = "http://192.168.0.107";

  // REST GET url
  String serverNodeURL = serverNodeIP + "/json";

  // Send request
  http.useHTTP10(true);
  http.begin(client, serverNodeURL);
  http.GET();

  // turn payload in JSON document
  String payload = http.getString();
  deserializeJson(doc, payload);

  // get weather values
  int temperatureValue = doc["Temp-Humid"]["temperature"];
  int humidityValue = doc["Temp-Humid"]["humidity"];

  // scale the amount of water users should drink according to weather
  // conditions
  weatherScale = (temperatureValue / 100) + 1;
  dispenseAmount = 250 * weatherScale;
  Serial.println(dispenseAmount);

  // Disconnect
  http.end();
}

/**
 * get current time via NTP
 * see: https://github.com/sstaub/NTP
 */

// setup ntp time
void NTPv2setup() {
  ntp.begin();
  Serial.println("start NTP");
}

// loop for ntp time
void NTPv2loop() { ntp.update(); }

/**
 * setup functionality required in general by all components
 */

void Generalsetup() { Serial.begin(9600); }

/**
 * Set up wifi connection.
 * @note multiple components require wifi connection
 */

void WiFisetup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

/**
 * main setup function that calls all the component setup functions
 * @note order of functions matters
 */

void setup() {
  Generalsetup();
  WiFisetup();
  MFRC522setup();
  Adafruit_SH1106Gsetup();
  Serversetup();
  Relaysetup();
  Firebasesetup();
  NTPv2setup();
}

/**
 * main loop function that calls all component loop functions
 * @note order of functions matters
 */

void loop() {
  MFRC522loop();
  Adafruit_SH1106Gloop();
  Serverloop();
  WLevelloop();
  NTPv2loop();
}