#ifndef NMEA_H
#define NMEA_H


struct NmeaData {
    float log_trip_distance;   // Trip distance in nautical miles
    float log_total_distance;  // Total distance in nautical miles
    float water_temperature;   // Water temperature in Celsius
    float speed_knots;        // Speed through water in knots
    float speed_kmh;          // Speed through water in km/h
    float dbt_feet;        // Depth below transducer in feet
    float dbt_meters;      // Depth below transducer in meters
    float dbt_fathoms;     // Depth below transducer in fathoms
    float deepth_meters;    // Depth in meters
    float deepth_offset;    // Depth offset
};

class Nmea {

public:
    ~Nmea() = default;

    static Nmea& get_instance() {
        static Nmea instance;
        return instance;
    }

    void parse(const char* msg, uint16_t size) {
        if (!is_checksum_valid(msg, size)) {
            return;
        }

        if (strncmp(msg, "$VWVLW", 6) == 0) {
            parse_vlw(msg, size);
        } else if (strncmp(msg, "$VWMTW", 6) == 0) {
            parse_mtw(msg, size);
        } else if (strncmp(msg, "$VWVHW", 6) == 0) {
            parse_vhw(msg, size);
        } else if (strncmp(msg, "$SDDBT", 6) == 0) {
            parse_dbt(msg, size);
        } else if (strncmp(msg, "$SDDPT", 6) == 0) {
            parse_dpt(msg, size);
        }

    }

    const NmeaData& get_data() const {
        return nmea_data_;
    }

private:
    Nmea() = default;

    void parse_vlw(const char* msg, uint16_t size) {
        // Example: $VWVLW,0.33,N,0.33,N*4C
        float trip_distance = 0.0f;
        float total_distance = 0.0f;

        sscanf(msg, "$VWVLW,%f,N,%f,N", &trip_distance, &total_distance);

        nmea_data_.log_trip_distance = trip_distance;
        nmea_data_.log_total_distance = total_distance;
    }

    void parse_mtw(const char* msg, uint16_t size) {
        // Example: $VWMTW,0.0,C*22
        float temperature = 0.0f;

        sscanf(msg, "$VWMTW,%f,C", &temperature);

        nmea_data_.water_temperature = temperature;
    }

    void parse_vhw(const char* msg, uint16_t size) {
        // Example: $VWVHW,,,,,0.00,N,0.00,K*4D
        float speed_knots = 0.0f;
        float speed_kmh = 0.0f;

        sscanf(msg, "$VWVHW,,,,,%f,N,%f,K", &speed_knots, &speed_kmh);

        nmea_data_.speed_knots = speed_knots;
        nmea_data_.speed_kmh = speed_kmh;
    }

    void parse_dbt(const char* msg, uint16_t size) {
        // Example: $SDDBT,0.0,f,0.0,M,0.0,F*06
        float depth_feet = 0.0f;
        float depth_meters = 0.0f;
        float depth_fathoms = 0.0f;

        sscanf(msg, "$SDDBT,%f,f,%f,M,%f,F", &depth_feet, &depth_meters, &depth_fathoms);

        nmea_data_.dbt_feet = depth_feet;
        nmea_data_.dbt_meters = depth_meters;
        nmea_data_.dbt_fathoms = depth_fathoms;
    }

    void parse_dpt(const char* msg, uint16_t size) {
        // Example: $SDDPT,0.0,0.0,*7B
        float depth = 0.0f;
        float offset = 0.0f;

        sscanf(msg, "$SDDPT,%f,%f", &depth, &offset);
        nmea_data_.deepth_meters = depth;
        nmea_data_.deepth_offset = offset;
    }

    bool is_checksum_valid(const char* msg, uint16_t size) {
        uint8_t checksum = 0;
        uint8_t msg_checksum = 0;

        for (uint16_t i = 0; i < size; i++) {
            if (msg[i] == '*' && i + 1 < size) {
                msg_checksum = strtol(&msg[i + 1], nullptr, 16);
                break;
            } else {
                if (msg[i] == '$' || msg[i] == '!') {
                    continue;
                }
                checksum ^= msg[i];
            }
        }

        return checksum == msg_checksum;
    }

private:
    NmeaData nmea_data_;

};

