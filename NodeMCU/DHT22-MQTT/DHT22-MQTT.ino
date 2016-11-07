/**
 * Copyright 2016   Rene Kremer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Delay to sleep after loop
#define SLEEP_DELAY_IN_SECONDS  600
// Length of array of sensor data
#define ARRAY_LENGTH 250
// default value if position in array has no value
#define DEFAULT_VALUE 999
// what pin we're connected to
#define DHTPIN D4     
// Type of DHT
#define DHTTYPE DHT22
// DHT object
DHT dht(DHTPIN, DHTTYPE, 20);
// name of wlan
const char* ssid = "wlan";
// password for wlan
const char* password = "password";
// broker
const char* mqtt_server = "192.168.0.125";
// this client in WiFi
WiFiClient espClient;
// this mqtt client
PubSubClient client(espClient, mqtt_server, 1883);
// static ip of this node
IPAddress ip(192, 168, 0, 126);
// gateway for to connect this node to
IPAddress gateway(192, 168, 0, 1);
// subnet
IPAddress subnet(255, 255, 255, 0);
// floats for (avg) humidity and (avg) temperature
float h, t, avgH, avgT;
// counter for calcAvg function
int counter;
// arrays of sensor data
float temperatures[ARRAY_LENGTH], humidities[ARRAY_LENGTH];
// setup function
void setup() {
  // setup WiFi
  setup_wifi();
  dht.begin();
}
// sets up the wifi
void setup_wifi() {
  delay(10);

  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}
// reconnects to mqtt broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (!client.connect("ESP8266Client")) {
      delay(5000);
    }
  }
}
// calcs the avg of the sensor values
void calcAvg() {
  counter = 0;
  float tmpH = 0;
  float tmpT = 0;
  for (int i = 0; i < ARRAY_LENGTH; i++) {
    if (humidities[i] != DEFAULT_VALUE && temperatures[i] != DEFAULT_VALUE) {
      tmpH += humidities[i];
      tmpT += temperatures[i];
      counter++;
    }
  }
  avgH = tmpH / counter;
  avgT = tmpT / counter;
}
// gets the data from the dht sensor and calcs the average of the read data to get a more detailed value
void getData() {
  for (int i = 0; i < ARRAY_LENGTH; i++) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h = dht.readHumidity();
    // Read temperature as Celsius
    t = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      h = 0.0;
      t = 0.0;
      return;
    }
    humidities[i] = h;
    temperatures[i] = t;
  }
  calcAvg();
}
// well loop, getting values, sending them, deep sleeping
void loop() {
  for (int i = 0; i < ARRAY_LENGTH; i++) {
    temperatures[i] = DEFAULT_VALUE;
    humidities[i] = DEFAULT_VALUE;
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  getData();

  client.publish("temperature/living-room", String(avgT));
  client.publish("humidity/living-room", String(avgH));

  client.disconnect();

  WiFi.disconnect();

  delay(100);

  ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
  delay(500);   // wait for deep sleep to happen
}
