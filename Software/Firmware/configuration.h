/* Set these to your desired credentials. */
//WIFI
const char *ssid = "Pretty fly for a WiFi";
const char *password = "aaaaaaaaAAAAAAAA11111111!!!!!!!!";

//MQTT
#define MQTT_SERVER "192.168.178.25"  //you MQTT IP Address
#define mqtt_username "homeassistant" 
#define mqtt_password ""
#define mqtt_clientname "Wohnzimmer_sofa_temp_hum_pres"

// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "Wohnzimmer_sofa_temp_hum_pres";
const PROGMEM char* MQTT_SERVER_IP = "192.168.178.25";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "Wohnzimmer_sofa_temp_hum_pres";
const PROGMEM char* MQTT_PASSWORD = "";

// MQTT: topic
const PROGMEM char* MQTT_SENSOR_TOPIC = "Wohnzimmer/Sofa";


//OTA
#define otahostname "Wohnzimmer_sofa_temp_hum_pres"
#define otapassword ""

//DHT22
#define DHTTYPE DHT22

// sleeping time
const uint16_t SLEEPING_TIME_IN_SECONDS = 30; // 10 minutes x 60 seconds

