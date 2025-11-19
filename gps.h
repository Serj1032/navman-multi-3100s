#pragma once

#include "config.h"
#include "nmea.h"

class GPS {
public:
    ~GPS() = default;

    static GPS& get_instance() {
        static GPS instance;
        return instance;
    }

    void init() {
        GPS_SERIAL.begin(GPS_BAUDRATE);

        ts = millis();
    }

    void process() {
        
        uint32_t now = millis();
        if (now - ts > 1000) {
            ts = now;
            // printf("%s,%s,%.7f,%.7f,%.2f,%.2f\n",
            //     gnss.UTCDate.c_str(),
            //     gnss.UTCTime.c_str(),
            //     gnss.lat, gnss.lon, gnss.velocity, gnss.course);
        }

        while (GPS_SERIAL.available() > 0) {
            char c = GPS_SERIAL.read();
            if (c == '\n' || c == '\r') {
                if (buffer_idx_ == 0) {
                    continue;
                }
                buffer_[buffer_idx_] = '\0';
                Serial.println(buffer_);
                Nmea::get_instance().parse(buffer_, buffer_idx_);
                buffer_idx_ = 0;
            } else {
                buffer_[buffer_idx_++] = c;
                if (buffer_idx_ >= GPS_SERIAL_BUFFER_SIZE) {
                    buffer_idx_ = 0;
                }
            }
        }
    }

private:
    GPS() = default;

private:
    uint32_t ts;
    char buffer_[GPS_SERIAL_BUFFER_SIZE];
    uint16_t buffer_idx_ = 0;
};
