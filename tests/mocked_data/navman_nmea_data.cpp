#include "navman_nmea_data.h"
#include "gps_nmea_data.h"   // reuse nmea_checksum() and append helper

#include <stdio.h>
#include <string.h>

namespace MockedData {

/**
 * Append a complete NMEA sentence (with checksum and CRLF) to buffer.
 * Reuses the same logic as gps_nmea_data.cpp.
 */
static size_t append_nmea(char* buffer, size_t buffer_size, size_t offset, const char* body) {
    char sentence[256];
    int len = snprintf(sentence, sizeof(sentence), "$%s*", body);
    if (len < 0 || (size_t)len >= sizeof(sentence)) {
        return 0;
    }

    uint8_t cs = nmea_checksum(sentence);

    len = snprintf(sentence, sizeof(sentence), "$%s*%02X\r\n", body, cs);
    if (len < 0 || (size_t)len >= sizeof(sentence)) {
        return 0;
    }

    size_t slen = (size_t)len;
    if (offset + slen > buffer_size) {
        return 0;
    }

    memcpy(buffer + offset, sentence, slen);
    return slen;
}

size_t generate_navman_nmea_data(char* buffer, size_t buffer_size, uint16_t packet_count) {
    size_t offset = 0;

    for (uint16_t i = 0; i < packet_count; i++) {
        char body[200];
        size_t written;

        // Varying values — small increments to simulate live data
        float distance_total  = 0.33f + i * 0.02f;
        float distance_reset  = 0.33f + i * 0.02f;
        float water_temp      = 14.5f + (i % 10) * 0.1f;      // 14.5 – 15.4 °C
        float speed_knots     = 3.0f + (i % 8) * 0.25f;        // 3.0 – 4.75 kn
        float speed_kmh       = speed_knots * 1.852f;
        float heading_true    = 245.0f + (i % 15);              // 245 – 259°
        float heading_mag     = heading_true + 2.5f;
        float depth_m         = 8.5f + (i % 12) * 0.3f;        // 8.5 – 11.8 m
        float depth_ft        = depth_m * 3.28084f;
        float depth_fathoms   = depth_m * 0.546807f;
        float depth_offset    = 0.5f;

        // $VWVLW,<total>,N,<since_reset>,N
        snprintf(body, sizeof(body), "VWVLW,%.2f,N,%.2f,N",
                 (double)distance_total, (double)distance_reset);
        written = append_nmea(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;

        // $VWMTW,<temp>,C
        snprintf(body, sizeof(body), "VWMTW,%.1f,C",
                 (double)water_temp);
        written = append_nmea(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;

        // $VWVHW,<hdg_true>,T,<hdg_mag>,M,<spd_kn>,N,<spd_kmh>,K
        snprintf(body, sizeof(body), "VWVHW,%.1f,T,%.1f,M,%.2f,N,%.2f,K",
                 (double)heading_true, (double)heading_mag,
                 (double)speed_knots, (double)speed_kmh);
        written = append_nmea(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;

        // $SDDBT,<feet>,f,<meters>,M,<fathoms>,F
        snprintf(body, sizeof(body), "SDDBT,%.1f,f,%.1f,M,%.1f,F",
                 (double)depth_ft, (double)depth_m, (double)depth_fathoms);
        written = append_nmea(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;

        // $SDDPT,<depth>,<offset>,
        snprintf(body, sizeof(body), "SDDPT,%.1f,%.1f,",
                 (double)depth_m, (double)depth_offset);
        written = append_nmea(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;

        // $PTTKD,<depth>,,B
        snprintf(body, sizeof(body), "PTTKD,%.1f,,B",
                 (double)depth_m);
        written = append_nmea(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;

        // $PTTKV,<spd_kn>,,<spd_kmh>,<total>,<reset>,<temp>,C
        snprintf(body, sizeof(body), "PTTKV,%.2f,,%.2f,%.2f,%.2f,%.1f,C",
                 (double)speed_knots, (double)speed_kmh,
                 (double)distance_total, (double)distance_reset,
                 (double)water_temp);
        written = append_nmea(buffer, buffer_size, offset, body);
        if (written == 0) break;
        offset += written;
    }

    return offset;
}

} // namespace MockedData
