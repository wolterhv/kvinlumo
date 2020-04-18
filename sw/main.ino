// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#include "main.h"

#define   CHRONO_RESET          7000
#define   SYSTEM_CYCLE_PERIOD   10

#define   POT1_MIN              0
#define   POT1_MAX              1023
#define   POT2_MIN              POT1_MIN
#define   POT2_MAX              POT1_MAX

#define   BL_CP_MIN             300
#define   BL_CP_MAX             1500
#define   BL_DC_MIN             0.2
#define   BL_DC_MAX             0.8

// Declare globals
int    sensor_amb;
int    sensor_pot1;
int    sensor_pot2;
float  pot1;
float  pot2;

int    time;
int    timep;
int    ts;

int    bl_hl;
int    bl_fl;
int    bl_fr;
int    bl_rl;
int    bl_rr;

char message[100];

MachineStates  machineStates;
GestureHandler gestureHandler;
DynLight       dynLight;
Button         leftButton;
Button         rightButton;

// Setup system
void 
setup () 
{
        // Setup serial port
        Serial.begin (9600);
        while (!Serial) {
                delay(SYSTEM_CYCLE_PERIOD);
        }
        Serial.println ("Kvinlumo");

        machine_states_configure (&machineStates,
                                  S_AMB_NIGHT,
                                  S_HEA_FWH,
                                  S_BRK_NB,
                                  S_LMO_BL);

        // Initialize dynamic light signal
        dynlight_init (&dynLight);

        // Initialize inputs
        sensor_amb  = A1;
        sensor_pot1 = A2;
        sensor_pot2 = A3;
        pot1        = 0;
        pot2        = 0;
        // float ain_pot = 0;
        // float ain_amb = 0;

        // Initialize buttons
        button_init (&leftButton, 5);
        button_init (&rightButton, 4);
        pinMode (leftButton.pin, INPUT);
        pinMode (rightButton.pin, INPUT);

        button_connect (&leftButton,
                        BTN_CB_SINGLE_PRESS,
                        &callback_set_lbutton_single_pressed);
        button_connect (&rightButton,
                        BTN_CB_SINGLE_PRESS,
                        &callback_set_rbutton_single_pressed);
        button_connect (&leftButton,
                        BTN_CB_LONG_PRESS,
                        &callback_set_lbutton_long_pressed);
        button_connect (&rightButton,
                        BTN_CB_LONG_PRESS,
                        &callback_set_rbutton_long_pressed);
        button_connect (&leftButton,
                        BTN_CB_DOUBLE_PRESS,
                        &callback_set_lbutton_double_pressed);

        button_set_callback_data (&leftButton,  (void*) &gestureHandler);
        button_set_callback_data (&rightButton, (void*) &gestureHandler);

        // Initialize outputs (lights)
        bl_hl =  10;
        bl_fl =  9;
        bl_fr =  8;
        bl_rl =  7;
        bl_rr =  6;
        pinMode (bl_hl,OUTPUT);
        pinMode (bl_fl,OUTPUT);
        pinMode (bl_fr,OUTPUT);
        pinMode (bl_rl,OUTPUT);
        pinMode (bl_rr,OUTPUT);

        // Setup clock
        time =   0;
        timep =  0;
        ts =     0;

        gesturehandler_init (&gestureHandler, &machineStates);

        // everything ok here
        return;
}

// Main loop
void 
loop () 
{
        // Update time

        timep = time;
        time = millis () % CHRONO_RESET;
        ts = (time > timep ? time-timep : time-timep+CHRONO_RESET);

        // Update sensors
        pot1 = analogRead (sensor_pot1);
        pot2 = analogRead (sensor_pot2);

        if (time % 100 > 90) {
                sprintf(message, "pot1: %d\tpot2: %d\n", (int) pot1, pot2);
                Serial.println (message); 
        }

        button_update (&leftButton, ts);
        button_update (&rightButton, ts);

        // Process input

        dynLight.duty_cycle   = INTERPOLATE(BL_DC_MIN,  BL_DC_MAX,
                                            POT1_MIN,   POT1_MAX,  pot1);
        dynLight.cycle_period = INTERPOLATE(BL_CP_MIN,  BL_CP_MAX, 
                                            POT2_MIN,   POT2_MAX,  pot2);
        gesturehandler_update (&gestureHandler, ts);

        // Update actuators

        dynlight_update (&dynLight, ts);

        set_lmo (&machineStates, &dynLight);
        set_hl (&machineStates, &dynLight, bl_hl);
        set_fx (&machineStates, &dynLight, S_HEA_SL, bl_fl);
        set_fx (&machineStates, &dynLight, S_HEA_SR, bl_fr);
        set_rx (&machineStates, &dynLight, S_HEA_SL, bl_rl);
        set_rx (&machineStates, &dynLight, S_HEA_SR, bl_rr);

        // Finish cycle

        delay (SYSTEM_CYCLE_PERIOD);
        return;
}

// Other functions
void 
set_hl (const MachineStates *states,
        const DynLight      *dynLight,
        const int            light) 
{
        if (states->sm_amb == S_AMB_NIGHT) {
                digitalWrite (light,dynLight->modelight);
        } else {
                digitalWrite (light,LOW);
        }
        return;
}

