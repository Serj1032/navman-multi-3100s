// https://content.u-blox.com/sites/default/files/products/documents/u-blox6_ReceiverDescrProtSpec_%28GPS.G6-SW-10018%29_Public.pdf

#pragma once

#include "config.h"
#include "protocols/nmea_protocol.h"

/*
$GPRMC,225530.00,V,,,,,,,191125,,,N*71
$GPVTG,,,,,,,,,N*30
$GPGGA,225530.00,,,,,0,00,99.99,,,,,,*65
$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30
$GPGSV,3,1,11,01,01,265,18,02,29,279,,08,57,300,,10,76,089,*7E
$GPGSV,3,2,11,15,02,027,,16,29,205,29,18,14,079,17,23,36,049,*76
$GPGSV,3,3,11,26,02,185,,27,83,197,21,32,22,137,27*40
$GPGLL,,,,,225530.00,V,N*49
*/

class GPS : public NmeaProtocol {
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

    bool read_serial(char* c) override {
        if (GPS_SERIAL.available() > 0) {
            *c = GPS_SERIAL.read();
            return true;
        }
        return false;
    }

    void parse(char* msg, uint16_t size) override {
        if (strncmp(msg, "$GPRMC", 6) == 0) {
            // Serial.println(msg);
            course_is_valid_ = false;
            parse_nmea_words(msg, size, &GPS::parse_gprmc, this);
        } else if (strncmp(msg, "$GPVTG", 6) == 0) {
            // Handle VTG message
        } else if (strncmp(msg, "$GPGGA", 6) == 0) {
            // Handle GGA message
        } else if (strncmp(msg, "$GPGSA", 6) == 0) {
            // parse_gpgsa(msg, size);
            // Handle GSA message
        } else if (strncmp(msg, "$GPGSV", 6) == 0) {
            parse_nmea_words(msg, size, &GPS::parse_gpgsv, this);
        } else if (strncmp(msg, "$GPGLL", 6) == 0) {
            // Handle GLL message
        }
    }

    uint8_t get_sv_count() const {
        return sv_count_;
    }

    uint32_t get_utc_time() const {
        return utc_time_;
    }

    bool is_valid() const {
        return is_valid_;
    }

    float get_course() const {
        return course_;
    }

    bool is_course_valid() const {
        return course_is_valid_;
    }

    float get_speed_knots() const {
        return speed_knots_;
    }

    const char get_mode() const {
        return mode;
    }

private:
    GPS() : NmeaProtocol() {}

    static void parse_gpgsv(uint8_t field_idx, char* word, void* context) {
        GPS* gps = static_cast<GPS*>(context);
        switch (field_idx) {
            case 3: // Number of satellites in view
                gps->sv_count_ = atoi(word);
                break;
            default:
                break;
        }
    }

    static void parse_gprmc(uint8_t field_idx, char* word, void* context) {
        GPS* gps = static_cast<GPS*>(context);
        switch (field_idx) {
            case 1: // UTC time
                gps->utc_time_ = atol(word);
                break;
            case 2: // Status (A=valid, V=invalid)
                gps->is_valid_ = (word[0] == 'A');
                break;
            case 3: // Latitude
                gps->latitude_ = atof(word);
                break;
            case 4: // Latitude direction
                gps->lat_dir_ = word[0];
                break;
            case 5: // Longitude
                gps->longitude_ = atof(word);
                break;
            case 6: // Longitude direction
                gps->lon_dir_ = word[0];
                break;
            case 7: // Speed over ground (knots)
                gps->speed_knots_ = atof(word);
                break;
            case 8: // Course over ground
                gps->course_ = atof(word);
                gps->course_is_valid_ = true;
                break;
            case 9: // Date
                gps->date_ = atol(word);
                break;
            case 10: // Magnetic variation
                gps->magnetic_variation_ = atof(word);
                break;
            case 11: // Magnetic variation direction
                gps->mag_var_dir_ = word[0];
                break;
            case 12: // Mode
                gps->mode = word[0];
                break;
            default:
                break;
        }
    }


private:
    uint32_t ts;

    uint8_t sv_count_ = 0;
    bool is_valid_ = false;
    uint32_t utc_time_ = 0;
    float latitude_ = 0.0f;
    char lat_dir_ = 'N';
    float longitude_ = 0.0f;
    char lon_dir_ = 'E';
    float speed_knots_ = 0.0f;
    float course_ = 0.0f;
    bool course_is_valid_ = false;
    uint32_t date_ = 0;
    float magnetic_variation_ = 0.0f;
    char mag_var_dir_ = 'E';
    char mode = 'N'; // N = No fix, A = Autonomous, D = Differential, E = Estimated
};
