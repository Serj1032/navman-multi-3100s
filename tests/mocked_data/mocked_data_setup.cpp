#include "mocked_data_setup.h"
#include "gps_nmea_data.h"
#include "navman_nmea_data.h"

#include <HardwareSerial.h>
#include <MockedSerialDataProvider.h>

// GPS NMEA data buffer — large enough for ~50 sentence pairs
static char gps_nmea_buffer[8192];
static MockedSerialDataProvider gps_data_provider;

// Navman NMEA data buffer — 7 sentences per set, ~50 sets
static char navman_nmea_buffer[16384];
static MockedSerialDataProvider navman_data_provider;

namespace MockedData {

void setup() {
    // Generate GPS NMEA data (50 GPRMC+GPGSV pairs ≈ 50 seconds of GPS data)
    size_t gps_data_size = generate_gps_nmea_data(gps_nmea_buffer, sizeof(gps_nmea_buffer), 50);

    // Attach the GPS data provider to Serial2 (GPS_SERIAL)
    // Feed at 9600 baud ≈ 960 bytes/sec, matching GPS_BAUDRATE
    gps_data_provider.set_data(gps_nmea_buffer, gps_data_size);
    gps_data_provider.set_baud_rate(9600);
    Serial2.set_data_provider(&gps_data_provider);

    // Generate Navman NMEA data (50 full sentence sets)
    size_t navman_data_size = generate_navman_nmea_data(navman_nmea_buffer, sizeof(navman_nmea_buffer), 50);

    // Attach the Navman data provider to Serial3 (NAVMAN_SERIAL)
    navman_data_provider.set_data(navman_nmea_buffer, navman_data_size);
    navman_data_provider.set_baud_rate(9600);
    Serial3.set_data_provider(&navman_data_provider);
}

} // namespace MockedData
