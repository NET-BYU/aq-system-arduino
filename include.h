#ifndef INCLUDE_
#define INCLUDE_

void rtc_setup ();
bool sd_setup();
void dht_setup();

String dht_get();
String rtc_get();
int sd_write(String data);

#endif