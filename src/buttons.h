// Author:       Wolter Hellmund Vega
// Date:         2020-03-24
// Description:  x
//               x
// License:      x

#ifndef H_BUTTONS
#define H_BUTTONS

#include <Arduino.h>
#include <stdint.h>
#include "main.h"

#define BTN_DEL_MIN        10
#define BTN_DEL_HOLD       500
#define BTN_DEL_BMAX       100
#define BTN_STATE_LIFE_MAX 3000

typedef struct {
    uint8_t state;
    uint16_t state_life;
} ButtonState;

typedef struct {
    ButtonState state0;
    ButtonState state1;
    ButtonState state2;
    ButtonState state3;
} ButtonStateMemory;

typedef struct {
    uint8_t id;
    uint8_t pin;
    ButtonStateMemory memory;
    ButtonState *currState;
    ButtonState *prevState;
    uint8_t event;
} Button;

enum {
    BTN_ST_NONE,
    BTN_ST_UP,
    BTN_ST_DOWN
};

enum {
    BTN_EV_SP,
    BTN_EV_DP,
    BTN_EV_HL,
    BTN_EV_NN
};

void
button_setup(
        Button *button,
        const uint8_t pin);

void 
button_update(
        Button *button,
        const uint8_t ts);

void 
button_update_state(
        Button *button, 
        const uint8_t state, 
        const uint16_t state_life);

bool 
button_get_doublepressed(
        Button *button);

bool
button_get_singlepressed(
        Button *button);

bool
button_get_held(
        Button *button);

void
button_init_state_memory(
        Button *button);

void
button_interpret_state_memory(
        Button *button);

void
button_clear_button_state_memory(
        Button *button);

#endif // H_BUTTONS
