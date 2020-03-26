// Author:       Wolter Hellmund Vega
// Date:         2020-03-24
// Description:  x
//               x
// License:      x

#include "buttons.h"

void
button_setup(
        Button *button,
        const uint8_t pin) {
    button_init_state_memory(button);
    button->id = 0;
    button->pin = pin;
    button->event = BTN_EV_NN;  
    button->currState = &button->memory.state0;
    button->prevState = &button->memory.state1;
}

void
button_init_state_memory(Button *button) {
    button->memory.state0.state      = BTN_ST_NONE;
    button->memory.state1.state      = BTN_ST_NONE;
    button->memory.state2.state      = BTN_ST_NONE;
    button->memory.state3.state      = BTN_ST_NONE;
    button->memory.state0.state_life = 0;
    button->memory.state1.state_life = 0;
    button->memory.state2.state_life = 0;
    button->memory.state3.state_life = 0;
    return;
}

void 
button_update_state(Button *button, 
        const uint8_t state, 
        const uint16_t state_life) 
{
    button->memory.state3            = button->memory.state2;
    button->memory.state2            = button->memory.state1;
    button->memory.state1            = button->memory.state0;
    button->memory.state0.state      = state;
    button->memory.state0.state_life = state_life;
    return;
}

bool 
button_get_doublepressed(Button *button) {
    return ((button->memory.state3.state == BTN_ST_DOWN) && 
            (val_is_within_range(button->memory.state3.state_life,BTN_DEL_MIN,BTN_DEL_HOLD)) &&
            (button->memory.state2.state == BTN_ST_UP) && 
            (val_is_within_range(button->memory.state2.state_life,BTN_DEL_MIN,BTN_DEL_BMAX)) &&
            (button->memory.state1.state == BTN_ST_DOWN) &&
            (val_is_within_range(button->memory.state1.state_life,BTN_DEL_MIN,BTN_DEL_HOLD)));
}

bool
button_get_singlepressed(Button *button) {
    return ((button->memory.state1.state == BTN_ST_DOWN) &&
            (val_is_within_range(button->memory.state1.state_life,BTN_DEL_MIN,BTN_DEL_HOLD)));
}

bool
button_get_held(Button *button) {
    return ((button->memory.state0.state == BTN_ST_DOWN) &&
            (val_is_within_range(button->memory.state0.state_life,BTN_DEL_MIN,BTN_DEL_HOLD)));
}

void
button_interpret_state_memory(Button *button) {
    /* if (button->event == BTN_EV_NN) { */
        if (button_get_doublepressed(button)) {
            button->event = BTN_EV_DP;
            button_clear_button_state_memory(button);
        } else if (button_get_singlepressed(button)) {
            button->event = BTN_EV_SP;
            button_clear_button_state_memory(button);
        } else if (button_get_held(button)) {
            button->event = BTN_EV_HL;
            button_clear_button_state_memory(button);
        } else {
            button->event = BTN_EV_NN;
        }
    /* } else { */
    /*     button->event = BTN_EV_NN; */
    /* } */
    return;
}

void button_update(Button *button, const uint8_t ts) {
    // Update state and its life
    uint8_t read_state = BTN_ST_NONE;
    if (digitalRead(button->pin) == 1) {
        read_state = BTN_ST_UP;
    } else {
        read_state = BTN_ST_DOWN;
    }

    if (read_state == button->currState->state) {
        if (button->currState->state_life+ts < BTN_STATE_LIFE_MAX) {
            button->currState->state_life += ts;
        }
    } else {
        button_update_state(button, read_state, 0);
    }

    // Detect events
    button_interpret_state_memory(button);
    return;
}

void
button_clear_button_state_memory(Button *button) {
    button->memory.state0.state = BTN_ST_NONE;
    button->memory.state1.state = BTN_ST_NONE;
    button->memory.state2.state = BTN_ST_NONE;
    button->memory.state3.state = BTN_ST_NONE;
    button->memory.state0.state_life = 0;
    button->memory.state1.state_life = 0;
    button->memory.state2.state_life = 0;
    button->memory.state3.state_life = 0;
    return;
}

