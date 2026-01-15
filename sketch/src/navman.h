#ifndef NAVMAN_H
#define NAVMAN_H

#include "config.h"
#include "protocols/nmea_protocol.h"


class Navman : public NmeaProtocol {
public:
    ~Navman() = default;

    static Navman& get_instance() {
        static Navman instance;
        return instance;
    }

    void init() {
        NAVMAN_SERIAL.begin(NAVMAN_BAUDRATE);
    }

    bool read_serial(char* c) override {
        if (NAVMAN_SERIAL.available() > 0) {
            *c = NAVMAN_SERIAL.read();
            return true;
        }
        return false;
    }

    void parse(char* msg, uint16_t size) override {
        // Implement parsing logic here
        Serial.print("Parsed Navman message: ");
        Serial.println(msg);
    }


private:
    Navman() : NmeaProtocol() {}

private:

};


#endif // NAVMAN_H