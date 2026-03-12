#pragma once

/**
 * Mocked Data Setup
 * 
 * Initializes mocked data providers for host-based testing.
 * Call setup_mocked_data() early in setup() to wire up
 * data providers to the appropriate serial ports.
 */

namespace MockedData {

/**
 * Initialize all mocked data providers.
 * - Generates GPS NMEA data and attaches it to Serial2 (GPS_SERIAL)
 */
void setup();

} // namespace MockedData
