#ifndef NAVMAN_H
#define NAVMAN_H

#include "config.h"
#include "nmea.h"

class Navman {
public:
    ~Navman() = default;

    static Navman& get_instance() {
        static Navman instance;
        return instance;
    }

    void init() {
        NAVMAN_SERIAL.begin(NAVMAN_BAUDRATE);
    }

    void process() {
        while (NAVMAN_SERIAL.available() > 0) {
            char c = NAVMAN_SERIAL.read();
            Serial.print(c);
            if (c == '\n' || c == '\r') {
                if (buffer_idx_ == 0) {
                    continue;
                }
                buffer_[buffer_idx_] = '\0';
                // Serial.print(buffer_);
                Nmea::get_instance().parse(buffer_, buffer_idx_);
                buffer_idx_ = 0;
            } else {
                buffer_[buffer_idx_++] = c;
                if (buffer_idx_ >= NAVMAN_SERIAL_BUFFER_SIZE) {
                    buffer_idx_ = 0;
                }
            }
        }
    }

private:
    Navman() = default;

private:
    char buffer_[NAVMAN_SERIAL_BUFFER_SIZE];
    uint16_t buffer_idx_ = 0;
};


#endif // NAVMAN_H