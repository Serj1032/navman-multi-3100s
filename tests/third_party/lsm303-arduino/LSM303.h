#ifndef LSM303_h
#define LSM303_h

#include <Arduino.h>

// Mock LSM303 sensor for host testing
class LSM303
{
  public:
    template <typename T> struct vector
    {
      T x, y, z;
    };

    enum deviceType { device_DLH, device_DLM, device_DLHC, device_D, device_auto };
    enum sa0State { sa0_low, sa0_high, sa0_auto };

    // register addresses (subset for mock)
    enum regAddr
    {
      CTRL_REG1_A       = 0x20,
      CTRL_REG4_A       = 0x23,
      OUT_X_L_A         = 0x28,
      OUT_X_H_A         = 0x29,
      OUT_Y_L_A         = 0x2A,
      OUT_Y_H_A         = 0x2B,
      OUT_Z_L_A         = 0x2C,
      OUT_Z_H_A         = 0x2D,
      CRA_REG_M         = 0x00,
      CRB_REG_M         = 0x01,
      MR_REG_M          = 0x02,
    };

    vector<int16_t> a; // accelerometer readings
    vector<int16_t> m; // magnetometer readings
    vector<int16_t> m_max; // maximum magnetometer values, used for calibration
    vector<int16_t> m_min; // minimum magnetometer values, used for calibration

    byte last_status; // status of last I2C transmission

    LSM303(void);

    bool init(deviceType device = device_auto, sa0State sa0 = sa0_auto);
    deviceType getDeviceType(void) { return _device; }

    void enableDefault(void);

    void writeAccReg(byte reg, byte value);
    byte readAccReg(byte reg);
    void writeMagReg(byte reg, byte value);
    byte readMagReg(int reg);

    void writeReg(byte reg, byte value);
    byte readReg(int reg);

    void readAcc(void);
    void readMag(void);
    void read(void);

    void setTimeout(unsigned int timeout);
    unsigned int getTimeout(void);
    bool timeoutOccurred(void);

    float heading(void);
    template <typename T> float heading(vector<T> from);

    // vector functions
    template <typename Ta, typename Tb, typename To> static void vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out);
    template <typename Ta, typename Tb> static float vector_dot(const vector<Ta> *a, const vector<Tb> *b);
    static void vector_normalize(vector<float> *a);

  private:
    deviceType _device; // chip type
    byte acc_address;
    byte mag_address;

    unsigned int io_timeout;
    bool did_timeout;

    int testAddrReg(byte address, regAddr reg);
    int16_t testReg(byte address, regAddr reg);
};

// Template implementations
template <typename T> float LSM303::heading(vector<T> from)
{
    // Mock implementation - returns 0
    (void)from;
    return 0.0f;
}

template <typename Ta, typename Tb, typename To> void LSM303::vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out)
{
    out->x = (a->y * b->z) - (a->z * b->y);
    out->y = (a->z * b->x) - (a->x * b->z);
    out->z = (a->x * b->y) - (a->y * b->x);
}

template <typename Ta, typename Tb> float LSM303::vector_dot(const vector<Ta> *a, const vector<Tb> *b)
{
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

#endif
