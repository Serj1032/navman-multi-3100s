#pragma once

#include <Arduino.h>

namespace {
    static const uint16_t NMEA_MAX_SIZE = 256; 
    static const uint8_t NMEA_MAX_FIELD_AMOUNT = 20; 
};

class NmeaPacket {
public:
    NmeaPacket(char* buffer, uint16_t size);
    virtual ~NmeaPacket() = default;

    const char* packet_id() const;

    const char* field(uint8_t index) const;

    uint8_t field_count() const { return field_count_; }

private:
    void parse_fields(char* buffer, uint16_t size);

private:
    char* fields_[NMEA_MAX_FIELD_AMOUNT];
    uint8_t field_count_ = 0;
};

using OnNmeaPacketCallback = void(*)(const NmeaPacket* packet, void* context);

class NmeaProtocol {
public:
    NmeaProtocol() = default;
    virtual ~NmeaProtocol() = default;

    void process_char(char c);

    void set_packet_callback(OnNmeaPacketCallback callback, void* context = nullptr) {
        packet_callback_ = callback;
        callback_context_ = context;
    }

private:
    bool is_checksum_valid(const char* msg, uint16_t size);

private:
    char buffer_[NMEA_MAX_SIZE];
    uint16_t buffer_idx_ = 0;
    OnNmeaPacketCallback packet_callback_ = nullptr;
    void* callback_context_ = nullptr;
};
