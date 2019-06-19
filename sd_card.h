
#ifndef __SD_CARD
#define __SD_CARD
#include <SPI.h>
#include <SD.h>


void printDirectory(File dir, int numTabs);
void sd_setup();
void log_data(String data);


#endif
