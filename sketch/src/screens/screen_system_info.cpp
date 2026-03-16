#include "display/screen.h"
#include "display/screen_manager.h"
#include "display/color_scheme.h"
#include "sensor_manager.h"
#include "utils.h"

#define LOG_TAG "S-SYS"
#include "logger.h"

namespace {
    // Layout constants
    const uint16_t COL_LABEL_X   = 10;
    const uint16_t COL_VALUE_X   = 140;
    const uint16_t SECTION_START = 40;
    const uint16_t LINE_HEIGHT   = 22;
    const uint8_t  FONT_SIZE     = 2;

    const uint16_t PROBE_Y       = SECTION_START;

    const uint16_t COL_WIDTH = 120;

    const uint16_t GPS_STATS_Y   = PROBE_Y + 3 * LINE_HEIGHT + 14;
    const uint16_t GPS_STATS_X   = COL_LABEL_X;

    const uint16_t NVM_STATS_Y   = GPS_STATS_Y;
    const uint16_t NVM_STATS_X   = COL_LABEL_X + COL_WIDTH;
}

SystemInfoScreen::SystemInfoScreen()
    : Screen()
    // Header
    , title_text_("System Info", 10, 4, FONT_SIZE)
    , header_icon_(0, 0, header_icon, header_width, header_height)
    // Probe status
    , compass_label_("Compass:", COL_LABEL_X, PROBE_Y, FONT_SIZE)
    , compass_status_("---", COL_VALUE_X, PROBE_Y, FONT_SIZE)
    , gps_label_("GPS:", COL_LABEL_X, PROBE_Y + LINE_HEIGHT, FONT_SIZE)
    , gps_status_("---", COL_VALUE_X, PROBE_Y + LINE_HEIGHT, FONT_SIZE)
    , navman_label_("Navman:", COL_LABEL_X, PROBE_Y + 2 * LINE_HEIGHT, FONT_SIZE)
    , navman_status_("---", COL_VALUE_X, PROBE_Y + 2 * LINE_HEIGHT, FONT_SIZE)
    // GPS NMEA stats
    , gps_stats_label_("GPS NMEA", GPS_STATS_X, GPS_STATS_Y, FONT_SIZE)
    , gps_rx_text_("Rx:  0", GPS_STATS_X, GPS_STATS_Y + LINE_HEIGHT, FONT_SIZE)
    , gps_crc_text_("CRC: 0", GPS_STATS_X, GPS_STATS_Y + 2 * LINE_HEIGHT, FONT_SIZE)
    , gps_bof_text_("BOF: 0", GPS_STATS_X, GPS_STATS_Y + 3 * LINE_HEIGHT, FONT_SIZE)
    // Navman NMEA stats
    , navman_stats_label_("Navman NMEA", NVM_STATS_X, NVM_STATS_Y, FONT_SIZE)
    , navman_rx_text_("Rx:  0", NVM_STATS_X, NVM_STATS_Y + LINE_HEIGHT, FONT_SIZE)
    , navman_crc_text_("CRC: 0", NVM_STATS_X, NVM_STATS_Y + 2 * LINE_HEIGHT, FONT_SIZE)
    , navman_bof_text_("BOF: 0", NVM_STATS_X, NVM_STATS_Y + 3 * LINE_HEIGHT, FONT_SIZE)
{
    header_icon_.set_child(&title_text_);

    buttons_.get_button(2).set_icon(BitmapIcon(0, 0, left_arrow_icon, left_arrow_width, left_arrow_height));
    buttons_.get_button(3).set_icon(BitmapIcon(0, 0, right_arrow_icon, right_arrow_width, right_arrow_height));

    gps_ = SensorManager::get_instance().get_sensor<GPS>();
    navman_ = SensorManager::get_instance().get_sensor<Navman>();
    compass_ = SensorManager::get_instance().get_sensor<Compass>();

    // Section headers use header color
    uint16_t hdr_color = ColorScheme::get_instance().header_color();
    gps_stats_label_.set_color(hdr_color);
    navman_stats_label_.set_color(hdr_color);
}

