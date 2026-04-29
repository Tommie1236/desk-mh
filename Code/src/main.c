#include "main.h"
#include "util/delay.h"
#include "stdbool.h"
#include "avr/cpufunc.h"

#include "string.h" // memcpy, memcmp
#include <avr/io.h>


static uint8_t address = 0;
static uint8_t dmx_buffer[512] = {0};
static uint8_t pan_value = 0;
static uint8_t tilt_value = 0;
static uint8_t master_dimmer = 0;
#ifdef MH_SPOT
static color_t led_color = (color_t){0, 0, 0};
#endif // MH_SPOT
#ifdef MH_WASH
static color_t led_colors[7] = {(color_t){0, 0, 0}};
#endif // MH_WASH


int main() {
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);

    status_t status;
    setup_io();

    /*

    for (;;) {
        PORTC.OUT = 0x04;
        _delay_ms(500);
        PORTC.OUT = 0;
        _delay_ms(500);
    }

    */

    for (;;) {
        address = read_address();

        // only update value when different from dmx
        if (*(dmx_buffer + address + PATCH_PAN_OFFSET) != pan_value) {
            pan_value = dmx_buffer[address];
            status.position_updated = 1;
        }
        if (*(dmx_buffer + address + PATCH_TILT_OFFSET) != tilt_value) {
            tilt_value = dmx_buffer[address + 1];
            status.position_updated = 1;
        }
        if (*(dmx_buffer + address + PATCH_MASTER_OFFSET) != master_dimmer) {
            master_dimmer = *(dmx_buffer + address + PATCH_MASTER_OFFSET);
            status.led_updated = 1;
        }
#ifdef MH_SPOT
        if (!memcmp(dmx_buffer + address + PATCH_COLOR_OFFSET, &led_color, 3)) {
            memcpy(&led_colors, dmx_buffer + address + PATCH_COLOR_OFFSET, 3);
            status.led_updated = 1;
        }
#endif // MH_SPOT
#ifdef MH_WASH
        if (!memcmp(dmx_buffer + address + PATCH_COLOR_OFFSET, &led_colors, 21)) {
            memcpy(&led_colors, dmx_buffer + address + PATCH_COLOR_OFFSET, 21);
            status.led_updated = 1;
        }
#endif // MH_WASH
    }

    return 0;
}


void setup_io() {
    // see main.h for pinout.
    PORTA.DIR &= 0x01;      // set PA[1-7] to inputs 
    PORTA.PIN0CTRL |= PORT_ISC_INPUT_DISABLE_gc;    // disable PA[0] completly
    PORTB.DIR = 0b00010111; // set PB[0-2, 4] to output PA[3] to input
    PORTC.DIR = 0b00000101; // set PC[0,2] to output, PC[3] to input
    
    // PWM
    // set prescale, enable timer.
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;
    // set single slope and enable compare ch 0,1
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0_bm | TCA_SINGLE_CMP1_bm;
    // disable split mode
    TCA0.SINGLE.CTRLD = 0;

    // reset every 6250 cycles. takes one cycle to reset
    TCA0.SINGLE.PER = 6249;

    // set start to center
    TCA0.SINGLE.CMP0 = 468;
    TCA0.SINGLE.CMP1 = 468;


    // TODO: uart(dmx), ws2812b, pullups?
}


uint8_t read_address() {
    // no full 8-bit port available due to udpi.
    uint8_t addr = PORTA.IN & 0xFE;
    addr |= (PORTC.IN >> 3) & 0x01;
    return addr;
}

void set_servos(uint8_t pan, uint8_t tilt) {
    TCA0.SINGLE.CMP0 = 312 + (pan * 10 / 8);
    _delay_ms(1);   // stagger servo updates to keep power stable
    TCA0.SINGLE.CMP1 = 312 + (tilt * 10 / 8);
}

void set_led_color(color_t *color) {
    // TODO: ws2812b code
    return;
}
