#include <gps_service.h>

int8_t data_available_callback(void) {
    int loAvailable = Serial2.available();
    if (0 < loAvailable) {
        return (1);
    }

    return (0);
}

int8_t data_read_callback(void) {
    uint8_t data = Serial2.read();
    return data;
}

Gps_service::Gps_service() {
    myGNSS = new GNSSCollector(&data_available_callback, &data_read_callback);
}

void Gps_service::setup() {
    myGNSS->setBreakTime(5);
    Serial2.begin(9600);

    const unsigned char command_EnableGPGBS_uBlox[]  = {0xb5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x09, 0x01, 0x04, 0x22};
    const unsigned char command_EnableGPGST_uBlox[]  = {0xb5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x07, 0x01, 0x02, 0x1e};

    Serial2.write(command_EnableGPGST_uBlox, sizeof(command_EnableGPGST_uBlox));
    delay(100);

    Serial2.write(command_EnableGPGBS_uBlox, sizeof(command_EnableGPGBS_uBlox));
    delay(100);
}

void Gps_service::loop(const GNSS_data*& all_GNSS_data) {

    if (0 == Serial2.available()) {
        return;
    }
    
    myGNSS->collectData(false, true);
    myGNSS->printGNSSData(true);

    const GNSS_data* position = myGNSS->getGNSSData();
    
    if (position->pos_status == 'A') {
        all_GNSS_data = position;
    }
}
