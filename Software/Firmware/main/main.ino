/*
    Author  : Jan Buhlrich
              Tjorben Eberle @TJ_ger
    Date    : August, 2018
    Project : MQTT BME280
    Desc    :
    Version : 1.1

    Hardware list:  - Wemos D1 mini
                    - BME280
                    -

    circuit diagram:
                    - https://github.com/Nanunan/ESP8266_MQTT_temperature_and_humidity_sensor/blob/master/Media/Layout_DHT22.png

    Further project information:
                    - https://github.com/Nanunan/ESP8266_MQTT_temperature_and_humidity_sensor

    Common mistakes & tips:
                    - Flashing ESP8266 may not be possible when RST and D0 are connected.

                    #
                    
Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  D2 on ESP8266
SCK (Serial Clock)  ->  D1 on ESP8266
 */

#include <BME280I2C.h>
#include <Wire.h>

//MQTT
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//Upload over the Air
#include <ESP8266mDNS.h>

#include "pins.h"
#include "configuration.h"


#define SERIAL_BAUD 115200

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

//MQTT

WiFiClient wifiClient;
PubSubClient client(wifiClient);
char t_char[100];
char h_char[100];

//Mac
uint8_t MAC_array[6];
char MAC_char[18];

//////////////////////////////////////////////////////////////////


void setup()
{
  Serial.begin(SERIAL_BAUD);

  //Print mac adress first
  Serial.println();
  Serial.println("start_room_sensor");
  Serial.println("Mac adress:");
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i) {
    sprintf(MAC_char, "%s%02x:", MAC_char, MAC_array[i]);
  }
  Serial.println(MAC_char);

//Setup WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
//MQTT
  Serial.print("Attempting MQTT connection...");


        // init the MQTT connection
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);

  while(!Serial) {} // Wait



//Setup BME280
  Wire.begin(D2,D1);

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
}

//////////////////////////////////////////////////////////////////
void loop()
{

  
if (!client.connected()) 
  {
    reconnectwifi();
  }
  client.loop();
  delay(1000);
  client.loop();
 
  //  publishData(t,h);
     printBME280Data(&Serial);
   delay(500);


   

  Serial.println("INFO: Closing the MQTT connection");
  client.disconnect();

  Serial.println("INFO: Closing the Wifi connection");
  WiFi.disconnect();

 ESP.deepSleep(SLEEPING_TIME_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
  delay(500); // wait for deep sleep to happen
}

//////////////////////////////////////////////////////////////////
void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   pres= pres/100;
   client->println("Pa");

   publishData(temp,hum,pres);

   delay(1000);
}



void reconnectwifi() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("INFO: Attempting MQTT connection...");
    // Attempt to connect
      String clientName;
      clientName =mqtt_clientname;
      
    if (client.connect((char*) clientName.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("INFO: connected");
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishData(float p_temperature, float p_humidity, float p_pressure) {
  // create a JSON object
  // doc : https://github.com/bblanchon/ArduinoJson/wiki/API%20Reference
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  // INFO: the data must be converted into a string; a problem occurs when using floats...
  root["temperature"] = (String)p_temperature;
  root["humidity"] = (String)p_humidity;
  root["pressure"] = (String)p_pressure;
  root.prettyPrintTo(Serial);
  Serial.println("");
  /*
     {
        "temperature": "23.20" ,
        "humidity": "43.70"
     }
  */
  char data[200];
  root.printTo(data, root.measureLength() + 1);
  client.publish(MQTT_SENSOR_TOPIC, data, true);
  yield();
}

// function called when a MQTT message arrived
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
}
