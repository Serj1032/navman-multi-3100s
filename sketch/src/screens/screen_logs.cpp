#include "display/screen.h"
#include "display/screen_manager.h"
#include "display/color_scheme.h"
#include "sensor_manager.h"

#define LOG_TAG "S_LOG"
#include "logger.h"

namespace {
    const char* MODE_TITLES[] = {
        "Logs: GPS NMEA",
        "Logs: Navman NMEA",
        "Logs: System"
    };

    // Short label for button 0 showing the NEXT mode to switch to
    const char* NEXT_MODE_LABELS[] = {
        "Navman",  // current GPS  → next is Navman
        "System",  // current Navman → next is System
        "GPS"      // current System → next is GPS
    };
}

LogsScreen::LogsScreen()
    : Screen()
    , title_text_(MODE_TITLES[0], 10, 4, 2)
{

    buttons_.get_button(2).set_icon(BitmapIcon(0, 0, left_arrow_icon, left_arrow_width, left_arrow_height));
    buttons_.get_button(3).set_icon(BitmapIcon(0, 0, right_arrow_icon, right_arrow_width, right_arrow_height));
    update_buttons();

    // Initialize line Text objects at their Y positions
    for (uint8_t i = 0; i < LOGS_MAX_LINES; i++) {
        lines_[i] = Text("", 2, LOGS_START_Y + i * LOGS_LINE_HEIGHT, LOGS_FONT_SIZE);
    }

    gps_ = SensorManager::get_instance().get_sensor<GPS>();
    navman_ = SensorManager::get_instance().get_sensor<Navman>();

    attach_callbacks();
}

LogsScreen::~LogsScreen()
{
    detach_callbacks();
}

void LogsScreen::draw_screen(Display &display)
{
    title_text_.set_color(ColorScheme::get_instance().text_color());
    buttons_.draw(display);

    title_text_.draw(display);

    for (uint8_t i = 0; i < LOGS_MAX_LINES; i++) {
        lines_[i].draw(display);
    }
}

void LogsScreen::clear_screen(Display &display)
{
    title_text_.clear_content(display);
    buttons_.clear_content(display);

    for (uint8_t i = 0; i < LOGS_MAX_LINES; i++) {
        lines_[i].clear_content(display);
    }
}

void LogsScreen::update()
{
    // Lines are updated via callbacks — nothing to poll
}

void LogsScreen::key_press_handler(uint8_t button_index, ButtonEventType event_type)
{
    if (event_type != ButtonEventType::SHORT) {
        return;
    }

    if (button_index == 0) {
        uint8_t next = (static_cast<uint8_t>(mode_) + 1) % static_cast<uint8_t>(LogMode::LOG_MODE_COUNT);
        switch_mode(static_cast<LogMode>(next));
    }
    else if (button_index == 1) {
        paused_ = !paused_;
        update_buttons();
    }
    else if (button_index == 2) {
        ScreenManager::get_instance().next_screen();
    }
    else if (button_index == 3) {
        ScreenManager::get_instance().previous_screen();
    }
}

void LogsScreen::switch_mode(LogMode mode)
{
    detach_callbacks();
    mode_ = mode;

    const char* mode_title = MODE_TITLES[static_cast<uint8_t>(mode_)];
    title_text_.set_text(mode_title);

    paused_ = false;
    update_buttons();

    clear_lines();
    attach_callbacks();
}

void LogsScreen::detach_callbacks()
{
    switch (mode_) {
        case LogMode::GPS_NMEA:
            if (gps_) gps_->get_nmea_protocol().set_raw_line_callback(nullptr);
            break;
        case LogMode::NAVMAN_NMEA:
            if (navman_) navman_->get_nmea_protocol().set_raw_line_callback(nullptr);
            break;
        case LogMode::SYSTEM_LOG:
#ifdef __DEBUG__
            Logger::get_instance().set_listener(nullptr);
#endif
            break;
        default:
            break;
    }
}

void LogsScreen::attach_callbacks()
{
    switch (mode_) {
        case LogMode::GPS_NMEA:
            if (gps_) gps_->get_nmea_protocol().set_raw_line_callback(on_gps_raw_line, this);
            break;
        case LogMode::NAVMAN_NMEA:
            if (navman_) navman_->get_nmea_protocol().set_raw_line_callback(on_navman_raw_line, this);
            break;
        case LogMode::SYSTEM_LOG:
#ifdef __DEBUG__
            Logger::get_instance().set_listener(on_system_log, this);
#endif
            break;
        default:
            break;
    }
}

void LogsScreen::update_buttons()
{
    buttons_.get_button(0).set_label(Text(NEXT_MODE_LABELS[static_cast<uint8_t>(mode_)], 0, 0, 2));
    buttons_.get_button(1).set_label(Text(paused_ ? "Play" : "Pause", 0, 0, 2));
}

void LogsScreen::push_line(const char* line)
{
    if (paused_) return;

    // Write new line at current position
    lines_[write_idx_].set_text(line);

    // Advance and erase the next line to act as a visual cursor gap
    write_idx_ = (write_idx_ + 1) % LOGS_MAX_LINES;
    lines_[write_idx_].set_text("");
}

void LogsScreen::clear_lines()
{
    for (uint8_t i = 0; i < LOGS_MAX_LINES; i++) {
        lines_[i].set_text("");
    }
    write_idx_ = 0;
}

void LogsScreen::on_gps_raw_line(const char* line, void* ctx)
{
    static_cast<LogsScreen*>(ctx)->push_line(line);
}

void LogsScreen::on_navman_raw_line(const char* line, void* ctx)
{
    static_cast<LogsScreen*>(ctx)->push_line(line);
}

void LogsScreen::on_system_log(const char* message, void* ctx)
{
    static_cast<LogsScreen*>(ctx)->push_line(message);
}
