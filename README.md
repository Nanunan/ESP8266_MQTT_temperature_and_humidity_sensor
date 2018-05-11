# ESP8266_MQTT_temperature_and_humidity_sensor

This is an ESP8266 based project to monitor the temperature and humidity in your house / rooms. 
The sensor publishes the data to an MQTT topic. Deep sleep is used, not to distort the meassurements of the sensor I use Home Asssistant to display the results.

## Features
  -ESP8266 with deep sleep
  -easy configuration window
  -MQTT publish
  -supports DHT22 sensor
  -home assistant integration included
  
## Layout
![layout](https://github.com/Nanunan/ESP8266_MQTT_temperature_and_humidity_sensor/blob/master/Media/Layout_DHT22.png)

 - DHT22 leg 1 - VCC
 - DHT22 leg 2 - D1/GPIO5
 - DHT22 leg 4 - GND
 - D0/GPIO16 - RST (wake-up purpose)
## To do:
  -include BME280 sensor
  -include home assistant template
