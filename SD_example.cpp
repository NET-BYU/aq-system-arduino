/*
  Listfiles
  This example shows how print out the files in a
  directory on a SD card
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 12
 ** MISO - pin 13
 ** CLK - pin 14
 ** CS - pin 11
*/
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

File root;
const int chipSelect = 15;


void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

// return 0 if all passed
// return -1 if failed
// return -2 if SD not inserted
bool sd_setup() {

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect) && digitalRead(0)) {
    Serial.println("initialization failed!");
    return false;
  }
  Serial.println("initialization done.");

  root = SD.open("/");
  printDirectory(root, 0);  
  File f = SD.open("datalog.csv", FILE_READ);
  if(f){
    // write the headers if there is nothing in the file.
    if(!f.available()){
      f.close();
      f = SD.open("datalog.csv", FILE_WRITE);
      String headers = String("ID, date, time, temp, humidity");
      f.println(headers);
      f.close();
    }else{
      // TODO: 
      // save the number of lines in another file in case failure and reset
      // or just save it to SRAM
    } 
  }else{
    Serial.println("File not opened. Init failed");
    return false;
  }
  
  Serial.println("done!");
  return true;
}
long int counter = 0;

int sd_write(String data) {
  // make a string for assembling the data to log:
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile;
  // check if the SD is inserted
  if(digitalRead(0)){
    // Serial.print(dataFile);
    dataFile = SD.open("datalog.csv", FILE_WRITE);
  }else{
    return -1;
  }
  // if the file is available, write to it:
  Serial.print("Writing to file: ");
  
  Serial.print(dataFile);
  Serial.print(' ');
  if (dataFile && digitalRead(0)) {
    int16_t val = (int16_t)dataFile.println(String(String(counter) + ',' + data));
    counter++;
    if(dataFile){
      dataFile.close();
    }
    return val;
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.csv");
    if(dataFile){
      dataFile.close();
    }
    return -1;
  }
}

