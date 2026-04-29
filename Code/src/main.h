#include <avr/io.h>


// dmx patch
// 1: pan
// 2: tilt
// 3: red1
// 4: green1
// 5: blue1
// 6-23: red/green/blue 2...7 (for mh wash)

#define PATCH_PAN_OFFSET        0
#define PATCH_TILT_OFFSET       1
#define PATCH_MASTER_OFFSET     2
#define PATCH_COLOR_OFFSET      3

//
//                x -------------- PA1 addr1
//                |  x ----------- PA0 updi/prog
//                |  |  x -------- PC3 addr0
//                |  |  |  x ----- PC2 Status led
//                |  |  |  |  x -- PC1 x
//               20 21 22 23 24
//    addr2 PA2 1               15 PC0 Led
//    addr3 PA3 2               14 PB0 Pan
//    gnd   gnd 3               13 PB1 Tilt
//    vcc   vcc 4               12 PB2 Dmx tx
//    addr4 PA4 5               11 PB3 Dmx rx
//                6  7  8  9 10 
//                |  |  |  |  x -- PB4 Dmx dir
//                |  |  |  x ----- PB5 x
//                |  |  x -------- PA7 addr7
//                |  x ----------- PA6 addr6
//                x -------------- PA5 addr5
//

// uncomment correct model
#define MH_SPOT
#define MH_WASH

#if (defined(MH_SPOT) == defined(MH_WASH))
    // TODO: uncomment error check
    //#error "Invalidd model configuration: choose either SPOT or WASH"
#endif

// TODO: defines still needed? using direct PORT/PIN numbers in code
// and the hardware is already made.
// Keeping it here for reference right now
#define ADDR0       22
#define ADDR1       20
#define ADDR2       1
#define ADDR3       2
#define ADDR4       5
#define ADDR5       6
#define ADDR6       7
#define ADDR7       8

#define DMX_DIR     10
#define DMX_RX      11
#define DMX_TX      12

#define PWM_TILT    13
#define PWM_PAN     14
#define LED         15

#define STATUS_LED  23

// g r b ??
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color_t;

typedef struct {
    uint8_t position_updated;
    uint8_t led_updated;
} status_t;

void setup_io();

uint8_t read_address();

void set_servos(uint8_t pan, uint8_t tilt);

void set_led_color(color_t *color);
