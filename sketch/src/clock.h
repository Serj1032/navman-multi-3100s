#pragma once

#include "sensor_manager.h"
#include "display/color_scheme.h"

class Time {
public:
    Time(uint32_t utc_time) : 
        hours_((utc_time / 10000) % 100),
        minutes_((utc_time / 100) % 100),
        seconds_(utc_time % 100)
    {}

    void set_time(uint32_t utc_time) {
        hours_ = (utc_time / 10000) % 100;
        minutes_ = (utc_time / 100) % 100;
        seconds_ = utc_time % 100;
    }

    uint32_t time(int8_t tz) const {
        int8_t adjusted_hours = (hours_ + tz) % 24;
        if (adjusted_hours < 0) {
            adjusted_hours += 24;
        }
        return (uint32_t)adjusted_hours * 10000 + minutes_ * 100 + seconds_;
    }

    uint8_t get_hours(int8_t tz) const {
        int8_t adjusted_hours = (hours_ + tz) % 24;
        if (adjusted_hours < 0) {
            adjusted_hours += 24;
        }
        return adjusted_hours;
    }

    uint8_t get_minutes() const {
        return minutes_;
    }

    uint8_t get_seconds() const {
        return seconds_;
    }

private:
    int8_t hours_;
    int8_t minutes_;
    int8_t seconds_;
};

class Clock {
public: 
    static Clock& get_instance() {
        static Clock instance;
        return instance;
    }

    void init(int8_t time_zone = 0) {
        time_zone_ = time_zone;
        gps_ = SensorManager::get_instance().get_sensor<GPS>();
    }

    void set_time_zone(int8_t tz) {
        time_zone_ = tz;
    }

    int8_t get_time_zone() const {
        return time_zone_;
    }

    void process() {
        if (!gps_) {
            return;
        }
        const GpsSolution* sol = gps_->get_solution();
        time_.set_time(sol->utc_time_);

        uint8_t hr = time_.get_hours(time_zone_);
        uint8_t min = time_.get_minutes();

        // TODO: Add any additional processing based on time of day
        if (hr > 7 && hr < 16) {
            ColorScheme::get_instance().set_day_mode(true);
        } else {
            ColorScheme::get_instance().set_day_mode(false);
        }
        
    }

    uint32_t get_local_time() const {
        return time_.time(time_zone_);
    }

private:
    Clock() :  time_(0), time_zone_(0) {}

private:
    GPS* gps_ = nullptr;

    Time time_;
    int8_t time_zone_;
};