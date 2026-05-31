#include "config.h"
#include "navman.h"

#define LOG_TAG "NVMN"
#include "logger.h"

Navman::Navman() : Sensor() {}

int Navman::probe() {
    LOG_DEBUG("Initializing Navman sensor");

    // Navman Multi 3100s has inverted UART data line
    // Using pin 19 as input and pin 18 as output to create an inverter
    pinMode(18, OUTPUT);
    pinMode(19, INPUT); 
    attachInterrupt(digitalPinToInterrupt(19), []() {
        digitalWrite(18, !digitalRead(19));
    }, CHANGE);

    NAVMAN_SERIAL.begin(NAVMAN_BAUDRATE);
    nmea_.set_packet_callback(&Navman::nmea_callback, this);
    return 0;
}

void Navman::process() {
    char c;
    while (NAVMAN_SERIAL.available() > 0) {
        c = NAVMAN_SERIAL.read();
        nmea_.process_char(c);
    }
    check_timeout();
}

void Navman::check_timeout() {
    if (last_data_ts_ == 0) {
        // No data ever received
        solution_.data_available_ = false;
        return;
    }
    uint32_t now = millis();
    if (now - last_data_ts_ > NAVMAN_DATA_TIMEOUT_MS) {
        if (solution_.data_available_) {
            LOG_WARN("Navman data timeout");
            solution_.data_available_ = false;
            solution_.water_temp_valid_ = false;
            solution_.depth_valid_ = false;
            solution_.water_speed_valid_ = false;
            solution_.distance_valid_ = false;
        }
    }
}

void Navman::nmea_callback(const NmeaPacket* packet, void* context) {
    Navman* navman = static_cast<Navman*>(context);
    if (navman) {
        navman->parse(packet);
    }
}

void Navman::parse(const NmeaPacket* packet) {
    const char* id = packet->packet_id();
    if (!id) return;

    last_data_ts_ = millis();
    solution_.data_available_ = true;

    if (strncmp(id, "VWMTW", 5) == 0) {
        parse_vwmtw(packet);
    } else if (strncmp(id, "SDDBT", 5) == 0) {
        parse_sddbt(packet);
    } else if (strncmp(id, "SDDPT", 5) == 0) {
        parse_sddpt(packet);
    } else if (strncmp(id, "VWVHW", 5) == 0) {
        parse_vwvhw(packet);
    } else if (strncmp(id, "VWVLW", 5) == 0) {
        parse_vwvlw(packet);
    }
    //  else if (strncmp(id, "PTTKD", 5) == 0) {
    //     parse_pttkd(packet);
    // } else if (strncmp(id, "PTTKV", 5) == 0) {
    //     parse_pttkv(packet);
    // }
}

// $VWMTW,<temperature>,C*XX
void Navman::parse_vwmtw(const NmeaPacket* packet) {
    const char* temp = packet->field(1);
    if (temp && temp[0] != '\0') {
        solution_.water_temp_c_ = atof(temp);
        solution_.water_temp_valid_ = true;
    }
}

// $SDDBT,<feet>,f,<meters>,M,<fathoms>,F*XX
void Navman::parse_sddbt(const NmeaPacket* packet) {
    const char* feet = packet->field(1);
    if (feet && feet[0] != '\0') {
        solution_.depth_ft_ = atof(feet);
    }
    const char* meters = packet->field(3);
    if (meters && meters[0] != '\0') {
        solution_.depth_m_ = atof(meters);
    }
    const char* fathoms = packet->field(5);
    if (fathoms && fathoms[0] != '\0') {
        solution_.depth_fathoms_ = atof(fathoms);
    }
    solution_.depth_valid_ = true;
}

// $SDDPT,<depth_m>,<offset_m>,*XX
void Navman::parse_sddpt(const NmeaPacket* packet) {
    const char* depth = packet->field(1);
    if (depth && depth[0] != '\0') {
        solution_.depth_m_ = atof(depth);
    }
    const char* offset = packet->field(2);
    if (offset && offset[0] != '\0') {
        solution_.depth_offset_m_ = atof(offset);
    }
    solution_.depth_valid_ = true;
}

// $VWVHW,<heading_true>,T,<heading_mag>,M,<speed_kn>,N,<speed_kmh>,K*XX
void Navman::parse_vwvhw(const NmeaPacket* packet) {
    const char* hdg_true = packet->field(1);
    if (hdg_true && hdg_true[0] != '\0') {
        solution_.heading_true_ = atof(hdg_true);
    }
    const char* hdg_mag = packet->field(3);
    if (hdg_mag && hdg_mag[0] != '\0') {
        solution_.heading_magnetic_ = atof(hdg_mag);
    }
    const char* spd_kn = packet->field(5);
    if (spd_kn && spd_kn[0] != '\0') {
        solution_.water_speed_knots_ = atof(spd_kn);
    }
    const char* spd_kmh = packet->field(7);
    if (spd_kmh && spd_kmh[0] != '\0') {
        solution_.water_speed_kmh_ = atof(spd_kmh);
    }
    solution_.water_speed_valid_ = true;
}

// $VWVLW,<total_nm>,N,<since_reset_nm>,N*XX
void Navman::parse_vwvlw(const NmeaPacket* packet) {
    const char* total = packet->field(1);
    if (total && total[0] != '\0') {
        solution_.distance_total_nm_ = atof(total);
    }
    const char* since_reset = packet->field(3);
    if (since_reset && since_reset[0] != '\0') {
        solution_.distance_since_reset_nm_ = atof(since_reset);
    }
    solution_.distance_valid_ = true;
}

// $PTTKD,<depth>,<offset>,<unit>*XX  (proprietary depth)
void Navman::parse_pttkd(const NmeaPacket* packet) {
    const char* depth = packet->field(1);
    if (depth && depth[0] != '\0') {
        solution_.depth_m_ = atof(depth);
    }
    const char* offset = packet->field(2);
    if (offset && offset[0] != '\0') {
        solution_.depth_offset_m_ = atof(offset);
    }
    solution_.depth_valid_ = true;
}

// $PTTKV,<speed>,<>,<avg_speed>,<trip_nm>,<total_nm>,<temp>,<temp_unit>*XX  (proprietary velocity/trip)
void Navman::parse_pttkv(const NmeaPacket* packet) {
    const char* speed = packet->field(1);
    if (speed && speed[0] != '\0') {
        solution_.water_speed_knots_ = atof(speed);
        solution_.water_speed_valid_ = true;
    }
    const char* trip = packet->field(4);
    if (trip && trip[0] != '\0') {
        solution_.distance_since_reset_nm_ = atof(trip);
    }
    const char* total = packet->field(5);
    if (total && total[0] != '\0') {
        solution_.distance_total_nm_ = atof(total);
        solution_.distance_valid_ = true;
    }
    const char* temp = packet->field(6);
    if (temp && temp[0] != '\0') {
        solution_.water_temp_f_ = atof(temp);
        solution_.water_temp_valid_ = true;
    }
}
