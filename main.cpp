#include <Arduino.h>
#include "include.h"

int sd_initialized = 0;

int try_sd_init_counter = 0;
void setup(){
    // set the baudrate for comms
    Serial.begin(9600);
    // set the mode of the pins
    pinMode(0, INPUT);
    // set up the pin
    if(sd_setup()){
        sd_initialized = 1;
    }
    rtc_setup ();
    dht_setup();
}

#define SENSOR_WAIT_TIME 6000 // milliseconds

void loop(){
    String date_time = rtc_get();
    String temp = dht_get();
    // if the sd initialization was successful
    // if(sd_initialized != -1){
    Serial.println("Writing SD Card");
    // write to the SD card.
    Serial.print("SD: ");
    Serial.println(digitalRead(0));
    int val = sd_write(String(date_time + ',' + temp)); 
    if(val < 0){
        sd_setup();
    }else{
        Serial.println(val);
    }

    delay(SENSOR_WAIT_TIME);
}