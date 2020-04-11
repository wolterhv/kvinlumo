// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#include "machinestates.h"

void 
machine_states_configure (MachineStates *ms,
                          const int      amb,
                          const int      hea,
                          const int      brk,
                          const int      lmo)
{
        ms->sm_amb = amb;
        ms->sm_hea = hea;
        ms->sm_brk = brk;
        ms->sm_lmo = lmo;
        return;
}

void 
machine_states_copy (const MachineStates *src,
                     MachineStates       *tgt)
{
        tgt->sm_amb = src->sm_amb;
        tgt->sm_hea = src->sm_hea;
        tgt->sm_brk = src->sm_brk;
        tgt->sm_lmo = src->sm_lmo;
        return;
}

// void
// machine_states_update (MachineStates *machineStates,
//                        Button        *leftButton,
//                        Button        *rightButton)
// {
//         /* if ((leftButton.event == BTN_EV_SP) && (rightButton.event != BTN_EV_SP)) { */
//         if (leftButton->event == BTN_EV_SP) {
//                 // Left button single press
//                 /* Serial.println ("Left button single press"); */
//                 leftButton->event  = BTN_EV_NN;
//                 rightButton->event = BTN_EV_NN;
// 
//                 if (machineStates->sm_hea == S_HEA_SL) {
//                         machineStates->sm_hea = S_HEA_FWH;
//                 } else {
//                         machineStates->sm_hea = S_HEA_SL;
//                 }
//         } else if ((rightButton->event == BTN_EV_SP) && 
//                    (leftButton->event != BTN_EV_SP)) {
//                 /* Serial.println ("Right button single press"); */
//                 leftButton->event  = BTN_EV_NN;
//                 rightButton->event = BTN_EV_NN;
// 
//                 if (machineStates->sm_hea == S_HEA_SR) {
//                         machineStates->sm_hea = S_HEA_FWH;
//                 } else {
//                         machineStates->sm_hea = S_HEA_SR;
//                 }
//                 // Delays may be required to be taken into account because buttons
//                 // } else if (
//                 //         (leftButton->event == BTN_EV_HL) &&
//                 //         (rightButton->event == BTN_EV_HL)) {
//                 //     Serial.println ("Both buttons.held");
//                 //     leftButton->event  = BTN_EV_NN;
//                 //     rightButton->event = BTN_EV_NN;
//                 //     if (machineStates->sm_amb == S_AMB_DAY) {
//                 //         machineStates->sm_amb = S_AMB_NIGHT;
//                 //     } else {
//                 //         machineStates->sm_amb = S_AMB_DAY;
//                 //     }
//                 // } else if (
//                 //         (leftButton->event == BTN_EV_DP) &&
//                 //         (rightButton->event == BTN_EV_DP)) {
//                 //     Serial.println ("Both buttons double pressed");
//                 //     leftButton->event  = BTN_EV_NN;
//                 //     rightButton->event = BTN_EV_NN;
//                 //     if (machineStates->sm_lmo == S_LMO_BL) {
//                 //         machineStates->sm_lmo = S_LMO_ST;
//                 //     } else {
//                 //         machineStates->sm_lmo = S_LMO_BL;
//                 //     }
//         }
// 
//         // if (dynLight.cycle_period >= (BL_PER_MIN+BL_PER_MAX)/2) {
//         /* if (pot/POT1_MAX < 0.25)  { */
//         /*     machine_states_copy (&machineStates1, &machineStates); */
//         /* machineStates->sm_lmo = S_LMO_BL; */
//         /* machineStates->sm_hea = S_HEA_SL; */
//         /* machineStates->sm_amb = S_AMB_DAY; */
//         /* machineStates->sm_brk = S_BRK_NB; */
//         /* } else if (pot/POT1_MAX < 0.50) { */
//         /*     machine_states_copy (&machineStates2, &machineStates); */
//         /* } else if (pot/POT1_MAX < 0.75) { */
//         /*     machine_states_copy (&machineStates3, &machineStates); */
//         /* } else if (analogRead (sensor_pot)/POT_MAX < 1) { */
//         /* } else { */
//         /*     machine_states_copy (&machineStates4, &machineStates); */
//         /* machineStates->sm_amb = S_AMB_NIGHT; */
//         /* machineStates->sm_lmo = S_LMO_ST; */
//         /* machineStates->sm_hea = S_HEA_FWH; */
//         /* machineStates->sm_brk = S_BRK_BR; */
//         /* } */
// }
