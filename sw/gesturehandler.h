// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#ifndef _GESTUREHANDLER_H_
#define _GESTUREHANDLER_H_

#include <Arduino.h>
#include "machinestates.h"

enum {
        GH_BTN_UNKNOWN,
        GH_BTN_SPRESSED,
        GH_BTN_LPRESSED,
        GH_BTN_DPRESSED
};

typedef struct _GestureHandler {
        uint8_t    state;
        uint8_t    state_rbutton;
        uint8_t    state_lbutton;
        uint16_t   timer;
        uint16_t   timeout_spress;
        uint16_t   timeout_lpress;
        uint16_t   timeout_dpress;
        void      *callback_data;
        void     (*callback_left_spressed)(void*);
        void     (*callback_both_spressed)(void*);
        void     (*callback_right_spressed)(void*);
        void     (*callback_left_lpressed)(void*);
        void     (*callback_left_dpressed)(void*);
        void     (*callback_right_lpressed)(void*);
        void     (*callback_right_dpressed)(void*);
        void     (*callback_both_lpressed)(void*);
        void     (*callback_both_dpressed)(void*);
} GestureHandler;

void gesturehandler_init           (GestureHandler *gestureHandler,
                                    MachineStates  *machineStates);

void gesturehandler_setup          (GestureHandler *gh);

void gesturehandler_update         (GestureHandler *gh,
                                    const uint16_t  ts);

void gesturehandler_callback_empty (void           *data);

#endif // _GESTUREHANDLER_H_
