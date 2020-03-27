// Author:       Wolter HELLMUND VEGA
// Date:         2020-03-15
// Description:  x
//               x
// License:      x

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

#define   BTN_DEL_MIN           10
#define   BTN_DEL_HOLD          500
#define   BTN_DEL_BMAX          100
#define   BTN_STATE_LIFE_MAX    3000

// Declare globals
int sensor_amb;
int sensor_pot;
float pot;

int time;
int timep;
int ts;

int bl_hl;
int bl_fl;
int bl_fr;
int bl_rl;
int bl_rr;

typedef struct {
    uint16_t chrono;
    float duty_cycle;
    uint8_t cycle_period;
    uint8_t modelight;
    uint8_t blinker;
    uint8_t anti_modelight;
    uint8_t anti_blinker;
} DynLight;

typedef struct {
    uint8_t sender;
    uint8_t type;
    bool processed;

} Event;

enum {
    LIGHT_OFF=LOW,
    LIGHT_ON=HIGH
};

MachineStates machineStates;
// MachineStates machineStates1;
// MachineStates machineStates2;
// MachineStates machineStates3;
// MachineStates machineStates4;
DynLight dynLight;
Button leftButton;
Button rightButton;

// Setup system
void setup() {
    // Setup serial port
    Serial.begin(9600);
    Serial.println("Hello world.");

    // Initialize state machines
    /* machine_states_configure(&machineStates1, S_AMB_NIGHT, S_HEA_FWH, S_BRK_NB, S_LMO_BL); */
    /* machine_states_configure(&machineStates2, S_AMB_NIGHT, S_HEA_SL,  S_BRK_NB, S_LMO_BL); */
    /* machine_states_configure(&machineStates3, S_AMB_NIGHT, S_HEA_SR,  S_BRK_BR, S_LMO_BL); */
    /* machine_states_configure(&machineStates4, S_AMB_NIGHT, S_HEA_FWH, S_BRK_BR, S_LMO_BL); */
    machine_states_configure(&machineStates, S_AMB_DAY, S_HEA_FWH, S_BRK_NB, S_LMO_BL);

    // Initialize dynamic light signal
    dynLight.duty_cycle     = 0.5;
    dynLight.cycle_period   = 700;
    dynLight.modelight      = 0;
    dynLight.anti_modelight = 0;
    dynLight.blinker        = 0;
    dynLight.anti_blinker   = 0;

    // Initialize inputs
    sensor_amb = 1;
    sensor_pot = 2;
    pot = 0;
    // float ain_pot = 0;
    // float ain_amb = 0;
    pinMode(sensor_amb,INPUT);
    pinMode(sensor_pot,INPUT);

    // Initialize buttons
    button_setup(&leftButton, 5);
    button_setup(&rightButton, 4);
    pinMode(leftButton.pin, INPUT);
    pinMode(rightButton.pin, INPUT);


    // Initialize outputs (lights)
    bl_hl = 10;
    bl_fl = 9;
    bl_fr = 8;
    bl_rl = 7;
    bl_rr = 6;
    pinMode(bl_hl,OUTPUT);
    pinMode(bl_fl,OUTPUT);
    pinMode(bl_fr,OUTPUT);
    pinMode(bl_rl,OUTPUT);
    pinMode(bl_rr,OUTPUT);

    // Setup clock
    time = 0;
    timep = 0;
    ts = 0;

    return;
}

// Main loop
void loop() {
    // Update time
    timep = time;
    time = millis() % CHRONO_RESET;
    ts = (time > timep ? time-timep : time-timep+CHRONO_RESET);

    // Update sensors
    pot = analogRead(sensor_pot);
    button_update(&leftButton, ts);
    button_update(&rightButton, ts);

    // Update machine states
    machine_states_update(&machineStates, &leftButton, &rightButton);

    // Update actuators
    dynlight_update(&dynLight, ts);

    set_lmo(&machineStates, &dynLight);
    set_hl(&machineStates, &dynLight, bl_hl);
    set_fx(&machineStates, &dynLight, S_HEA_SL, bl_fl);
    set_fx(&machineStates, &dynLight, S_HEA_SR, bl_fr);
    set_rx(&machineStates, &dynLight, S_HEA_SL, bl_rl);
    set_rx(&machineStates, &dynLight, S_HEA_SR, bl_rr);
    
    delay(SYSTEM_CYCLE_PERIOD);
    return;
}

// Other functions
void set_hl(
        const MachineStates *states,
        const DynLight *dynLight, 
        const int light) {
    if (states->sm_amb == S_AMB_NIGHT) {
        digitalWrite(light,dynLight->modelight);
    } else {
        digitalWrite(light,LOW);
    }
    return;
}

void set_fx(
        const MachineStates *states,
        const DynLight *dynLight,
        const int dir,
        const int light) {
    if (states->sm_hea == dir) {
        digitalWrite(light,dynLight->blinker);
    } else {
        digitalWrite(light,LOW);
    }
    // TODO: implement backup mode
    /* if (states->sm_hea == S_HEA_FWH) { */
    /*     if ( (states->sm_amb == S_AMB_DAY) &&   (states->sm_brk == S_BRK_BR) )  */
    /*     || ( (states->sm_amb == S_AMB_NIGHT) && (states->sm_brk == S_BRK_NB) ) { */
    /*         digitalWrite(bl_fl,); */
    /*     } */
    /* } */
    return;
}

/* void set_fr() { */
/*     return; */
/* } */
/*  */
void set_rx(
        const MachineStates *states,
        const DynLight *dynLight,
        const int dir,
        const int light) {
    if (states->sm_hea == S_HEA_FWH) {
        if (
            ((states->sm_amb == S_AMB_DAY) && (states->sm_brk == S_BRK_BR)) 
            || ((states->sm_amb == S_AMB_NIGHT) && (states->sm_brk == S_BRK_NB)) 
        ) {
            digitalWrite(light,dynLight->modelight);
        } else if ( (states->sm_amb == S_AMB_DAY) && (states->sm_brk == S_BRK_NB) ) {
            /* digitalWrite(light,LIGHT_OFF) */
            digitalWrite(light,LOW);
        } else if ( (states->sm_amb == S_AMB_NIGHT) && (states->sm_brk == S_BRK_BR) ) {
            /* digitalWrite(light,lmo_not) */
            digitalWrite(light,dynLight->anti_modelight);
        }
    } else if (states->sm_hea == dir) {
        digitalWrite(light,dynLight->blinker);
    } else {
        if (states->sm_brk == S_BRK_BR) {
            digitalWrite(light,HIGH);
        } else {
            digitalWrite(light,LOW);
        }
    }
    return;
}

void set_lmo(
        const MachineStates *states,
        DynLight *dynLight) {
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
dynlight_update(DynLight *dynLight, const uint8_t ts) {
    dynLight->chrono += ts;
    return;
}

bool
val_is_within_range(const int val, const int a, const int b) {
    return ((val >= a) && (val <= b));
}

