#include "main.h"
#include "util/delay.h"
#include "avr/cpufunc.h"

#include "string.h" // memcpy, memcmp

/*
static uint8_t address = 0;
static uint8_t dmx_buffer[512] = {0};
static uint8_t pan_value = 0;
static uint8_t tilt_value = 0;
static uint8_t master_dimmer = 0;
static uint8_t led_colors[21] = {0};
*/

int main() {
    //_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);

    setup_io();


    for (;;) {
        PORTC.OUT = 0x04;
        _delay_ms(200);
        PORTC.OUT = 0;
        _delay_ms(200);
    }

    return 0;

}
/*
    for (;;) {

        // only update value when different from dmx
        if (!memcmp(dmx_buffer + address, &pan_value, 1)) {
            pan_value = dmx_buffer[address];

        }
        if (!memcmp(dmx_buffer + address + 1, &tilt_value, 1)) {
            tilt_value = dmx_buffer[address + 1];

        }
        if (!memcmp(dmx_buffer + address + 2, &master_dimmer, 1)) {
            master_dimmer = *(dmx_buffer + address + 2);

        }
        if (!memcmp(dmx_buffer + address + 3, &led_colors, 21)) {
            memcpy(&led_colors, dmx_buffer + 2, 3);

        }
    }

    return 0;
}*/


void setup_io() {
    // see main.h for pinout.
    PORTA.DIR &= 0x01;      // set PA[1-7] to inputs 
    PORTA.PIN0CTRL |= PORT_ISC_INPUT_DISABLE_gc;    // disable PA[0] completly
    PORTB.DIR = 0b00010111; // set PB[0-2, 4] to output PA[3] to input
    PORTC.DIR = 0b00000101; // set PC[0,2] to output, PC[3] to input
    

    // TODO: setup pwm, uart(dmx), ws2812b
}


uint8_t read_address() {
    uint8_t addr = PORTA.IN & 0xFE;
    addr |= (PORTC.IN >> 3) & 0x01;
    return addr;
}
