#pragma once

#include <Arduino.h>
#include <Wire.h>

// ── I2C Address ───────────────────────────────────────────────────────────────
// AD0 pin LOW  → 0x68
// AD0 pin HIGH → 0x69
#define MPU6500_I2C_ADDR_LOW  0x68
#define MPU6500_I2C_ADDR_HIGH 0x69

// ── Register map ─────────────────────────────────────────────────────────────
#define MPU6500_REG_SMPLRT_DIV      0x19
#define MPU6500_REG_CONFIG          0x1A
#define MPU6500_REG_GYRO_CONFIG     0x1B
#define MPU6500_REG_ACCEL_CONFIG    0x1C
#define MPU6500_REG_ACCEL_CONFIG2   0x1D
#define MPU6500_REG_INT_PIN_CFG     0x37
#define MPU6500_REG_INT_ENABLE      0x38
#define MPU6500_REG_ACCEL_XOUT_H    0x3B
#define MPU6500_REG_TEMP_OUT_H      0x41
#define MPU6500_REG_GYRO_XOUT_H     0x43
#define MPU6500_REG_USER_CTRL       0x6A
#define MPU6500_REG_PWR_MGMT_1      0x6B
#define MPU6500_REG_PWR_MGMT_2      0x6C
#define MPU6500_REG_WHO_AM_I        0x75

#define MPU6500_WHO_AM_I_VAL        0x70

// ── Config enums ─────────────────────────────────────────────────────────────
enum MPU6500_GyroRange {
    MPU6500_GYRO_RANGE_250DPS  = 0x00,
    MPU6500_GYRO_RANGE_500DPS  = 0x08,
    MPU6500_GYRO_RANGE_1000DPS = 0x10,
    MPU6500_GYRO_RANGE_2000DPS = 0x18
};

enum MPU6500_AccelRange {
    MPU6500_ACCEL_RANGE_2G  = 0x00,
    MPU6500_ACCEL_RANGE_4G  = 0x08,
    MPU6500_ACCEL_RANGE_8G  = 0x10,
    MPU6500_ACCEL_RANGE_16G = 0x18
};

enum MPU6500_DLPFBandwidth {
    MPU6500_DLPF_BW_250HZ = 0x00,
    MPU6500_DLPF_BW_184HZ = 0x01,
    MPU6500_DLPF_BW_92HZ  = 0x02,
    MPU6500_DLPF_BW_41HZ  = 0x03,
    MPU6500_DLPF_BW_20HZ  = 0x04,
    MPU6500_DLPF_BW_10HZ  = 0x05,
    MPU6500_DLPF_BW_5HZ   = 0x06
};

// ── Data struct ───────────────────────────────────────────────────────────────
struct MPU6500Data {
    float accelX, accelY, accelZ;  // g
    float gyroX,  gyroY,  gyroZ;   // °/s
    float tempC;                   // °C
};

// ── Driver class ──────────────────────────────────────────────────────────────
class MPU6500 {
public:
    // addr    – I2C address (0x68 if AD0 low, 0x69 if AD0 high)
    // i2cBus  – Wire instance (default Wire)
    MPU6500(uint8_t addr = MPU6500_I2C_ADDR_LOW, TwoWire &i2cBus = Wire)
        : _addr(addr), _wire(i2cBus) {}

    // Initialise I2C bus + sensor. Returns true on success.
    bool begin(int sda = 21, int scl = 22, uint32_t freq = 400000UL) {
        _wire.begin(sda, scl, freq);

        // Reset device
        writeReg(MPU6500_REG_PWR_MGMT_1, 0x80);
        delay(100);

        // Wake up, auto-select best clock source
        writeReg(MPU6500_REG_PWR_MGMT_1, 0x01);
        delay(10);

        // Verify WHO_AM_I
        uint8_t whoami = readReg(MPU6500_REG_WHO_AM_I);
        if (whoami != MPU6500_WHO_AM_I_VAL) {
            Serial.printf("[MPU6500] WHO_AM_I = 0x%02X (expected 0x70)\n", whoami);
            return false;
        }

        // Defaults: ±2 g, ±250 °/s, DLPF 92 Hz
        writeReg(MPU6500_REG_SMPLRT_DIV,    0x00);
        writeReg(MPU6500_REG_GYRO_CONFIG,   MPU6500_GYRO_RANGE_250DPS);
        writeReg(MPU6500_REG_ACCEL_CONFIG,  MPU6500_ACCEL_RANGE_2G);
        writeReg(MPU6500_REG_ACCEL_CONFIG2, MPU6500_DLPF_BW_92HZ);
        writeReg(MPU6500_REG_CONFIG,        MPU6500_DLPF_BW_92HZ);

        _gyroScale  = 131.0f;
        _accelScale = 16384.0f;

        return true;
    }

