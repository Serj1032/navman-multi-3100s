#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * Navman NMEA Mock Data Provider
 *
 * Generates realistic Navman instrument NMEA sentences for host-based testing.
 * Produces sets of: VWVLW, VWMTW, VWVHW, SDDBT, SDDPT, PTTKD, PTTKV
 * with slightly varying values per packet to simulate live sensor data.
 *
 * @param buffer       Output buffer to fill with NMEA data
 * @param buffer_size  Size of the output buffer
 * @param packet_count Number of full sentence sets to generate
 * @return             Number of bytes written to buffer
 */

namespace MockedData {

size_t generate_navman_nmea_data(char* buffer, size_t buffer_size, uint16_t packet_count);

} // namespace MockedData
