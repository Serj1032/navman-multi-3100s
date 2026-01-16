#pragma once

#include "display/screens/base_screen.h"
#include "display/text.h"
#include "display/icons/bitmap_icon.h"
#include "display/buttons.h"
#include "display/widgets/heading_widget.h"
#include "display/widgets/clock_widget.h"
#include "display/widgets/metrics_widget.h"
#include "display/widgets/satellite_widget.h"
#include "display/assets/bulb.h"
#include "display/assets/header.h"
#include "display/assets/left_arrow.h"
#include "display/assets/right_arrow.h"
#include "display/assets/settings.h"

class DashboardScreen : public Screen
{
public:
    DashboardScreen();
    ~DashboardScreen() override = default;

    void update() override;
    
protected:
    void draw_screen(Display &display) override;
    void clear_screen(Display &display) override;
    void key_press_handler(uint8_t button_index, ButtonEventType event_type) override;

private:
    Buttons buttons_;
    Text title_text_;
    BitmapIcon header_icon_;
    HeadingWidget heading_widget_;
    ClockWidget clock_widget_;
    MetricsWidget metrics_widget_;
    SatelliteWidget satellite_widget_;
};
