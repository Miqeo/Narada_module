#include <Arduino.h>
#include "gps_service.h"
#include "kim_service.h"

Kim_service kim_service;
Gps_service gps_service;

void setup() {
	Serial.begin(9600);
	Serial.print("Narada module V0.1");
	#if defined(ESP32)
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
  	#endif

	gps_service.setup();
	kim_service.setup();
}

void loop() {

	const GNSS_data* all_GNSS_data;

	gps_service.loop(all_GNSS_data);

	if (all_GNSS_data == nullptr) {
		return;
	}

	Serial.printf("last location %0.8f ,%0.8f\n", all_GNSS_data->lat, all_GNSS_data->lon);

	kim_service.loop(all_GNSS_data);
}