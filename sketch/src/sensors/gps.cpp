/**
 * Protocol:
 * https://content.u-blox.com/sites/default/files/products/documents/u-blox6_ReceiverDescrProtSpec_%28GPS.G6-SW-10018%29_Public.pdf
 * 
 * NMEA sentences examples:
 *  $GPRMC,225530.00,V,,,,,,,191125,,,N*71
 *  $GPVTG,,,,,,,,,N*30
 *  $GPGGA,225530.00,,,,,0,00,99.99,,,,,,*65
 *  $GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30
 *  $GPGSV,3,1,11,01,01,265,18,02,29,279,,08,57,300,,10,76,089,*7E
 *  $GPGSV,3,2,11,15,02,027,,16,29,205,29,18,14,079,17,23,36,049,*76
 *  $GPGSV,3,3,11,26,02,185,,27,83,197,21,32,22,137,27*40
 *  $GPGLL,,,,,225530.00,V,N*49
 */

#include "config.h"
#include "gps.h"

#include "logger.h"

GPS::GPS() : Sensor() {}

int GPS::probe() {
    LOG_DEBUG("Initializing GPS sensor");
    GPS_SERIAL.begin(GPS_BAUDRATE);
    nmea_.set_packet_callback(&GPS::nmea_callback, this);
    ts_ = millis();
    return 0;
}

void GPS::process() {
    char c;
    while (GPS_SERIAL.available() > 0) {
        c = GPS_SERIAL.read();
        nmea_.process_char(c);
    }
}

void GPS::nmea_callback(const NmeaPacket* packet, void* context) {
    GPS* gps = static_cast<GPS*>(context);
    if (gps) {
        gps->parse(packet);
    }
}

void GPS::parse(const NmeaPacket* packet) {
    if (strncmp(packet->packet_id(), "GPRMC", 5) == 0) {
        parse_gprmc(packet);
    } else if (strncmp(packet->packet_id(), "GPVTG", 5) == 0) {
        // Handle VTG message
    } else if (strncmp(packet->packet_id(), "GPGGA", 5) == 0) {
        // Handle GGA message
    } else if (strncmp(packet->packet_id(), "GPGSA", 5) == 0) {
        // Handle GSA message
    } else if (strncmp(packet->packet_id(), "GPGSV", 5) == 0) {
        parse_gpgsv(packet);
    } else if (strncmp(packet->packet_id(), "GPGLL", 5) == 0) {
        // Handle GLL message
    }
}

void GPS::parse_gpgsv(const NmeaPacket* packet) {
    // Field 3: Number of satellites in view
    const char* sv_count = packet->field(3);
    if (sv_count) {
        solution_.sv_count_ = atoi(sv_count);
    }
}

void GPS::parse_gprmc(const NmeaPacket* packet) {    
    // Field 1: UTC time
    const char* utc_time = packet->field(1);
    if (utc_time) {
        solution_.utc_time_ = atol(utc_time);
    }
    
    // Field 2: Status (A=valid, V=invalid)
    const char* status = packet->field(2);
    if (status) {
        solution_.is_valid_ = (status[0] == 'A');
    } else {
        solution_.is_valid_ = false;
    }
    
    // Field 3: Latitude
    const char* latitude = packet->field(3);
    if (latitude) {
        solution_.latitude_ = atof(latitude);
    }
    
    // Field 4: Latitude direction
    const char* lat_dir = packet->field(4);
    if (lat_dir) {
        solution_.lat_dir_ = lat_dir[0];
    }
    
    // Field 5: Longitude
    const char* longitude = packet->field(5);
    if (longitude) {
        solution_.longitude_ = atof(longitude);
    }
    
    // Field 6: Longitude direction
    const char* lon_dir = packet->field(6);
    if (lon_dir) {
        solution_.lon_dir_ = lon_dir[0];
    }
    
    // Field 7: Speed over ground (knots)
    const char* speed = packet->field(7);
    if (speed) {
        solution_.speed_knots_ = atof(speed);
    }
    
    // Field 8: Course over ground
    const char* course = packet->field(8);
    if (course) {
        solution_.course_ = atof(course);
        solution_.course_is_valid_ = true;
    } else {
        solution_.course_is_valid_ = false;
    }
    
    // Field 9: Date
    const char* date = packet->field(9);
    if (date) {
        solution_.date_ = atol(date);
    }
    
    // Field 10: Magnetic variation
    const char* mag_var = packet->field(10);
    if (mag_var) {
        solution_.magnetic_variation_ = atof(mag_var);
    }
    
    // Field 11: Magnetic variation direction
    const char* mag_var_dir = packet->field(11);
    if (mag_var_dir) {
        solution_.mag_var_dir_ = mag_var_dir[0];
    }
    
    // Field 12: Mode
    const char* mode = packet->field(12);
    if (mode) {
        solution_.mode = mode[0];
    }
}