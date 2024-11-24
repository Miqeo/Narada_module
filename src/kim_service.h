#ifndef kimh
#define kimh

#include <ultimateGNSSParser.h>
#include "KIM_Arduino_Library.h"
#include "ArduinoCbor.h"

class Kim_service {
    /*-- CONFIGURATION START --*/
    #define TX_PERIOD 10000//50000
    char DATA[47] = "12345678123456781234567812345678123456AABBCCDD";

    /*-- CONFIGURATION END --*/

    #define TX_DURATION_MS  6000
    #define INTERVAL_MS     (TX_PERIOD-TX_DURATION_MS)

    /*-- AT COMMANDS START --*/
    char ID[3]  = "ID";
    char FW[3]  = "FW";
    char TXX[3]  = "TX";
    char PWR[4] = "PWR";
    char AFMT[5] = "AFMT";
    char SAVE[9] = "SAVE_CFG";
    char PING[5] = "PING";
    /*-- AT COMMANDS END --*/

    #if defined(__AVR_ATmega328P__) or defined(ESP8266) or defined(ESP32)
    SoftwareSerial* kserial;
    #else
    HardwareSerial* kserial;
    #endif

    KIM* kim;

    unsigned long previousMillis = 0;


    void ping();
    void sendData(const GNSS_data*& all_GNSS_data);

    public:
    Kim_service();

    void setup();
    void loop(const GNSS_data*& all_GNSS_data);
};

#endif