#pragma once

#include "display/screens/base_screen.h"
#include "display/text.h"
#include "display/icons/bitmap_icon.h"
#include "display/buttons.h"
#include "display/assets/header.h"
#include "display/assets/left_arrow.h"
#include "display/assets/right_arrow.h"

class GPS;
class Navman;
class Compass;

class SystemInfoScreen : public Screen
{
public:
    SystemInfoScreen();
    ~SystemInfoScreen() override = default;

    void update() override;

protected:
    void draw_screen(Display &display) override;
    void clear_screen(Display &display) override;
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override;

private:
    void update_sensor_status();
    void update_nmea_stats();

    // Header
    Text title_text_;
    BitmapIcon header_icon_;
    Buttons buttons_;

    // Probe status labels
    Text compass_label_;
    Text compass_status_;
    Text gps_label_;
    Text gps_status_;
    Text navman_label_;
    Text navman_status_;

    // GPS NMEA stats
    Text gps_stats_label_;
    Text gps_rx_text_;
    Text gps_crc_text_;
    Text gps_bof_text_;

    // Navman NMEA stats
    Text navman_stats_label_;
    Text navman_rx_text_;
    Text navman_crc_text_;
    Text navman_bof_text_;

    // Sensor pointers
    GPS* gps_ = nullptr;
    Navman* navman_ = nullptr;
    Compass* compass_ = nullptr;
};
