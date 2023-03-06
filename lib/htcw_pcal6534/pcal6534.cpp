#include "pcal6534.hpp"
using namespace arduino;

uint8_t pcal6534::reg(uint8_t id) {
    m_i2c.beginTransmission(m_address);
    m_i2c.write(id);
    m_i2c.endTransmission();
    if(0==m_i2c.requestFrom(m_address,(uint8_t)1)) {
        return 0;
    }
    return (uint8_t)m_i2c.read();
}
void pcal6534::reg(uint8_t id,uint8_t value) {
    m_i2c.beginTransmission(m_address);
    m_i2c.write(id);
    m_i2c.write(value);
    m_i2c.endTransmission();
}
pcal6534::pcal6534(TwoWire& i2c, uint8_t address) : m_i2c(i2c), m_address(address), m_initialized(false) {

}
bool pcal6534::initialize() {
    if(!m_initialized) {
        m_initialized = true;
        m_i2c.begin();
        reset();
    }
    return m_initialized;
}
bool pcal6534::initialized() const {
    return m_initialized;
}
void pcal6534::reset() {
    if(!m_initialized) {
        // initialize resets
        initialize();
        return;
    }
    m_i2c.beginTransmission(0);
    m_i2c.write(0x06);
    m_i2c.endTransmission();
}
void pcal6534::mode(uint8_t pin, pcal6534_mode mode) {
    pin%=34;
    const uint8_t port = pin/8;
    const uint8_t ppin = pin%8;
    const uint8_t pin_bit = 1<<ppin;
    uint8_t ra,r;
    switch(mode) {
    case pcal6534_mode::output:
            // set push pull
            ra = 0x68+port;
            r = reg(ra);
            r &= ~pin_bit;
            reg(ra,r);
            // set output
            ra = 0x0F+port;
            r=reg(ra);
            r&=~pin_bit;
            reg(ra,r);
        break;
        case pcal6534_mode::output_open_drain:
            // set open drain
            ra = 0x68+port;
            r = reg(ra);
            r |= pin_bit;
            reg(ra,r);
            // set output
            ra = 0x0F+port;
            r=reg(ra);
            r&=~pin_bit;
            reg(ra,r);
        break;
        case pcal6534_mode::input: 
            // disable resistors
            ra = 0x3F+port;
            r = reg(ra);
            r &= ~pin_bit;
            reg(ra,r);
            // set input
            ra = 0x0F+port;
            r=reg(ra);
            r |= pin_bit;
            reg(ra,r);
        break;
        case pcal6534_mode::input_pulldown:
            // set pulldown
            ra = 0x44+port;
            r = reg(ra);
            r &= ~pin_bit;
            reg(ra,r);
            // enable resistors
            ra = 0x3F+port;
            r = reg(ra);
            r |= pin_bit;
            reg(ra,r);
            // set input
            ra = 0x0F+port;
            r=reg(ra);
            r |= pin_bit;
            reg(ra,r);
        break;
        case pcal6534_mode::input_pullup:
            // set pullup
            ra = 0x44+port;
            r = reg(ra);
            r |= pin_bit;
            reg(ra,r);
            // enable resistors
            ra = 0x3F+port;
            r = reg(ra);
            r |= pin_bit;
            reg(ra,r);
            // set input
            ra = 0x0F+port;
            r=reg(ra);
            r |= pin_bit;
            reg(ra,r);
        break;   
     }
}
void pcal6534::digital_write(uint8_t pin,bool value) {
    pin%=34;
    const uint8_t port = pin/8;
    const uint8_t ppin = pin%8;
    const uint8_t pin_bit = 1<<ppin;
    uint8_t r = reg(0x05+port);
    if(value) {
        r|=pin_bit;
    } else {
        r&=~pin_bit;
    }
    reg(0x05+port,r);
}
bool pcal6534::digital_read(uint8_t pin) {
    pin%=34;
    const uint8_t port = pin/8;
    const uint8_t ppin = pin%8;
    const uint8_t pin_bit = 1<<ppin;
    uint8_t r = reg(0x00+port);
    return r & pin_bit;
}
pcal6534::accessor pcal6534::operator[](int index) {
    return accessor(*this,index);
}