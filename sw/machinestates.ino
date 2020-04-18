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

