// Author:       Wolter HELLMUND VEGA
// Date:         2020-03-15
// Description:  x
//               x
// License:      x

#ifndef H_MACHINESTATES
#define H_MACHINESTATES

#include <Arduino.h>
#include "buttons.h"

typedef struct {
    uint8_t sm_brk;
    uint8_t sm_hea;
    uint8_t sm_amb;
    uint8_t sm_lmo;
    double time;
} MachineStates;

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

void 
machine_states_configure(
        MachineStates *ms, 
        const int amb,
        const int hea,
        const int brk,
        const int lmo);

void 
machine_states_copy(
        const MachineStates *src, 
        MachineStates *tgt);

void
machine_states_update(
        MachineStates *machineStates,
        Button *leftButton,
        Button *rightButton);

#endif // H_MACHINESTATES
