// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#ifndef H_BUTTONS
#define H_BUTTONS

#include <Arduino.h>
#include <stdint.h>
#include "main.h"

#define BTN_DEL_MIN        10
#define BTN_DEL_HOLD       500
#define BTN_DEL_BMAX       100
#define BTN_STATE_LIFE_MAX 3000
#define BTN_MIN_STATE_AGE 15

#define BTN_TIMER_SINGLE_PRESS 300
#define BTN_TIMER_LONG_PRESS   700
#define BTN_TIMER_DOUBLE_PRESS 1000

typedef struct
{
        uint8_t id;
        uint8_t pin;
        uint8_t state;
        uint8_t position_curr;
        uint8_t position_prev;
        uint16_t position_age;
        uint16_t timer;
        void *callback_data;
        void (*callback_long_press)(void*);
        void (*callback_single_press)(void*);
        void (*callback_double_press)(void*);
} Button;

enum {
        BTN_EV_SP,
        BTN_EV_DP,
        BTN_EV_HL,
        BTN_EV_NN
};

enum {
        BTN_CB_SINGLE_PRESS,
        BTN_CB_LONG_PRESS,
        BTN_CB_DOUBLE_PRESS
};

enum {
        BTN_POS_UNKNOWN,
        BTN_POS_DOWN,
        BTN_POS_UP
};

void button_setup                     (Button         *button,
                                       const uint8_t   pin);

void button_update                    (Button         *button,
                                       const uint16_t   ts);

void button_connect (Button *button,
                     const uint8_t type,
                     void (*callback_function)(void*));

#endif // H_BUTTONS
