# ESP8266_MQTT_temperature_and_humidity_sensor

This is an ESP8266 based project to monitor the temperature and humidity in your house / rooms. 
The sensor publishes the data to an MQTT topic. Deep sleep is used, not to distort the meassurements of the sensor I use Home Asssistant to display the results.

## Features
  -ESP8266 with deep sleep
  -easy configuration window
  -MQTT publish
  -supports BME280 sensor
  -home assistant integration included
  
## Layout
![layout](https://github.com/Nanunan/ESP8266_MQTT_temperature_and_humidity_sensor/blob/master/Media/Layout_DHT22.png)


Connecting the BME280 Sensor:
Sensor        ->        Board
- Vin (Voltage In)    ->  3.3V
- Gnd (Ground)        ->  Gnd
- SDA (Serial Data)   ->  D2 on ESP8266
- SCK (Serial Clock)  ->  D1 on ESP8266

Board         ->        Board
- 0/GPIO16 - RST (wake-up purpose)


## To do:
  - include DHT22 sensor
  - include BME 180 sensor
  
  
## Home assistant template

 ### sensor:
  - platform: mqtt
    - state_topic: 'SENSORTOPIC'
    - name: 'NameTemperature'
    - unit_of_measurement: '°C'
    - value_template: '{{ value_json.temperature }}'
  - platform: mqtt
    - state_topic: 'SENSORTOPIC'
    - name: 'NameHumidity'
    - unit_of_measurement: '%'
    - value_template: '{{ value_json.humidity }}'
  - platform: mqtt
    - state_topic: 'SENSORTOPIC'
    - name: 'NamePressure'
    - unit_of_measurement: 'hPa'
    - value_template: '{{ value_json.pressure }}'