#endif // NMEA_H


/*
$VWVLW,0.33,N,0.33,N*4C
$VWMTW,0.0,C*22 $VWVHW,,,,,0.00,N,0.00,K*4D
$SDDBT,0.0,f,0.0,M,0.0,F*06
$SDDPT,0.0,0.0,*7B
$PTTKD,0.0,,B*1F
$PTTKV,0.00,,0.00,0.33,0.33,0.0,C*0C

VLW — Distance Traveled through Water (Log Distance)
Сообщение измеряет путь судна, пройденный по воде (водомерный лаг).

Формат:
$--VLW,<Trip distance,N>,<Total distance,N>*hh

Расшифровка:

0.33,N — расстояние за текущий рейс (Trip distance) = 0.33 морских мили

0.33,N — общее расстояние (Total distance) = 0.33 морских мили

*4C — контрольная сумма

📘 Итог: Лаг показывает, что судно прошло 0.33 морских мили за рейс и всего 0.33 мили с момента установки/сброса.

🌡️ 2️⃣ $VWMTW,0.0,C*22

MTW — Mean Temperature of Water

Формат:
$--MTW,<Temperature>,C*hh

Расшифровка:

0.0,C — температура воды = 0.0 °C

*22 — контрольная сумма

📘 Итог: Температура воды по датчику — 0.0 °C (скорее всего, датчик не подключен или не откалиброван).

⚓ 3️⃣ $VWVHW,,,,,0.00,N,0.00,K*4D

VHW — Water Speed and Heading

Формат:
$--VHW,<Heading True>,T,<Heading Magnetic>,M,<Speed Knots>,N,<Speed km/h>,K

Расшифровка:

Heading True и Magnetic не указаны (пустые поля — ,,,,,)

0.00,N — скорость через воду = 0.00 узлов

0.00,K — скорость через воду = 0.00 км/ч

*4D — контрольная сумма

📘 Итог: Судно стоит на месте или датчик лаг отсутствует.

🌊 4️⃣ $SDDBT,0.0,f,0.0,M,0.0,F*06

DBT — Depth Below Transducer

Формат:
$--DBT,<Depth Feet>,f,<Depth Meters>,M,<Depth Fathoms>,F

Расшифровка:

0.0,f — глубина под датчиком = 0.0 футов

0.0,M — глубина под датчиком = 0.0 метров

0.0,F — глубина под датчиком = 0.0 саженей

📘 Итог: Эхолот не получает данных (0.0 — нет дна или ошибка).

🌊 5️⃣ $SDDPT,0.0,0.0,*7B

DPT — Depth

Формат:
$--DPT,<Depth>,<Offset>

Расшифровка:

0.0 — глубина от датчика до дна = 0.0 м

0.0 — смещение датчика (например, до ватерлинии) = 0.0 м

📘 Итог: Эхолот показывает 0.0 м (нет сигнала).

⚙️ 6️⃣ $PTTKD,0.0,,B*1F

PTTKD — собственное (проприетарное) предложение от производителя (PTTK)
Начинается с P, значит это proprietary sentence — не стандарт NMEA, а специфическое сообщение, например от прибора TTK (может быть TTTech, T-Tek или TTK Systems).

Структура не стандартизирована.
По значениям можно предположить:

0.0 — возможно скорость или угол

B — статус (например, режим работы или тип данных)

📘 Итог: Проприетарное сообщение производителя TTK, значение без документации не интерпретируется точно.

⚙️ 7️⃣ $PTTKV,0.00,,0.00,0.33,0.33,0.0,C*0C

Также проприетарное сообщение (PTTKV).
Поля похожи на измерения скорости и пути.

Вероятная структура (по аналогии с VLW/VHW):

0.00 — скорость (узлы или м/с)

второе пустое — возможно угол

0.00 — скорость относительно воды

0.33 — путь за рейс (Trip)

0.33 — путь общий (Total)

0.0 — возможно температура или другое значение

C — единицы измерения (вероятно °C или тип канала)

📘 Итог: Похоже на проприетарное расширение данных лога — аналог VLW/VHW, но в формате производителя TTK.


*/