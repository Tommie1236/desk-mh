#include <avr/io.h>



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
