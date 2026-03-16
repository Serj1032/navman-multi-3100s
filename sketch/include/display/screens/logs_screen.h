#pragma once

#include "display/screens/base_screen.h"
#include "display/text.h"
#include "display/icons/bitmap_icon.h"
#include "display/buttons.h"
#include "display/assets/header.h"
#include "display/assets/left_arrow.h"
#include "display/assets/right_arrow.h"

#define LOGS_MAX_LINES 28
#define LOGS_FONT_SIZE 1
#define LOGS_LINE_HEIGHT 9
#define LOGS_START_Y 32

class GPS;
class Navman;

enum class LogMode : uint8_t {
    GPS_NMEA = 0,
    NAVMAN_NMEA,
    SYSTEM_LOG,
    LOG_MODE_COUNT
};

class LogsScreen : public Screen
{
public:
    LogsScreen();
    ~LogsScreen() override;

    void update() override;

protected:
    void draw_screen(Display &display) override;
    void clear_screen(Display &display) override;
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override;

private:
    void switch_mode(LogMode mode);
    void detach_callbacks();
    void attach_callbacks();
    void push_line(const char* line);
    void clear_lines();
    void update_buttons();

    static void on_gps_raw_line(const char* line, void* ctx);
    static void on_navman_raw_line(const char* line, void* ctx);
    static void on_system_log(const char* message, void* ctx);

    // Header
    Text title_text_;
    Buttons buttons_;

    // Log lines (fixed positions, content shifts up)
    Text lines_[LOGS_MAX_LINES];

    // Mode
    LogMode mode_{LogMode::GPS_NMEA};
    bool paused_{false};

    // Sensor pointers
    GPS* gps_ = nullptr;
    Navman* navman_ = nullptr;
};
