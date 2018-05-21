/*
    Author  : Jan Buhlrich
              Tjorben Eberle @TJ_ger
    Date    : Mai, 2018
    Project : Room Sensor
    Desc    :
    Version : 1.0

    Hardware list:  - Wemos D1 mini
                    - DHT22
                    - BME280

    circuit diagram:
    https://github.com/Nanunan/ESP8266_MQTT_temperature_and_humidity_sensor/blob/master/Media/Layout_DHT22.png

    Further project information:
    https://github.com/Nanunan/ESP8266_MQTT_temperature_and_humidity_sensor

    Common mistakes & tips:

*/

#ifndef UNIT_TEST
#include <Arduino.h>
#endif

//MQTT
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//Upload over the Air
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

/*
  Local Headerfiles
*/
#include "pins.h"
#include "configuration.h"

//DHT
#include "DHT.h"
DHT dht(DHTPIN, DHTTYPE);


//MQTT
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);
char t_char[100];
char h_char[100];

//Mac
uint8_t MAC_array[6];
char MAC_char[18];

//OTA
bool flashtime = 0;



/*
  Setup
*/

void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);

  //Print mac adress first
  Serial.println();
  Serial.println("start_room_sensor");
  Serial.println("Mac adress:");
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i) {
    sprintf(MAC_char, "%s%02x:", MAC_char, MAC_array[i]);
  }
  Serial.println(MAC_char); //- See more at: http://www.esp8266.com/viewtopic.php?f=29&t=3587#sthash.hV7FUT1J.dpuf

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

//OTA
  ArduinoOTA.setHostname(otahostname);
  ArduinoOTA.setPassword(otapassword);
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
  
//MQTT
Serial.print("Attempting MQTT connection...");
      String clientName;
      clientName =mqtt_clientname;
      
      if (client.connect((char*) clientName.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("\tMQTT Connected");
        client.subscribe(mqtt_sensor_flash_topic);
      }
      else {
        Serial.println("\tFailed.");
        abort();
      }

//DHT
      dht.begin();
      Serial.println("DHT started.");

}

void loop() {

//If "mqtt_sensor_flash_topic" is 0, then read temperature and humidity and go to deepsleep.
//If "mqtt_sensor_flash_topic" is 1, stay awake, activate OTA and wait for flash.
  
if (!client.connected()) 
  {
    reconnectwifi();
  }
  client.loop();
  delay(1000);
  client.loop();

if (flashtime==0)
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("ERROR: Failed to read from DHT sensor!");
    return;
  } else {
    Serial.println(h);
    Serial.println(t);
    sprintf(t_char,"%f",t);
    sprintf(h_char,"%f",h);

    client.publish(mqtt_sensor_temperature_topic,t_char);
    client.publish(mqtt_sensor_humidity_topic,h_char);
  }

  Serial.println("INFO: Closing the MQTT connection");
  client.disconnect();

  Serial.println("INFO: Closing the Wifi connection");
  WiFi.disconnect();

  ESP.deepSleep(SLEEPING_TIME_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
  delay(500); // wait for deep sleep to happen
}

if(flashtime==1)
  {
  Serial.println("flashtime");
  ArduinoOTA.handle();

  }

}


void callback(char* topic, byte* payload, unsigned int length) {

  //convert topic to string to make it easier to work with
  String topicStr = topic;
  //EJ: Note:  the "topic" value gets overwritten everytime it receives confirmation (callback) message from MQTT

  //Print out some debugging info
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);

  if (topicStr == mqtt_sensor_flash_topic)
  {

    //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
    if (payload[0] == '1') {
      flashtime = 1;
      client.publish(mqtt_sensor_flash_confirm, "1");
    }

    //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
    else if (payload[0] == '0') {
      flashtime = 0;
      client.publish(mqtt_sensor_flash_confirm, "0");
    }
  }

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

/*
// function called to publish the temperature and the humidity
void publishData(float p_temperature, float p_humidity) {
  // create a JSON object
  // doc : https://github.com/bblanchon/ArduinoJson/wiki/API%20Reference
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  // INFO: the data must be converted into a string; a problem occurs when using floats...
  root["temperature"] = (String)p_temperature;
  root["humidity"] = (String)p_humidity;
  root.prettyPrintTo(Serial);
  Serial.println("");
  
     {
        "temperature": "23.20" ,
        "humidity": "43.70"
     }
  
  char data[200];
  root.printTo(data, root.measureLength() + 1);
  client.publish(mqtt_sensor_data_topic, data, true);
}
*/
