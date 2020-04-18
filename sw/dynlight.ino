// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#include "dynlight.h"

void
dynlight_init (DynLight *dynLight)
{
        // Init config
        dynLight->duty_cycle     = DYNLIGHT_DEFAULT_DUTY_CYCLE;
        dynLight->cycle_period   = DYNLIGHT_DEFAULT_CYCLE_PERIOD;

        // Init lights
        dynLight->modelight      = LOW;
        dynLight->anti_modelight = LOW;
        dynLight->blinker        = LOW;
        dynLight->anti_blinker   = LOW;

        return;
}

void
dynlight_update (DynLight *dynLight,
                 const uint8_t ts) 
{
        dynLight->chrono += ts;

        if (dynLight->chrono <= dynLight->cycle_period*dynLight->duty_cycle) {
                dynLight->blinker      = HIGH;
                dynLight->anti_blinker = LOW;
        } else {
                dynLight->blinker      = LOW;
                dynLight->anti_blinker = HIGH;
        }

        if (dynLight->chrono > dynLight->cycle_period) {
                dynLight->chrono -= dynLight->cycle_period;
        }

        return;
}
