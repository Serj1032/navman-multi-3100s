#include "nmea_protocol.h"
#include <ctype.h>

#define LOG_TAG "NMEA"
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
            
            // Check bounds before adding field
            if (field_count_ < NMEA_MAX_FIELD_AMOUNT) {
                fields_[field_count_++] = head;
            }
            
            // Bounds check for next field pointer
            if (i + 1 < size) {
                head = &buffer[i + 1];
            }
            
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
            return;
        }
        buffer_[buffer_idx_] = '\0';
        LOG_INFO("Rx: " + String(buffer_));
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
        // Check bounds BEFORE writing to prevent buffer overflow
        if (buffer_idx_ < NMEA_MAX_SIZE - 1) {
            buffer_[buffer_idx_++] = c;
        } else {
            LOG_WARN("NMEA: Buffer overflow, resetting buffer");
            buffer_idx_ = 0;
        }
    }   
}

bool NmeaProtocol::is_checksum_valid(const char* msg, uint16_t size) {
    if (msg == nullptr || size == 0) {
        return false;
    }
    
    uint8_t checksum = 0;
    uint8_t msg_checksum = 0;
    bool found_checksum = false;

    for (uint16_t i = 0; i < size; i++) {
        if (msg[i] == '*') {
            // Need at least 2 hex digits after '*'
            if (i + 2 >= size) {
                return false;
            }
            
            // Validate both characters are valid hex digits
            char c1 = msg[i + 1];
            char c2 = msg[i + 2];
            
            if (!isxdigit(c1) || !isxdigit(c2)) {
                return false;
            }
            
            // Parse checksum with error checking
            char* endptr = nullptr;
            long result = strtol(&msg[i + 1], &endptr, 16);
            
            // Verify strtol parsed exactly 2 characters and result is valid byte
            if (endptr == nullptr || endptr != &msg[i + 3] || result < 0 || result > 0xFF) {
                return false;
            }
            
            msg_checksum = (uint8_t)result;
            found_checksum = true;
            break;
        } else if (msg[i] == '\0') {
            // Premature null terminator
            break;
        } else {
            // Skip $ or ! at start
            if (msg[i] == '$' || msg[i] == '!') {
                continue;
            }
            checksum ^= msg[i];
        }
    }
    
    // Must have found a valid checksum
    return found_checksum && (checksum == msg_checksum);
}