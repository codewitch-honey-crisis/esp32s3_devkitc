#pragma once
#include <Arduino.h>
#include <Wire.h>
namespace arduino {
enum struct pcal6534_mode {
    output = 0,
    output_open_drain = 1,
    input = 2,
    input_pulldown = 3,
    input_pullup = 4
};
class pcal6534 final {
    TwoWire& m_i2c;
    uint8_t m_address;
    bool m_initialized;
    class accessor {
        pcal6534& m_parent;
        uint8_t m_pin;
        public:
            accessor(pcal6534& parent, uint8_t pin) : m_parent(parent), m_pin(pin) {

            }
            inline operator bool() {
                return m_parent.digital_read(m_pin);
            }
            inline accessor& operator=(bool rhs) {
                m_parent.digital_write(m_pin,rhs);
                return *this;
            }
    };
    friend accessor;
    uint8_t reg(uint8_t id);
    void reg(uint8_t id,uint8_t value);
    void digital_write(uint8_t pin,bool value);
    bool digital_read(uint8_t pin);
public:
    pcal6534(TwoWire& i2c = Wire, uint8_t address = 0x20);
    bool initialize();
    bool initialized() const;
    void reset();
    void mode(uint8_t pin, pcal6534_mode mode);
    accessor operator[](int index);
    
};
}