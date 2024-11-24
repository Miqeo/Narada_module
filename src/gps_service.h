#ifndef gpsh
#define gpsh

#include <ultimateGNSSParser.h>

class Gps_service {
    GNSSCollector *myGNSS;
    public:
    Gps_service();

    void setup();
    void loop(const GNSS_data*& all_GNSS_data);
};

#endif