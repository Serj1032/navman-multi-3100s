#pragma once

#include <stdint.h>
#include <stddef.h>

// Forward-declare millis() so the header doesn't depend on Arduino.h
unsigned long millis();

/**
 * MockedSerialDataProvider
 * 
 * Provides a static data buffer that HardwareSerial can read from in a loop.
 * Data is fed chunk-by-chunk based on simulated baud rate timing, so the
 * consumer (e.g. GPS) receives bytes at a realistic pace rather than all at once.
 *
 * After the entire buffer has been delivered it wraps around and starts again.
 */
class MockedSerialDataProvider {
public:
    MockedSerialDataProvider() = default;
    ~MockedSerialDataProvider() = default;

    /**
     * Set the data buffer to feed through the serial port.
     * The provider does NOT take ownership — the buffer must remain valid
     * for the lifetime of the provider.
     * 
     * @param data  Pointer to the data buffer
     * @param size  Number of bytes in the buffer
     */
    void set_data(const char* data, size_t size) {
        data_ = data;
        size_ = size;
        read_pos_ = 0;
        avail_pos_ = 0;
        last_feed_ms_ = 0;
    }

    /**
     * Set the simulated baud rate. This controls how fast bytes become
     * available.  At 9600 baud with 10 bits per byte the throughput is
     * 960 bytes/s — i.e. roughly 1 byte per millisecond.
     *
     * @param baud  Baud rate (default 9600)
     */
    void set_baud_rate(unsigned long baud) {
        baud_rate_ = baud;
    }

    /**
     * Returns the number of bytes currently available for reading.
     * New bytes are released over time based on the configured baud rate.
     */
    int available() {
        if (!data_ || size_ == 0) {
            return 0;
        }
        advance_available();
        // Available bytes: from read_pos_ up to avail_pos_
        if (avail_pos_ >= read_pos_) {
            return (int)(avail_pos_ - read_pos_);
        }
        return 0;
    }

    /**
     * Read one byte from the buffer.
     * Only bytes that have been "received" (based on timing) can be read.
     * Returns -1 if nothing is available yet.
     */
    int read() {
        if (available() <= 0) {
            return -1;
        }
        char c = data_[read_pos_];
        read_pos_++;
        if (read_pos_ >= size_ && avail_pos_ >= size_) {
            // Both cursors reached the end — wrap around for next cycle
            read_pos_ = 0;
            avail_pos_ = 0;
            last_feed_ms_ = 0; // Reset timing so the next cycle starts fresh
        }
        return (uint8_t)c;
    }

    /**
     * Peek at the next byte without advancing the read position.
     * Returns -1 if nothing is available yet.
     */
    int peek() {
        if (available() <= 0) {
            return -1;
        }
        return (uint8_t)data_[read_pos_];
    }

    /**
     * Reset all positions to the beginning of the buffer.
     */
    void reset() {
        read_pos_ = 0;
        avail_pos_ = 0;
        last_feed_ms_ = 0;
    }

    /**
     * Check if data has been set.
     */
    bool has_data() const {
        return data_ != nullptr && size_ > 0;
    }

private:
    /**
     * Advance avail_pos_ based on elapsed time since the last feed,
     * simulating bytes arriving at the configured baud rate.
     * At 9600 baud (10 bits/byte) ≈ 960 bytes/sec ≈ ~1.04 ms/byte.
     */
    void advance_available() {
        unsigned long now = millis();

        // First call — seed the timestamp
        if (last_feed_ms_ == 0) {
            last_feed_ms_ = now;
            return;
        }

        unsigned long elapsed = now - last_feed_ms_;
        if (elapsed == 0) {
            return;
        }

        // bytes_per_second = baud / 10  (8 data + 1 start + 1 stop)
        // new_bytes = elapsed_ms * bytes_per_second / 1000
        unsigned long bytes_per_sec = baud_rate_ / 10;
        size_t new_bytes = (size_t)(elapsed * bytes_per_sec / 1000);
        if (new_bytes == 0) {
            return;
        }

        // Advance available position, clamped to buffer size
        avail_pos_ += new_bytes;
        if (avail_pos_ > size_) {
            avail_pos_ = size_;
        }

        last_feed_ms_ = now;
    }

private:
    const char* data_ = nullptr;
    size_t size_ = 0;
    size_t read_pos_ = 0;        // Next byte the consumer will read
    size_t avail_pos_ = 0;       // One past the last byte available to read
    unsigned long last_feed_ms_ = 0;  // millis() of last advance
    unsigned long baud_rate_ = 9600;
};