void 
set_fx (const MachineStates *states,
        const DynLight      *dynLight,
        const int            dir,
        const int            light)
{
        if (states->sm_hea == dir) {
                digitalWrite (light,dynLight->blinker);
        } else {
                digitalWrite (light,LOW);
        }
        // TODO: implement backup mode
        /* if (states->sm_hea == S_HEA_FWH) { */
        /*     if ( (states->sm_amb == S_AMB_DAY) &&   (states->sm_brk == S_BRK_BR) )  */
        /*     || ( (states->sm_amb == S_AMB_NIGHT) && (states->sm_brk == S_BRK_NB) ) { */
        /*         digitalWrite (bl_fl,); */
        /*     } */
        /* } */
        return;
}

void 
set_rx (const MachineStates *states,
        const DynLight      *dynLight,
        const int            dir,
        const int            light)
{
        if (states->sm_hea == S_HEA_FWH) {
                if (
                    ((states->sm_amb == S_AMB_DAY) && (states->sm_brk == S_BRK_BR)) 
                    || ((states->sm_amb == S_AMB_NIGHT) && (states->sm_brk == S_BRK_NB)) 
                   ) {
                        digitalWrite (light,dynLight->modelight);
                } else if ( (states->sm_amb == S_AMB_DAY) && (states->sm_brk == S_BRK_NB) ) {
                        /* digitalWrite (light,LIGHT_OFF) */
                        digitalWrite (light,LOW);
                } else if ( (states->sm_amb == S_AMB_NIGHT) && (states->sm_brk == S_BRK_BR) ) {
                        /* digitalWrite (light,lmo_not) */
                        digitalWrite (light,dynLight->anti_modelight);
                }
        } else if (states->sm_hea == dir) {
                digitalWrite (light,dynLight->blinker);
        } else {
                if (states->sm_brk == S_BRK_BR) {
                        digitalWrite (light,HIGH);
                } else {
                        digitalWrite (light,LOW);
                }
        }
        return;
}

void set_lmo (const MachineStates *states,
              DynLight            *dynLight) 
{
        if (states->sm_lmo == S_LMO_BL) {
                dynLight->modelight = dynLight->blinker;
                dynLight->anti_modelight = HIGH;
        } else if (states->sm_lmo == S_LMO_ST) {
                dynLight->modelight = HIGH;
                dynLight->anti_modelight = dynLight->blinker;
        }
}

bool
val_is_within_range (const int val, 
                     const int a,
                     const int b) 
{
        return ((val >= a) && (val <= b));
}

void
callback_toggle_turn_left (void *callback_data)
{
        MachineStates *machineStates = (MachineStates*) callback_data;
        if (machineStates->sm_hea == S_HEA_SL)
                machineStates->sm_hea = S_HEA_FWH;
        else
                machineStates->sm_hea = S_HEA_SL;
        return;
}

void
callback_toggle_turn_right (void *callback_data)
{
        MachineStates *machineStates = (MachineStates*) callback_data;
        if (machineStates->sm_hea == S_HEA_SR)
                machineStates->sm_hea = S_HEA_FWH;
        else
                machineStates->sm_hea = S_HEA_SR;
        return;
}

void
callback_toggle_lmo (void *callback_data)
{
        MachineStates *machineStates = (MachineStates*) callback_data;
        if (machineStates->sm_lmo == S_LMO_BL)
                machineStates->sm_lmo = S_LMO_ST;
        else
                machineStates->sm_lmo = S_LMO_BL;
        return;
}

void
callback_toggle_amb (void *callback_data) 
{
        MachineStates *machineStates = (MachineStates*) callback_data;
        if (machineStates->sm_amb == S_AMB_DAY)
                machineStates->sm_amb = S_AMB_NIGHT;
        else
                machineStates->sm_amb = S_AMB_DAY;
        return;
}

void
callback_toggle_brk (void *callback_data)
{
        MachineStates *machineStates = (MachineStates*) callback_data;
        if (machineStates->sm_brk == S_BRK_NB)
                machineStates->sm_brk = S_BRK_BR;
        else
                machineStates->sm_brk = S_BRK_NB;
        return;
}

void
callback_set_lbutton_single_pressed (void *gestureHandler)
{
        Serial.println ("button: lbutton single_pressed");
        ((GestureHandler*) gestureHandler)->state_lbutton = GH_BTN_SPRESSED;
        return;
}

void
callback_set_rbutton_single_pressed (void *gestureHandler)
{
        Serial.println ("button: rbutton single_pressed");
        ((GestureHandler*) gestureHandler)->state_rbutton = GH_BTN_SPRESSED;
        return;
}

void 
callback_set_lbutton_long_pressed (void *gestureHandler)
{
        Serial.println ("button: lbutton long_pressed");
        ((GestureHandler*) gestureHandler)->state_lbutton = GH_BTN_LPRESSED;
        return;
}

void 
callback_set_rbutton_long_pressed (void *gestureHandler)
{
        Serial.println ("button: rbutton long_pressed");
        ((GestureHandler*) gestureHandler)->state_rbutton = GH_BTN_LPRESSED;
        return;
}

void
callback_set_lbutton_double_pressed (void *gestureHandler)
{
        Serial.println ("button: lbutton double_pressed");
        ((GestureHandler*) gestureHandler)->state_lbutton = GH_BTN_DPRESSED;
        return;
}

// vim: set cin cino+=(0
