
#define MQTT_VERSION MQTT_VERSION_3_1_1

// Wifi: SSID and password
const char* WIFI_SSID = "FRITZ!Box 7490";
const char* WIFI_PASSWORD = "11507805580202423221";

// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "Roomsensor_bedroom";
const PROGMEM char* MQTT_SERVER_IP = "192.168.178.62";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "homeassistant";
const PROGMEM char* MQTT_PASSWORD = "";

// MQTT: topic
const PROGMEM char* MQTT_SENSOR_TOPIC = "room/bedroom";

// sleeping time
const PROGMEM uint16_t SLEEPING_TIME_IN_SECONDS = 30; // 10 minutes x 60 seconds

// DHT - D1/GPIO5
#define DHTPIN D4
#define DHTTYPE DHT22

