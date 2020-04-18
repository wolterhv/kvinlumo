// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#include "gesturehandler.h"

void
gesturehandler_setup (GestureHandler *gh)
{
        gh->state = 0;
        gh->state_lbutton = GH_BTN_UNKNOWN;
        gh->state_rbutton = GH_BTN_UNKNOWN;
        gh->timer = 0;
        gh->timeout_spress = 300;
        gh->timeout_lpress = 300;
        gh->timeout_dpress = 300;

        gh->callback_left_spressed = &gesturehandler_callback_empty;
        gh->callback_left_lpressed = &gesturehandler_callback_empty;
        gh->callback_left_dpressed = &gesturehandler_callback_empty;
        gh->callback_right_spressed = &gesturehandler_callback_empty;
        gh->callback_right_lpressed = &gesturehandler_callback_empty;
        gh->callback_right_dpressed = &gesturehandler_callback_empty;
        gh->callback_both_spressed = &gesturehandler_callback_empty;
        gh->callback_both_lpressed = &gesturehandler_callback_empty;
        gh->callback_both_dpressed = &gesturehandler_callback_empty;

        gh->callback_data = NULL;
}

void
gesturehandler_init (GestureHandler *gestureHandler,
                     MachineStates  *machineStates)
{
        gesturehandler_setup (gestureHandler);

        gestureHandler->callback_data = (void*) machineStates;

        gestureHandler->callback_left_spressed = &callback_toggle_turn_left;
        gestureHandler->callback_right_spressed = &callback_toggle_turn_right;

        gestureHandler->callback_left_lpressed = &callback_toggle_lmo;
        gestureHandler->callback_right_lpressed = &callback_toggle_amb;

        gestureHandler->callback_left_dpressed = &callback_toggle_brk;

        return;
}

void
gesturehandler_update (GestureHandler *gh,
                       const uint16_t ts) 
{
        // Count down
        gh->timer = (gh->timer >= ts) ? gh->timer-ts : 0;
        bool timed_out = (gh->timer == 0);

        uint8_t next_state = gh->state;
        switch (gh->state) {
        // Default state
        case 0:
                // In this state, gh->state is used instead of next_state
                // because it would come in handy to stay in the switch in the
                // same cycle

                // Single press states
                if        (gh->state_lbutton == GH_BTN_SPRESSED) {
                        next_state = 1;
                        gh->timer = gh->timeout_spress;
                } else if (gh->state_rbutton == GH_BTN_SPRESSED) {
                        next_state = 2;
                        gh->timer = gh->timeout_spress;
                // Long press states
                } else if (gh->state_lbutton == GH_BTN_LPRESSED) {
                        next_state = 3;
                        gh->timer = gh->timeout_lpress;
                } else if (gh->state_rbutton == GH_BTN_LPRESSED) {
                        next_state = 4;
                        gh->timer = gh->timeout_lpress;
                // Double press states
                } else if (gh->state_lbutton == GH_BTN_DPRESSED) {
                        next_state = 5;
                        gh->timer = gh->timeout_dpress;
                } else if (gh->state_rbutton == GH_BTN_DPRESSED) {
                        next_state = 6;
                        gh->timer = gh->timeout_dpress;
                }
                break;
        // Left button single-pressed
        case 1:
                if (timed_out) {
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        Serial.println ("gesturehandler: lbutton single_pressed");
                        gh->callback_left_spressed (gh->callback_data);
                        next_state = 0;

                } else if (gh->state_rbutton == GH_BTN_SPRESSED) {
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        gh->callback_both_spressed (gh->callback_data);
                        next_state = 0;
                }     
                break;
        // Right button single-pressed
        case 2:
                if (timed_out) {
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        Serial.println ("gesturehandler: rbutton single_pressed");
                        gh->callback_right_spressed (gh->callback_data);
                        next_state = 0;

                } else if (gh->state_lbutton == GH_BTN_SPRESSED) {
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        gh->callback_both_spressed (gh->callback_data);
                        next_state = 0;
                }     
                break;
        // Left button long-pressed
        case 3:
                if (timed_out) {
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        Serial.println ("gesturehandler: lbutton long_pressed");
                        gh->callback_left_lpressed (gh->callback_data);
                        next_state = 0;

                } else if (gh->state_rbutton == GH_BTN_LPRESSED) {
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        gh->callback_both_lpressed (gh->callback_data);
                        next_state = 0;
                }
                break;
        // Right button long-pressed
        case 4:
                if (timed_out) {
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        Serial.println ("gesturehandler: rbutton long_pressed");
                        gh->callback_right_lpressed (gh->callback_data);
                        next_state = 0;

                } else if (gh->state_lbutton == GH_BTN_LPRESSED) {
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        gh->callback_both_lpressed (gh->callback_data);
                        next_state = 0;
                }
                break;
        // Left button double-pressed
        case 5:
                if (timed_out) {
                        Serial.println ("gesturehandler: lbutton double_pressed");
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        gh->callback_left_dpressed (gh->callback_data);
                        next_state = 0;

                } else if (gh->state_rbutton == GH_BTN_DPRESSED) {
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        gh->callback_both_dpressed (gh->callback_data);
                        next_state = 0;
                }
                break;
        // Right button double-pressed
        case 6:
                if (timed_out) {
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        gh->callback_right_dpressed (gh->callback_data);
                        next_state = 0;

                } else if (gh->state_lbutton == GH_BTN_DPRESSED) {
                        gh->state_lbutton = GH_BTN_UNKNOWN;
                        gh->state_rbutton = GH_BTN_UNKNOWN;
                        gh->callback_both_dpressed (gh->callback_data);
                        next_state = 0;
                }
                break;
        }
        gh->state = next_state;
        return;
}

void
gesturehandler_callback_empty (void *data)
{
        return;
}

