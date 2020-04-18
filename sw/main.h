// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#ifndef _MAIN_H_
#define _MAIN_H_

#include "machinestates.h"
#include "dynlight.h"
#include "buttons.h"
#include "gesturehandler.h"

void set_hl                              (const MachineStates *states,
                                          const DynLight      *dynLight,
                                          const int            light);

void set_fx                              (const MachineStates *states,
                                          const DynLight      *dynLight,
                                          const int            dir,
                                          const int            light);

void set_rx                              (const MachineStates *states,
                                          const DynLight      *dynLight,
                                          const int            dir,
                                          const int            light);

void set_lmo                             (const MachineStates *states,
                                          DynLight            *dynLight);

bool val_is_within_range                 (const int            val,
                                          const int            a,
                                          const int            b);

void callback_toggle_turn_left           (void                *callback_data);

void callback_toggle_turn_right          (void                *callback_data);

void callback_toggle_lmo                 (void                *callback_data);

void callback_toggle_amb                 (void                *callback_data);

void callback_set_lbutton_single_pressed (void                *gestureHandler);

void callback_set_rbutton_single_pressed (void                *gestureHandler);

void callback_set_lbutton_long_pressed   (void                *gestureHandler);

void callback_set_rbutton_long_pressed   (void                *gestureHandler);

void callback_set_lbutton_double_pressed (void                *gestureHandler);

#endif // _MAIN_H_
