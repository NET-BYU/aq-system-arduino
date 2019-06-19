#include "wemos_sht3x.h"

SHT3X sht30_t(0x45);

void sht_setup() {

  Serial.begin(9600);

}

void sht_loop() {

  if(sht30_t.get()==0){
    Serial.print("Temperature in Celsius : ");
    Serial.println(sht30_t.cTemp);
    Serial.print("Temperature in Fahrenheit : ");
    Serial.println(sht30_t.fTemp);
    Serial.print("Relative Humidity : ");
    Serial.println(sht30_t.humidity);
    Serial.println();
  }
  else
  {
    Serial.println("Error!");
  }
  delay(1000);

}
