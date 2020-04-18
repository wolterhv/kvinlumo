// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#ifndef _MACHINESTATES_H_
#define _MACHINESTATES_H_

#include <Arduino.h>
#include "buttons.h"

// brake states
enum {
        S_BRK_BR,       // braking
        S_BRK_NB        // not braking
};

// heading states
enum {
        S_HEA_FWH,      // forward
        S_HEA_SL,       // steering left
        S_HEA_SR        // steering right
};

// ambient states
enum {
        S_AMB_DAY,      // day
        S_AMB_NIGHT     // night
};

// light modes
enum {
        S_LMO_ST,       // steady mode
        S_LMO_BL        // blinking
};

typedef struct _MachineStates
{
        uint8_t  sm_brk;
        uint8_t  sm_hea;
        uint8_t  sm_amb;
        uint8_t  sm_lmo;
        double   time;
} MachineStates;

void machine_states_configure (MachineStates       *ms,
                               const int            amb,
                               const int            hea,
                               const int            brk,
                               const int            lmo);

void machine_states_copy      (const MachineStates *src,
                               MachineStates       *tgt);

#endif // _MACHINESTATES_H_
