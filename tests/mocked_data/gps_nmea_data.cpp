#include "gps_nmea_data.h"

#include <stdio.h>
#include <string.h>

namespace MockedData {

uint8_t nmea_checksum(const char* sentence) {
    uint8_t checksum = 0;
    // Skip the leading '$'
    if (*sentence == '$') {
        sentence++;
    }
    while (*sentence && *sentence != '*') {
        checksum ^= (uint8_t)*sentence;
        sentence++;
    }
    return checksum;
}

/**
 * Append a complete NMEA sentence (with checksum and CRLF) to buffer.
 * `body` should be the content between '$' and '*' (e.g., "GPRMC,120000.00,A,...").
 * Returns number of bytes written, or 0 if buffer is full.
 */
static size_t append_nmea_sentence(char* buffer, size_t buffer_size, size_t offset, const char* body) {
    // Format: $<body>*<XX>\r\n
    char sentence[256];
    int len = snprintf(sentence, sizeof(sentence), "$%s*", body);
    if (len < 0 || (size_t)len >= sizeof(sentence)) {
        return 0;
    }

    // Compute checksum over 'body' (between $ and *)
    uint8_t cs = nmea_checksum(sentence);

    // Append checksum hex and CRLF
    len = snprintf(sentence, sizeof(sentence), "$%s*%02X\r\n", body, cs);
    if (len < 0 || (size_t)len >= sizeof(sentence)) {
        return 0;
    }

    size_t slen = (size_t)len;
    if (offset + slen > buffer_size) {
        return 0; // Not enough space
    }

    memcpy(buffer + offset, sentence, slen);
    return slen;
}

size_t generate_gps_nmea_data(char* buffer, size_t buffer_size, uint16_t packet_count) {
    size_t offset = 0;

    // Base position: ~48.8566N, 2.3522E (Paris)
    // NMEA latitude format: DDMM.MMMM  -> 48°51.396' = 4851.3960
    // NMEA longitude format: DDDMM.MMMM -> 002°21.132' = 00221.1320
    // Base UTC time: 12:00:00.00
    // Date: 120326 (12 March 2026)

    for (uint16_t i = 0; i < packet_count; i++) {
        // Increment time by 1 second per packet
        uint32_t seconds = i;
        uint32_t hh = 12 + (seconds / 3600);
        uint32_t mm = (seconds % 3600) / 60;
        uint32_t ss = seconds % 60;

        // Small position drift: ~0.0001 per packet
        float lat_offset = i * 0.0001f;
        float lon_offset = i * 0.0002f;

        // Speed varies slightly: 5.0 - 5.5 knots
        float speed = 5.0f + (i % 6) * 0.1f;

        // Course varies slightly: 120 - 130 degrees
        float course = 120.0f + (i % 11);

        // GPRMC sentence
        // $GPRMC,HHMMSS.SS,A,DDMM.MMMM,N,DDDMM.MMMM,E,speed,course,DDMMYY,magvar,E,A*XX
        char body[200];
        snprintf(body, sizeof(body),
            "GPRMC,%02u%02u%02u.00,A,4851.%.04u,N,00221.%.04u,E,%.1f,%.1f,120326,003.1,W,A",
            (unsigned)(hh % 24), (unsigned)mm, (unsigned)ss,
            (unsigned)(3960 + (unsigned)(lat_offset * 10000)),
            (unsigned)(1320 + (unsigned)(lon_offset * 10000)),
            (double)speed,
            (double)course
        );

        size_t written = append_nmea_sentence(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;

        // GPGSV sentence (satellites in view)
        // $GPGSV,1,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*XX
        snprintf(body, sizeof(body),
            "GPGSV,1,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45"
        );

        written = append_nmea_sentence(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;
    }

    return offset;
}

} // namespace MockedData
