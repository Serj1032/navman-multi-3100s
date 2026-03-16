#pragma once

#include "protocols/nmea_protocol.h"

#include "sensor.h"

struct GpsSolution {
    uint8_t sv_count_ = 0;
    bool is_valid_ = false;
    uint32_t utc_time_ = 0;
    bool utc_time_valid_ = false;
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

class GPS : public Sensor {
public:
    GPS();
    ~GPS() = default;

    const GpsSolution* get_solution() const {
        return &solution_;
    }

    const NmeaStats& get_nmea_stats() const {
        return nmea_.get_stats();
    }

    void process() override;

private:
    int probe() override;

    static void nmea_callback(const NmeaPacket* packet, void* context);
    void parse(const NmeaPacket* packet);
    void parse_gpgsv(const NmeaPacket* packet);
    void parse_gprmc(const NmeaPacket* packet);
    void update_utc_time(const char* utc_time_field);

private:
    uint32_t ts_{0};
    NmeaProtocol nmea_;
    GpsSolution solution_;
    
    // Time interpolation tracking
    uint32_t last_valid_utc_time_{0};  // Last valid UTC time received
    uint32_t last_valid_timestamp_{0};  // System millis() when last valid UTC received
    bool has_valid_time_reference_{false}; // Whether we have a valid time reference for interpolation
};
