#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * GPS NMEA Mock Data Provider
 * 
 * Generates realistic GPS NMEA sentences for host-based testing.
 * Produces a sequence of GPRMC and GPGSV sentences that simulate
 * a moving vessel with a valid GPS fix.
 */

namespace MockedData {

/**
 * Compute NMEA XOR checksum for the content between '$' and '*'.
 * Returns the checksum byte.
 */
uint8_t nmea_checksum(const char* sentence);

/**
 * Fill the provided buffer with generated GPS NMEA data.
 * 
 * Generates `packet_count` sets of (GPRMC + GPGSV) sentences.
 * Each set simulates 1-second GPS updates with:
 *   - Valid fix (status 'A')
 *   - Position near 48.8566N, 2.3522E (Paris) with slight drift
 *   - Speed ~5.2 knots, course ~123.4 degrees
 *   - Date: 120326 (12 March 2026)
 *   - 8 satellites in view
 *   - UTC time incrementing from 120000.00
 * 
 * @param buffer       Output buffer to fill with NMEA data
 * @param buffer_size  Size of the output buffer
 * @param packet_count Number of GPRMC+GPGSV sentence pairs to generate
 * @return             Number of bytes written to buffer
 */
size_t generate_gps_nmea_data(char* buffer, size_t buffer_size, uint16_t packet_count);

} // namespace MockedData
