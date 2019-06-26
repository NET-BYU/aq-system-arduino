#include <Arduino.h>
#include "dht.h"

dht sensor;

void dht_setup() {
  // put your setup code here, to run once:

}

int timer, read_timer;


String dht_get() {
  // put your main code here, to run repeatedly:
    sensor.read22(16);
    Serial.print("Temperature: ");
    Serial.println(sensor.temperature);
    Serial.print("Humidity: ");
    Serial.println(sensor.humidity);
    String data = String(String(sensor.temperature) + "," + String(sensor.humidity));
    return data;
}
