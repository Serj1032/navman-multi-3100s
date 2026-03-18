#pragma once

#include "protocols/nmea_protocol.h"
#include "sensor.h"

// Timeout: if no data received for this period, mark solution as unavailable
#define NAVMAN_DATA_TIMEOUT_MS 5000

struct NavmanSolution {
    // Water temperature (VWMTW / PTTKV)
    float water_temp_c_{0.0f};
    float water_temp_f_{0.0f};
    bool water_temp_valid_{false};

    // Depth (SDDBT / SDDPT)
    float depth_m_{0.0f};
    float depth_ft_{0.0f};
    float depth_fathoms_{0.0f};
    float depth_offset_m_{0.0f};
    bool depth_valid_{false};

    // Water speed & heading (VWVHW)
    float water_speed_knots_{0.0f};
    float water_speed_kmh_{0.0f};
    float heading_true_{0.0f};
    float heading_magnetic_{0.0f};
    bool water_speed_valid_{false};

    // Distance traveled (VWVLW)
    float distance_total_nm_{0.0f};
    float distance_since_reset_nm_{0.0f};
    bool distance_valid_{false};

    // Overall data availability (set false on timeout)
    bool data_available_{false};
};

class Navman : public Sensor {
public:
    Navman();
    ~Navman() = default;

    const NavmanSolution* get_solution() const {
        return &solution_;
    }

    const NmeaStats& get_nmea_stats() const {
        return nmea_.get_stats();
    }

    NmeaProtocol& get_nmea_protocol() { return nmea_; }

    void process() override;

private:
    int probe() override;

    static void nmea_callback(const NmeaPacket* packet, void* context);
    void parse(const NmeaPacket* packet);

    void parse_vwmtw(const NmeaPacket* packet);
    void parse_sddbt(const NmeaPacket* packet);
    void parse_sddpt(const NmeaPacket* packet);
    void parse_vwvhw(const NmeaPacket* packet);
    void parse_vwvlw(const NmeaPacket* packet);
    void parse_pttkd(const NmeaPacket* packet);
    void parse_pttkv(const NmeaPacket* packet);

    void check_timeout();

private:
    NmeaProtocol nmea_;
    NavmanSolution solution_;
    uint32_t last_data_ts_{0};
};
