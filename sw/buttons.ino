// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#include "buttons.h"

void
button_init (Button        *button,
             const uint8_t  pin)
{
        button->id                    = 0;
        button->pin                   = pin;
        button->state                 = 0;
        button->timer                 = 0;
        button->position_curr         = BTN_POS_UNKNOWN;
        button->position_prev         = BTN_POS_UNKNOWN;
        button->position_age          = 0;
        button->callback_single_press = &button_callback_empty;
        button->callback_long_press   = &button_callback_empty;
        button->callback_double_press = &button_callback_empty;
        button->callback_data         = NULL;
}

void 
button_set_callback_data (Button *button,
                          void   *callback_data)
{
        button->callback_data = callback_data;
        return;
}

void
button_connect (Button         *button,
                const uint8_t   callback_type,
                void          (*callback_function)(void*))
{
        switch (callback_type) {
        case BTN_CB_SINGLE_PRESS:
                button->callback_single_press = callback_function;
        case BTN_CB_LONG_PRESS:
                button->callback_long_press   = callback_function;
        case BTN_CB_DOUBLE_PRESS:
                button->callback_double_press = callback_function;
        }
        return;
}

void 
button_update (Button         *button,
               const uint16_t  ts)
{
        // Update button position
        uint8_t read_state = BTN_POS_UNKNOWN;
        read_state = (digitalRead (button->pin) == 1)
                     ? BTN_POS_UP : BTN_POS_DOWN;

        button->timer = (button->timer >= ts) ? (button->timer-ts) : 0;

        bool button_down = ((read_state == BTN_POS_DOWN) &&
                            (button->position_age > BTN_TIMER_POS_MIN_AGE));
        bool button_up   = ((read_state == BTN_POS_UP) &&
                            (button->position_age > BTN_TIMER_POS_MIN_AGE));
        bool timed_out   = (button->timer == 0);

        button->position_prev = button->position_curr;
        if        (button_down) {
                button->position_curr = BTN_POS_DOWN;
        } else if (button_up) {
                button->position_curr = BTN_POS_UP;
        }

        // Update button position age
        button->position_age = (button->position_curr == button->position_prev)
                               ? button->position_age+ts : 0;

        // buttton state machine
        // based on diagram at http://mathertel.de/Arduino/OneButtonLibrary.aspx
        uint8_t next_state = button->state;
        switch (button->state) {
        case 0: 
                if (button_down) {
                        button->timer = BTN_TIMER_LONG_PRESS;
                        next_state = 1;
                }
                break;
        case 1:
                if (button->state == 1) {
                        if (button_up) {
                                button->timer = BTN_TIMER_SINGLE_PRESS;
                                next_state = 2;
                        } else if (timed_out) {
                                // send long_press
                                button->callback_long_press (button->callback_data);
                                next_state = 6;
                        }
                }
                break;
        case 2:
                if (button_down) {
                        // button->timer = timer3;
                        next_state = 3;
                } else if (timed_out) {
                        // send single_press
                        button->callback_single_press (button->callback_data);
                        next_state = 0;
                }
                break;
        case 3:
                if (button_up) {
                        // send double_press
                        button->callback_double_press (button->callback_data);
                        next_state = 0;
                }
                break;
        case 6:
                if (button_up) {
                        next_state = 0;
                }
                break;
        }
        button->state = next_state;

        return;
}

void
button_callback_empty (void *data) 
{
        return;
}
