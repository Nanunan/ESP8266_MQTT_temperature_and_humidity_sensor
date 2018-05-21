/* Set these to your desired credentials. */
//WIFI
const char *ssid = "YOURSSID";
const char *password = "YOURPW";

//MQTT
#define MQTT_SERVER "192.168.178.62"  //you MQTT IP Address
#define mqtt_username "homeassistant" 
#define mqtt_password ""
#define mqtt_clientname "room_sensor"

char const* mqtt_sensor_humidity_topic = "room/bedroom/data/h"; //room/bedroom/data
char const* mqtt_sensor_temperature_topic = "room/bedroom/data/t"; //room/bedroom/data
char const* mqtt_sensor_flash_topic = "room/bedroom/flash";
char const* mqtt_sensor_flash_confirm = "room/bedroom/confirm";

//OTA
#define otahostname "room_sensor"
#define otapassword "123"

//DHT22
#define DHTTYPE DHT22

// sleeping time
const uint16_t SLEEPING_TIME_IN_SECONDS = 30; // 10 minutes x 60 seconds


