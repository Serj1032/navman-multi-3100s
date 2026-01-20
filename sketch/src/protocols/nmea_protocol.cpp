#include "nmea_protocol.h"

#include "logger.h"

// NmeaPacket implementation
NmeaPacket::NmeaPacket(char* buffer, uint16_t size) {
    parse_fields(buffer, size);
}

const char* NmeaPacket::packet_id() const {
    return field(0);
}

const char* NmeaPacket::field(uint8_t index) const {
    if (index < field_count_) {
        return fields_[index];
    }
    return nullptr;
}

void NmeaPacket::parse_fields(char* buffer, uint16_t size) {
    if (buffer == nullptr || size == 0) {
        return;
    }

    char* head = buffer;
    
    // Skip leading $ or !
    if (head[0] == '$' || head[0] == '!') {
        head++;
    }

    for (uint16_t i = 0; i < size; i++) {
        if (buffer[i] == ',' || buffer[i] == '*') {
            buffer[i] = '\0';
            fields_[field_count_++] = head;
            head = &buffer[i + 1];
            
            // Stop at checksum marker
            if (buffer[i] == '*') {
                break;
            }
        }
    }
}

// NmeaProtocol implementation

void NmeaProtocol::process_char(char c) {
    if (c == '\n' || c == '\r') {
        if (buffer_idx_ == 0) {
            LOG_WARN("NMEA: Empty message received");
            return;
        }
        buffer_[buffer_idx_] = '\0';
        if (is_checksum_valid(buffer_, buffer_idx_)) {
            if (packet_callback_) {
                NmeaPacket packet(buffer_, buffer_idx_);
                packet_callback_(&packet, callback_context_);
            }
        } else {
            LOG_ERROR("NMEA: Checksum invalid");
        }
        buffer_idx_ = 0;
    } else {
        buffer_[buffer_idx_++] = c;
        if (buffer_idx_ >= NMEA_MAX_SIZE) {
            LOG_WARN("NMEA: Buffer overflow, resetting buffer");
            buffer_idx_ = 0;
        }
    }   
}

bool NmeaProtocol::is_checksum_valid(const char* msg, uint16_t size) {
    uint8_t checksum = 0;
    uint8_t msg_checksum = 0;

    for (uint16_t i = 0; i < size; i++) {
        if (msg[i] == '*' && i + 1 < size) {
            msg_checksum = strtol(&msg[i + 1], nullptr, 16);
            break;
        } else {
            if (msg[i] == '$' || msg[i] == '!') {
                continue;
            }
            checksum ^= msg[i];
        }
    }
    return checksum == msg_checksum;
}