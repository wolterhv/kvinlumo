// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#include "machinestates.h"
#include "buttons.h"
#include "main.h"

#define   CHRONO_RESET          7000
#define   POT1_MIN              0
#define   POT1_MAX              1023
#define   POT2_MIN              0
#define   POT2_MAX              1023
#define   PHR_MIN               0
#define   PHR_MAX               1023
#define   BL_PER_MIN            300
#define   BL_PER_MAX            1500
#define   BL_POT_FACT           (BL_PER_MAX-BL_PER_MIN)/(POT1_MAX-POT1_MIN)
#define   SYSTEM_CYCLE_PERIOD   10

// Declare globals
int    sensor_amb;
int    sensor_pot;
float  pot;

int    time;
int    timep;
int    ts;

int    bl_hl;
int    bl_fl;
int    bl_fr;
int    bl_rl;
int    bl_rr;

typedef struct 
{
        uint16_t  chrono;
        float     duty_cycle;
        uint16_t  cycle_period;
        uint8_t   modelight;
        uint8_t   blinker;
        uint8_t   anti_modelight;
        uint8_t   anti_blinker;
} DynLight;

MachineStates     machineStates;
GestureHandler    gestureHandler;
DynLight          dynLight;
Button            leftButton;
Button            rightButton;

// Setup system
void 
setup () 
{
        // Setup serial port
        Serial.begin (9600);
        Serial.println ("setup ()");

        machine_states_configure (&machineStates,
                                  S_AMB_NIGHT,
                                  S_HEA_FWH,
                                  S_BRK_NB,
                                  S_LMO_BL);

        // Initialize dynamic light signal
        dynLight.duty_cycle     = 0.5;
        dynLight.cycle_period   = 700;
        dynLight.modelight      = 0;
        dynLight.anti_modelight = 0;
        dynLight.blinker        = 0;
        dynLight.anti_blinker   = 0;

        // Initialize inputs
        sensor_amb =  1;
        sensor_pot =  2;
        pot =         0;
        // float ain_pot = 0;
        // float ain_amb = 0;
        pinMode (sensor_amb,INPUT);
        pinMode (sensor_pot,INPUT);

        // Initialize buttons
        button_setup (&leftButton, 5);
        button_setup (&rightButton, 4);
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
        pot = analogRead (sensor_pot);
        button_update (&leftButton, ts);
        button_update (&rightButton, ts);

        // Process input
        // Update machine states
        // gesturehandler_process_gestures (&gestureHandler, &machineStates);
        // machine_states_update (&machineStates, &leftButton, &rightButton);

        gesturehandler_update (&gestureHandler, ts);

        // Update actuators
        dynlight_update (&dynLight, ts);

        set_lmo (&machineStates, &dynLight);
        set_hl (&machineStates, &dynLight, bl_hl);
        set_fx (&machineStates, &dynLight, S_HEA_SL, bl_fl);
        set_fx (&machineStates, &dynLight, S_HEA_SR, bl_fr);
        set_rx (&machineStates, &dynLight, S_HEA_SL, bl_rl);
        set_rx (&machineStates, &dynLight, S_HEA_SR, bl_rr);

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

/* void set_fr () { */
/*     return; */
/* } */
/*  */
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
        if (dynLight->chrono <= dynLight->cycle_period*dynLight->duty_cycle) {
                dynLight->blinker = HIGH;
                dynLight->anti_blinker = LOW;
        } else {
                dynLight->blinker = LOW;
                dynLight->anti_blinker = HIGH;
        }

        if (dynLight->chrono > dynLight->cycle_period) {
                dynLight->chrono -= dynLight->cycle_period;
        }

        if (states->sm_lmo == S_LMO_BL) {
                dynLight->modelight = dynLight->blinker;
                dynLight->anti_modelight = HIGH;
        } else if (states->sm_lmo == S_LMO_ST) {
                dynLight->modelight = HIGH;
                dynLight->anti_modelight = dynLight->blinker;
        }
}

void
dynlight_update (DynLight *dynLight,
                 const uint8_t ts) 
{
        dynLight->chrono += ts;
        return;
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
                     MachineStates *machineStates) 
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

        // if (gh->state == 5)
        //         digitalWrite(10,HIGH);
        // else
        //         digitalWrite(10,LOW);

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
