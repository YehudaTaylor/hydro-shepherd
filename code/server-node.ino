/**
 * Server and weather station.
 * See report documentation for "Server Node"
 *
 * Requirements for this node:
 * Keep track of the weather.
 * Send alerts to staff when users have drunk too much or too little water.
 * Add a new user to the system via RFID tag.
 * Interact with database.
 * Respond to REST requests with JSON.
 * Dashboard for this node.
 */

/**
 *  libraries
 */

// NTP time
#include "NTP.h"
#include "WiFiUdp.h"

// OLED screen
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SPI.h>
#include <Wire.h>

// DHT sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// JSON documents
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

// RFID
#include <MFRC522.h>
#include <SPI.h>

/**
 * constants
 */

#define _SSID "SSIDNAME"     // Your WiFi SSID
#define _PASSWORD "password" // Your WiFi Password

// DHT
#define DHTPIN 3
#define DHTTYPE DHT11

// RFID
#define RST_PIN D3
#define SS_PIN D4

/**
 *  global variables
 */

String username = "";
String wifiIP = "";

// DHT sensor values
String temperature = "";
String humidity = "";

// PIR
#define PIN_PIR D8
int pirState = LOW;
int current_value = 0;

/**
 * Instances
 */

// NTP time
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);

// DHT sensor
DHT_Unified dht(DHTPIN, DHTTYPE);

// dynamic JSON documents
DynamicJsonDocument doc(1024);

// webserver
ESP8266WebServer server(80);

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

/**
 * RFID reader
 * @authors Initial Author: ryand1011 (https://github.com/ryand1011),
 * https://github.com/miguelbalboa
 * @cite
 * https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 */

void MFRC522setup() {
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
}

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
    username += (char)buffer2[i];
  }

  //----------------------------------------

  Serial.println(F("\n**End Reading**\n"));

  delay(10); // change value if you want to read cards faster

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

  display.println("The following user: ");
  display.println(username);

  display.display();
  display.clearDisplay();
}

void Adafruit_SH1106Gloop() {
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);

  display.println("The following user: ");
  display.println(username);
  display.println("WIFI IP address: ");
  display.println(wifiIP);
  display.println("Temperature: ");
  display.println(temperature);
  display.println("Humidity: ");
  display.println(humidity);

  display.display();
  display.clearDisplay();
}

/**
 * keep track of the weather
 * see requirement in report: (dynamically adjust drinking requirements based on
 * weather)
 * @copyright see "DHT license"
 */

uint32_t delayMS;

// setup DHT sensor
void DHT_Unifiedsetup() {
  // Initialize device.
  dht.begin();
  Serial.println(F("DHT Unified Sensor setup"));
  sensor_t sensor;
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

// loop for DHT sensor
void DHT_Unifiedloop() {
  // check if user is near server.
  // do not update weather values if yes
  if (pirState != HIGH) {
    // Delay between measurements.
    delay(delayMS);
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    } else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
      temperature = String(event.temperature);
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    } else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
      humidity = String(event.relative_humidity);
    }
  }
}

/**
 * scan for activity near server node
 * see requirement in report:
 * @cite
 * https://learning.oreilly.com/library/view/esp8266-home-automation/9781787282629/030a4305-064c-43f9-8d1b-1f77f03d94db.xhtml
 */

// setup motion sensor
void PIRsetup() { pinMode(PIN_PIR, INPUT); }

// scan for motion changes
void PIRloop() {
  current_value = digitalRead(PIN_PIR);
  if (current_value == HIGH) {
    if (pirState == LOW) {
      Serial.println(F("Sensor detected motion!"));
      Serial.println(F("Ignoring weather sensor readings!"));
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH) {
      Serial.println(F("Motion ended..."));
      Serial.println(F("Resuming weather sensor readings!"));
      pirState = LOW;
    }
  }
}

/**
 * REST requests with JSON response
 * @cite 5.105 Sample solution for Smart Door exercise
 */

// setup function for server
void Serversetup() {
  server.on("/", get_index);    // Get the index page on root route
  server.on("/json", get_json); // Get the json data on the '/json' route

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
  html += "<body> <h1>Server Node Dashboard</h1>";
  html += "<p> Welcome to the Server Node dashboard </p>";
  html += "<div>current user: " + username + "</div>";
  html += "<div>temperature is: " + temperature + "</div>";
  html += "<div>humidity is: " + humidity + "</div>";
  // html += "<div>user history is: " + getUserHistory() + "</div>";
  html += "</body> </html>";

  // Print a welcoming message on the index page
  server.send(200, "text/html", html);
}

// server "/json" route.
// Utility function to send JSON data
void get_json() {
  // Create JSON data
  jsonWeatherSensor();

  // Make JSON data ready for the http request
  String jsonStr;
  serializeJsonPretty(doc, jsonStr);

  // Send the JSON data
  server.send(200, "application/json", jsonStr);
}

// get user history in JSON format
String getUserHistory() {
  // Add JSON request data
  doc["Content-Type"] = "application/json";
  doc["Status"] = 200;

  // RFID user
  JsonObject user = doc.createNestedObject("User");
  user["username"] = username;
  user["health-conditions"] = "None";
  user["intake"] = 2000;

  // user history
  JsonObject history = doc.createNestedObject("History");
  JsonObject userData = history.createNestedObject("user-data");
  userData["user-id"] = username;
  userData["unix-time"] = ntp.formattedTime("%F %T");
  userData["amount"] = 2000;

  // Make JSON data ready for the http request
  String jsonStr;
  serializeJsonPretty(doc, jsonStr);

  return jsonStr;
}

// get data from weather station in JSON format
void jsonWeatherSensor() {
  // Add JSON request data
  doc["Content-Type"] = "application/json";
  doc["Status"] = 200;

  // DHT sensor values
  JsonObject temperatureAndHumidity = doc.createNestedObject("Temp-Humid");
  temperatureAndHumidity["sensorName"] = "DHT Sensor";
  temperatureAndHumidity["temperature"] = temperature;
  temperatureAndHumidity["humidity"] = humidity;

  // data for other departements
  // RFID user
  JsonObject user = doc.createNestedObject("User");
  user["username"] = username;
  user["health-conditions"] = "None";
  user["intake"] = 2000;

  // user history
  JsonObject history = doc.createNestedObject("History");
  JsonObject userData = history.createNestedObject("user-data");
  userData["user-id"] = username;
  userData["unix-time"] = ntp.formattedTime("%F %T");
  userData["amount"] = 2000;
}

/**
 * scan the database, in order to look for Users
 * who have not drunk enough or too much water for the day
 * and are at risk of dehydrating or overhydrating.
 * see requirement in report:
 */

// void scanDBForAlerts(){
// get list of users from database

// loop through users looking for volume consumed for the day

// check volume consumed for too much or little

// send alert to health staff

// }

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

/** setup functionality required in general by all components */
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
  Serversetup();
  MFRC522setup();
  Adafruit_SH1106Gsetup();
  DHT_Unifiedsetup();
  PIRsetup();
  NTPv2setup();
}

/**
 * main loop function that calls all component loop functions
 * @note order of functions matters
 */

void loop() {
  MFRC522loop();
  Adafruit_SH1106Gloop();
  DHT_Unifiedloop();
  PIRloop();
  Serverloop();
  NTPv2loop();
}