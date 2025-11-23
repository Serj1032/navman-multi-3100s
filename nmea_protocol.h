#pragma once

namespace {
    static const uint16_t NMEA_MAX_SIZE = 256; 
};

typedef void (*nmea_word_parser_t)(uint8_t field_idx, char* word, void* context);


class NmeaProtocol {
public:
    NmeaProtocol() = default;
    virtual ~NmeaProtocol() = default;

    virtual void process() {
        while (read_serial(&buffer_[buffer_idx_])) {
            char c = buffer_[buffer_idx_];
            if (c == '\n' || c == '\r') {
                if (buffer_idx_ == 0) {
                    continue;
                }
                buffer_[buffer_idx_] = '\0';
                if (is_checksum_valid(buffer_, buffer_idx_)) {
                    parse(buffer_, buffer_idx_);
                }
                buffer_idx_ = 0;
            } else {
                buffer_idx_++;
                if (buffer_idx_ >= NMEA_MAX_SIZE) {
                    buffer_idx_ = 0;
                }
            }
        }   
    }

protected:
    virtual bool read_serial(char* c) = 0;
    virtual void parse(char* msg, uint16_t size) = 0;

    bool is_checksum_valid(const char* msg, uint16_t size) {
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

    void parse_nmea_words(char* msg, uint16_t size, nmea_word_parser_t parser, void* context) {
        if (parser == nullptr || msg == nullptr || size == 0) {
            return;
        }
        
        char* head = msg;
        uint8_t field_idx = 0;

        for (uint16_t i = 0; i < size; i++) {
            if (msg[i] == ',' || msg[i] == '*') {
                msg[i] = '\0';
                if (field_idx == 0 && head[0] == '$') {
                    // Skip message ID
                } else {
                    if (*head != '\0') {
                        parser(field_idx, head, context);
                    }
                }
                field_idx++;
                head = &msg[i + 1];
            }
        }
    }

private:
    char buffer_[NMEA_MAX_SIZE];
    uint16_t buffer_idx_ = 0;
};