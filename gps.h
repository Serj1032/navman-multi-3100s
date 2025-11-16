#ifndef GPS_H
#define GPS_H

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
    }

    void process() {
        while (GPS_SERIAL.available() > 0) {
            char c = GPS_SERIAL.read();
            if (c == '\n' || c == '\r') {
                if (buffer_idx_ == 0) {
                    continue;
                }
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
    char buffer_[GPS_SERIAL_BUFFER_SIZE];
    uint16_t buffer_idx_ = 0;
};


#endif // GPS_H