#pragma once

// enum class SensorType : uint8_t {
//     NONE = 0,
//     GPS,
//     COMPASS,
// };

class SensorManager {
public:
    static SensorManager& get_instance() {
        static SensorManager instance;
        return instance;
    }

    void initialize_sensors();

    void process_sensors();

private: 
    SensorManager() = default;
    ~SensorManager() = default;
};