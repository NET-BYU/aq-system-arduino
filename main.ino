
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SPI.h>



#include "wemos_sht3x.h"
#include "hpma115s0.h"
#include "RTClib.h"
#include "sd_card.h"

// SD card 
File root;
#define SS_PIN 10
#define LOG_DATA_WAIT 15000


// Real Time Clock 
#define TIMESTAMP_WAIT_TIME 3000

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



// AQ Sensor
// uart software pins. These are the mappings on the uno. Tx of sensor should go to Uno_Rx, etc.
#define UNO_RX 5
#define UNO_TX 6

bool my_status;
float p25;
float p10;

SoftwareSerial mySerial(UNO_RX, UNO_TX); // RX, TX
HPMA115S0 my_hpm(mySerial);

// Temp and Humidity Sensor
#define SHT30_I2C_ADDR 0x45
#define SHT30_WAIT_TIME 1000

SHT3X sht30(SHT30_I2C_ADDR);

#define AQ_WAIT_TIME 15000
long count = 0;
void setup(){
    Serial.begin(9600);
    mySerial.begin(9600);
    delay(100);

     // Stop autosend
    my_status = my_hpm.stop_autosend(); 
    if (my_status == 1){
        Serial.println("Autosend disabled");
    }
    else{
    Serial.print("Error");  
    }
    delay(500);
    
    // Start fan (and measurement mode)
    my_status = my_hpm.start_measurement();  
    if (my_status == 1){
        Serial.println("Start Particle Measurement");
    }
    else{
    Serial.print("Error");  
    }
    delay(5000);


    #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
    #endif

    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, lets set the time!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    // SD card code
    // Open serial communications and wait for port to open:

    Serial.print("Initializing SD card...");

    if (!SD.begin(SS_PIN)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    root = SD.open("/");

    printDirectory(root, 0);

    Serial.println("done!");
}

int tempTimer, aqTimer, timestampTimer, logDataTimer = 0;

void loop(){
    int time = millis();
    // temp sensor code
    if(time - tempTimer > SHT30_WAIT_TIME){
        if(sht30.get()==0){
        Serial.print("Temperature in Celsius : ");
        Serial.println(sht30.cTemp);
        Serial.print("Temperature in Fahrenheit : ");
        Serial.println(sht30.fTemp);
        Serial.print("Relative Humidity : ");
        Serial.println(sht30.humidity);
        Serial.println();
        }
        tempTimer = millis();
    }

    // 
    if(time - aqTimer > AQ_WAIT_TIME){
        // Read the particle data every fifteen seconds
        my_status = my_hpm.read(&p25,&p10); 
        if (my_status == 1){
            Serial.print("PM2.5 value is ");
            Serial.println(p25);
            Serial.print("PM10 value is ");
            Serial.println(p10);
            Serial.println(" ");
        }
        else{
            Serial.println("Measurement fail");  
        }	 
        aqTimer = millis();
    }

    if(time - timestampTimer > TIMESTAMP_WAIT_TIME){
        DateTime now = rtc.now();
        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(" (");
        Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.print(") ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
        
        Serial.print(" since midnight 1/1/1970 = ");
        Serial.print(now.unixtime());
        Serial.print("s = ");
        Serial.print(now.unixtime() / 86400L);
        Serial.println("d");
        
        // calculate a date which is 7 days and 30 seconds into the future
        DateTime future (now + TimeSpan(7,12,30,6));
        
        Serial.print(" now + 7d + 30s: ");
        Serial.print(future.year(), DEC);
        Serial.print('/');
        Serial.print(future.month(), DEC);
        Serial.print('/');
        Serial.print(future.day(), DEC);
        Serial.print(' ');
        Serial.print(future.hour(), DEC);
        Serial.print(':');
        Serial.print(future.minute(), DEC);
        Serial.print(':');
        Serial.print(future.second(), DEC);
        Serial.println();
        
        Serial.println();
        timestampTimer = millis();
    }

    if(logDataTimer - time > LOG_DATA_WAIT){
        String data_string = String(count) + "\n";
        log_data(data_string);
        count += 1;
    }

}