    void setGyroRange(MPU6500_GyroRange range) {
        writeReg(MPU6500_REG_GYRO_CONFIG, (uint8_t)range);
        switch (range) {
            case MPU6500_GYRO_RANGE_250DPS:  _gyroScale = 131.0f;  break;
            case MPU6500_GYRO_RANGE_500DPS:  _gyroScale = 65.5f;   break;
            case MPU6500_GYRO_RANGE_1000DPS: _gyroScale = 32.8f;   break;
            case MPU6500_GYRO_RANGE_2000DPS: _gyroScale = 16.4f;   break;
        }
    }

    void setAccelRange(MPU6500_AccelRange range) {
        writeReg(MPU6500_REG_ACCEL_CONFIG, (uint8_t)range);
        switch (range) {
            case MPU6500_ACCEL_RANGE_2G:  _accelScale = 16384.0f; break;
            case MPU6500_ACCEL_RANGE_4G:  _accelScale = 8192.0f;  break;
            case MPU6500_ACCEL_RANGE_8G:  _accelScale = 4096.0f;  break;
            case MPU6500_ACCEL_RANGE_16G: _accelScale = 2048.0f;  break;
        }
    }

    void setDLPFBandwidth(MPU6500_DLPFBandwidth bw) {
        writeReg(MPU6500_REG_CONFIG,        (uint8_t)bw);
        writeReg(MPU6500_REG_ACCEL_CONFIG2, (uint8_t)bw);
    }

    // Burst-read all 14 sensor bytes in one I2C transaction
    MPU6500Data read() {
        uint8_t buf[14];
        readRegs(MPU6500_REG_ACCEL_XOUT_H, buf, 14);

        int16_t ax = (int16_t)((buf[0]  << 8) | buf[1]);
        int16_t ay = (int16_t)((buf[2]  << 8) | buf[3]);
        int16_t az = (int16_t)((buf[4]  << 8) | buf[5]);
        int16_t t  = (int16_t)((buf[6]  << 8) | buf[7]);
        int16_t gx = (int16_t)((buf[8]  << 8) | buf[9]);
        int16_t gy = (int16_t)((buf[10] << 8) | buf[11]);
        int16_t gz = (int16_t)((buf[12] << 8) | buf[13]);

        MPU6500Data d;
        d.accelX = ax / _accelScale;
        d.accelY = ay / _accelScale;
        d.accelZ = az / _accelScale;
        d.gyroX  = gx / _gyroScale;
        d.gyroY  = gy / _gyroScale;
        d.gyroZ  = gz / _gyroScale;
        d.tempC  = (t / 333.87f) + 21.0f;
        return d;
    }

private:
    uint8_t   _addr;
    TwoWire  &_wire;
    float     _gyroScale;
    float     _accelScale;

    void writeReg(uint8_t reg, uint8_t val) {
        _wire.beginTransmission(_addr);
        _wire.write(reg);
        _wire.write(val);
        _wire.endTransmission();
    }

    uint8_t readReg(uint8_t reg) {
        _wire.beginTransmission(_addr);
        _wire.write(reg);
        _wire.endTransmission(false);
        _wire.requestFrom(_addr, (uint8_t)1);
        return _wire.available() ? _wire.read() : 0xFF;
    }

    void readRegs(uint8_t startReg, uint8_t *buf, uint8_t len) {
        _wire.beginTransmission(_addr);
        _wire.write(startReg);
        _wire.endTransmission(false);
        _wire.requestFrom(_addr, len);
        for (uint8_t i = 0; i < len; i++) {
            buf[i] = _wire.available() ? _wire.read() : 0x00;
        }
    }
};