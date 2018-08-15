/* Set these to your desired credentials. */
//WIFI
const char *ssid = "YOURSSID";
const char *password = "YOURPW";

//MQTT
#define MQTT_SERVER "192.x.x.x"  //you MQTT IP Address
#define mqtt_username "homeassistant" 
#define mqtt_password ""
#define mqtt_clientname "YOURCLIENTNAME"

// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "YOURCLIENTID";
const PROGMEM char* MQTT_SERVER_IP = "YOURSERVERID";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "YOURUSERNAME";
const PROGMEM char* MQTT_PASSWORD = "";

// MQTT: topic
const PROGMEM char* MQTT_SENSOR_TOPIC = "MQTTTOPIC";


// sleeping time
const uint16_t SLEEPING_TIME_IN_SECONDS = 30; // 10 minutes x 60 seconds

