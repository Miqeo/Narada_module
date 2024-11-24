#include "kim_service.h"

Kim_service::Kim_service() {

    #if defined(__AVR_ATmega328P__) or defined(ESP8266) or defined(ESP32)
    kserial = new SoftwareSerial(RX_KIM, TX_KIM);
    #else
    kserial = Serial1;
    #endif

    kim = new KIM(kserial);
};

void Kim_service::setup() {
	kim->KIM_powerON(true);
	kim->KIM_userWakeupPinToggle();

	if (kim->KIM_check()) {
		Serial.println("KIM1 -- Check success");
	} else {
		Serial.println("KIM1 -- Check fail. Please check wiring and jumpers. Freezing.");
		while(1)
			;
	}

	if (kim->KIM_isKIM2() == true)
		Serial.println("KIM2 -- KIM2 module detected");
	else
		Serial.println("KIM1 -- KIM1 module detected");

	Serial.print("KIM1 -- Get ID : ");
	Serial.println(kim->KIM_sendATCommandGet(ID, sizeof(ID) - 1));
	Serial.print("KIM1 -- Get FW : ");
	Serial.println(kim->KIM_sendATCommandGet(FW, sizeof(FW) - 1));
 
 	/* Set Format frame configuration */
	char frameConfiguration[] = "1,16,32";
	Serial.print("KIM1 -- Get Frame Format : ");
	Serial.println(kim->KIM_sendATCommandGet(AFMT, sizeof(AFMT) - 1));
  	kim->KIM_sendATCommandSet(AFMT, sizeof(AFMT) - 1, frameConfiguration, sizeof(frameConfiguration) - 1);
	Serial.print("KIM1 -- Get Frame Format : ");
	Serial.println(kim->KIM_sendATCommandGet(AFMT, sizeof(AFMT) - 1));
 
	/* Set TX configuration */
	char power[] = "1000";
	kim->KIM_sendATCommandSet(PWR, sizeof(PWR) - 1, power, sizeof(power) - 1);
	Serial.print("KIM1 -- Get PWR : ");
	Serial.println(kim->KIM_sendATCommandGet(PWR, sizeof(PWR) - 1));

	/* Save configuration */
	kim->KIM_sendATCommandSet(SAVE, sizeof(SAVE) - 1, (char*)"", sizeof((char*)""));
}

void Kim_service::loop(const GNSS_data*& all_GNSS_data) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= INTERVAL_MS) {
        previousMillis = currentMillis;
        ping();
        sendData(all_GNSS_data);
    }
}

void Kim_service::ping() {
	Serial.print("KIM1 -- Pinging ... ");

	Serial.println(kim->KIM_sendATCommandGet(PING, sizeof(PING) - 1));
}

void Kim_service::sendData(const GNSS_data*& all_GNSS_data) {
	kim->KIM_powerON(true);
    Serial.print("KIM1 -- Send data ... ");

    const char* gga = "%013.8f,%s,%013.8f,%s";
    char gga_full[80];
    sprintf(gga_full, gga, all_GNSS_data->lat, all_GNSS_data->lat_dir, all_GNSS_data->lon, all_GNSS_data->lon_dir);

    Serial.println(gga_full);

    Serial.println("To send");
    delay(100);
	kim->KIM_sendATCommandSet(TXX, sizeof(TXX), gga_full, sizeof(gga_full) - 1);

	switch (kim->KIM_getState()) {
	case KIM_OK:
		Serial.println("Message sent");
		break;
	case KIM_ERROR:
		Serial.println("Error");
		break;
	case KIM_TIMEOUT:
		Serial.println("Message timeout");
		break;
	default:
		Serial.println("Unknown error");
	}

	Serial.println("KIM1 -- Turn OFF");

	kim->KIM_powerON(false);
}