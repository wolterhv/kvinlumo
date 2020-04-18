// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#ifndef _DYNLIGHT_H_
#define _DYNLIGHT_H_

#include <Arduino.h>

#define DYNLIGHT_DEFAULT_DUTY_CYCLE     0.5
#define DYNLIGHT_DEFAULT_CYCLE_PERIOD 700

#define INTERPOLATE(Y0,Y1,X0,X1,X) Y0+(Y1-Y0)/(X1-X0)*(X-X0)

typedef struct _DynLight
{
        uint16_t  chrono;
        float     duty_cycle;
        uint16_t  cycle_period;
        uint8_t   modelight;
        uint8_t   blinker;
        uint8_t   anti_modelight;
        uint8_t   anti_blinker;
} DynLight;

void dynlight_init   (DynLight      *dynLight);

void dynlight_update (DynLight      *dynLight,
                      const uint8_t  ts);

#endif // _DYNLIGHT_H_