void SystemInfoScreen::draw_screen(Display &display)
{
    uint16_t hdr_color = ColorScheme::get_instance().header_color();
    uint16_t txt_color = ColorScheme::get_instance().text_color();

    header_icon_.set_color(hdr_color);
    title_text_.set_color(txt_color);
    gps_stats_label_.set_color(txt_color);
    navman_stats_label_.set_color(txt_color);

    buttons_.draw(display);
    header_icon_.draw(display);
    title_text_.draw(display);

    // Probe status
    compass_label_.draw(display);
    compass_status_.draw(display);
    gps_label_.draw(display);
    gps_status_.draw(display);
    navman_label_.draw(display);
    navman_status_.draw(display);

    // GPS NMEA stats
    gps_stats_label_.draw(display);
    gps_rx_text_.draw(display);
    gps_crc_text_.draw(display);
    gps_bof_text_.draw(display);

    // Navman NMEA stats
    navman_stats_label_.draw(display);
    navman_rx_text_.draw(display);
    navman_crc_text_.draw(display);
    navman_bof_text_.draw(display);
}

void SystemInfoScreen::clear_screen(Display &display)
{
    title_text_.clear_content(display);
    header_icon_.clear_content(display);
    buttons_.clear_content(display);

    compass_label_.clear_content(display);
    compass_status_.clear_content(display);
    gps_label_.clear_content(display);
    gps_status_.clear_content(display);
    navman_label_.clear_content(display);
    navman_status_.clear_content(display);

    gps_stats_label_.clear_content(display);
    gps_rx_text_.clear_content(display);
    gps_crc_text_.clear_content(display);
    gps_bof_text_.clear_content(display);

    navman_stats_label_.clear_content(display);
    navman_rx_text_.clear_content(display);
    navman_crc_text_.clear_content(display);
    navman_bof_text_.clear_content(display);
}

void SystemInfoScreen::update()
{
    update_sensor_status();
    update_nmea_stats();
}

void SystemInfoScreen::update_sensor_status()
{
    uint16_t ok_color  = ColorScheme::get_instance().default_icon_color();
    uint16_t err_color = ColorScheme::get_instance().warning_color();

    if (compass_) {
        bool avail = compass_->is_available();
        compass_status_.set_text(avail ? "OK" : "N/A");
        compass_status_.set_color(avail ? ok_color : err_color);
    } else {
        compass_status_.set_text("N/A");
        compass_status_.set_color(err_color);
    }

    if (gps_) {
        bool avail = gps_->is_available();
        gps_status_.set_text(avail ? "OK" : "N/A");
        gps_status_.set_color(avail ? ok_color : err_color);
    } else {
        gps_status_.set_text("N/A");
        gps_status_.set_color(err_color);
    }

    if (navman_) {
        bool avail = navman_->is_available();
        navman_status_.set_text(avail ? "OK" : "N/A");
        navman_status_.set_color(avail ? ok_color : err_color);
    } else {
        navman_status_.set_text("N/A");
        navman_status_.set_color(err_color);
    }
}

void SystemInfoScreen::update_nmea_stats()
{
    if (gps_) {
        const NmeaStats& s = gps_->get_nmea_stats();
        gps_rx_text_.set_text("Rx:  " + String(s.received));
        gps_crc_text_.set_text("CRC: " + String(s.crc_errors));
        gps_bof_text_.set_text("BOF: " + String(s.bof_count));
    }

    if (navman_) {
        const NmeaStats& s = navman_->get_nmea_stats();
        navman_rx_text_.set_text("Rx:  " + String(s.received));
        navman_crc_text_.set_text("CRC: " + String(s.crc_errors));
        navman_bof_text_.set_text("BOF: " + String(s.bof_count));
    }
}

void SystemInfoScreen::key_press_handler(uint8_t button_index, ButtonEventType event_type)
{
    if (button_index == 2 && event_type == ButtonEventType::SHORT) 
    {
        ScreenManager::get_instance().next_screen();
    } 
    else if (button_index == 3 && event_type == ButtonEventType::SHORT) 
    {
        ScreenManager::get_instance().previous_screen();
    } 
}
