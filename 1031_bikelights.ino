// Author:       Wolter HELLMUND VEGA
// Date:         2020-03-15
// Description:  x
//               x
// License:      x

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

int bl_hl;
int bl_fl;
int bl_fr;
int bl_rl;
int bl_rr;

typedef struct {
    uint8_t sm_brk;
    uint8_t sm_hea;
    uint8_t sm_amb;
    uint8_t sm_lmo;
    double time;
} MachineStates;

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
    uint8_t state;
    uint16_t state_life;
} ButtonState;

typedef struct {
    ButtonState state0;
    ButtonState state1;
    ButtonState state2;
    ButtonState state3;
} ButtonStateMemory;

typedef struct {
    uint8_t id;
    uint8_t pin;
    ButtonStateMemory memory;
    ButtonState *currState;
    ButtonState *prevState;
    uint8_t event;
} Button;

enum {
    BTN_ST_NONE,
    BTN_ST_UP,
    BTN_ST_DOWN
};

enum {
    BTN_EV_SP,
    BTN_EV_DP,
    BTN_EV_HL,
    BTN_EV_NN
};

enum {
    S_BRK_BR,
    S_BRK_NB
};

enum {
    S_HEA_FWH,
    S_HEA_SL,
    S_HEA_SR
};

enum {
    S_AMB_DAY,
    S_AMB_NIGHT
};

enum {
    S_LMO_ST,
    S_LMO_BL
};

enum {
    LIGHT_OFF=LOW,
    LIGHT_ON=HIGH
};

MachineStates machineStates;
/* MachineStates machineStates1; */
/* MachineStates machineStates2; */
/* MachineStates machineStates3; */
/* MachineStates machineStates4; */
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

    dynLight.duty_cycle     = 0.5;
    dynLight.cycle_period   = 700;
    dynLight.modelight      = 0;
    dynLight.anti_modelight = 0;
    dynLight.blinker        = 0;
    dynLight.anti_blinker   = 0;

    // Initialize buttons
    button_setup(&leftButton, 5);
    button_setup(&rightButton, 4);

    // Initialize inputs
    sensor_amb = 1;
    sensor_pot = 2;
    pinMode(sensor_pot,INPUT);
    pinMode(sensor_amb,INPUT);

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
    pinMode(leftButton.pin, INPUT);
    pinMode(rightButton.pin, INPUT);

    return;
}

// Main loop
void loop() {
    int time = 0;
    int timep = 0;
    int ts = 0;
    float pot = 0;

    while (1) {
        // Update time
        timep = time;
        time = millis() % CHRONO_RESET;
        ts = (time > timep ? time-timep : time-timep+CHRONO_RESET);

        dynlight_update(&dynLight, ts);
        button_update(&leftButton, ts);
        button_update(&rightButton, ts);

        /* Serial.println(leftButton.currState->state); */
        /* Serial.println(leftButton.currState->state_life); */

        // Get blinking frequency
        // Get states
        // GEN is the main switch, nothing works when it is off anyway
        // BRK
        // HEA
        // AMB
        // LMO
        // dynLight.cycle_period = BL_PER_MIN + BL_POT_FACT*analogRead(sensor_pot);
        /* Serial.println(analogRead(2)); */
        /* Serial.println(lmo_cycle); */
        pot = analogRead(sensor_pot);

        // React on buttons
        /* if ((leftButton.event == BTN_EV_SP) && (rightButton.event != BTN_EV_SP)) { */
        if (leftButton.event == BTN_EV_SP) {
            // Left button single press
            Serial.println("Left button single press");
            leftButton.event  = BTN_EV_NN;
            rightButton.event = BTN_EV_NN;
            if (machineStates.sm_hea == S_HEA_SL) {
                machineStates.sm_hea = S_HEA_FWH;
            } else {
                machineStates.sm_hea = S_HEA_SL;
            }
        } else if (
                (rightButton.event == BTN_EV_SP) && 
                (leftButton.event != BTN_EV_SP)) {
            Serial.println("Right button single press");
            leftButton.event  = BTN_EV_NN;
            rightButton.event = BTN_EV_NN;
            if (machineStates.sm_hea == S_HEA_SR) {
                machineStates.sm_hea = S_HEA_FWH;
            } else {
                machineStates.sm_hea = S_HEA_SR;
            }
        // Delays may be required to be taken into account because buttons
        } else if (
                (leftButton.event == BTN_EV_HL) &&
                (rightButton.event == BTN_EV_HL)) {
            Serial.println("Both buttons held");
            leftButton.event  = BTN_EV_NN;
            rightButton.event = BTN_EV_NN;
            if (machineStates.sm_amb == S_AMB_DAY) {
                machineStates.sm_amb = S_AMB_NIGHT;
            } else {
                machineStates.sm_amb = S_AMB_DAY;
            }
        } else if (
                (leftButton.event == BTN_EV_DP) &&
                (rightButton.event == BTN_EV_DP)) {
            Serial.println("Both buttons double pressed");
            leftButton.event  = BTN_EV_NN;
            rightButton.event = BTN_EV_NN;
            if (machineStates.sm_lmo == S_LMO_BL) {
                machineStates.sm_lmo = S_LMO_ST;
            } else {
                machineStates.sm_lmo = S_LMO_BL;
            }
        }

        // if (dynLight.cycle_period >= (BL_PER_MIN+BL_PER_MAX)/2) {
        /* if (pot/POT1_MAX < 0.25)  { */
        /*     machine_states_copy(&machineStates1, &machineStates); */
            /* machineStates.sm_lmo = S_LMO_BL; */
            /* machineStates.sm_hea = S_HEA_SL; */
            /* machineStates.sm_amb = S_AMB_DAY; */
            /* machineStates.sm_brk = S_BRK_NB; */
        /* } else if (pot/POT1_MAX < 0.50) { */
        /*     machine_states_copy(&machineStates2, &machineStates); */
        /* } else if (pot/POT1_MAX < 0.75) { */
        /*     machine_states_copy(&machineStates3, &machineStates); */
        /* } else if (analogRead(sensor_pot)/POT_MAX < 1) { */
        /* } else { */
        /*     machine_states_copy(&machineStates4, &machineStates); */
            /* machineStates.sm_amb = S_AMB_NIGHT; */
            /* machineStates.sm_lmo = S_LMO_ST; */
            /* machineStates.sm_hea = S_HEA_FWH; */
            /* machineStates.sm_brk = S_BRK_BR; */
        /* } */

        set_lmo(&machineStates, &dynLight);
        set_hl(&machineStates, &dynLight, bl_hl);
        set_fx(&machineStates, &dynLight, S_HEA_SL, bl_fl);
        set_fx(&machineStates, &dynLight, S_HEA_SR, bl_fr);
        set_rx(&machineStates, &dynLight, S_HEA_SL, bl_rl);
        set_rx(&machineStates, &dynLight, S_HEA_SR, bl_rr);
        
        delay(SYSTEM_CYCLE_PERIOD);
    }
    return;
}

// Other functions
void machine_states_configure(
        MachineStates *ms, 
        const int amb,
        const int hea,
        const int brk,
        const int lmo) {
    ms->sm_amb = amb;
    ms->sm_hea = hea;
    ms->sm_brk = brk;
    ms->sm_lmo = lmo;
    return;
}

void machine_states_copy(const MachineStates *src, MachineStates *tgt) {
    tgt->sm_amb = src->sm_amb;
    tgt->sm_hea = src->sm_hea;
    tgt->sm_brk = src->sm_brk;
    tgt->sm_lmo = src->sm_lmo;
    return;
}

void set_hl(const MachineStates *states,const DynLight *dynLight, const int light) {
    if (states->sm_amb == S_AMB_NIGHT) {
        digitalWrite(light,dynLight->modelight);
    } else {
        digitalWrite(light,LOW);
    }
    return;
}

void set_fx(const MachineStates *states, const DynLight *dynLight, const int dir, const int light) {
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
void set_rx(const MachineStates *states, const DynLight *dynLight, const int dir, const int light) {
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

void set_lmo(const MachineStates *states, DynLight *dynLight) {
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

void
button_setup(Button *button,
        const uint8_t pin) {
    button_init_state_memory(button);
    button->id = 0;
    button->pin = pin;
    button->event = BTN_EV_NN;  
    button->currState = &button->memory.state0;
    button->prevState = &button->memory.state1;
}

void
button_init_state_memory(Button *button) {
    button->memory.state0.state      = BTN_ST_NONE;
    button->memory.state1.state      = BTN_ST_NONE;
    button->memory.state2.state      = BTN_ST_NONE;
    button->memory.state3.state      = BTN_ST_NONE;
    button->memory.state0.state_life = 0;
    button->memory.state1.state_life = 0;
    button->memory.state2.state_life = 0;
    button->memory.state3.state_life = 0;
    return;
}

void 
button_update_state(Button *button, 
        const uint8_t state, 
        const uint16_t state_life) 
{
    button->memory.state3            = button->memory.state2;
    button->memory.state2            = button->memory.state1;
    button->memory.state1            = button->memory.state0;
    button->memory.state0.state      = state;
    button->memory.state0.state_life = state_life;
    return;
}

bool 
button_get_doublepressed(Button *button) {
    return ((button->memory.state3.state == BTN_ST_DOWN) && 
            (val_is_within_range(button->memory.state3.state_life,BTN_DEL_MIN,BTN_DEL_HOLD)) &&
            (button->memory.state2.state == BTN_ST_UP) && 
            (val_is_within_range(button->memory.state2.state_life,BTN_DEL_MIN,BTN_DEL_BMAX)) &&
            (button->memory.state1.state == BTN_ST_DOWN) &&
            (val_is_within_range(button->memory.state1.state_life,BTN_DEL_MIN,BTN_DEL_HOLD)));
}

bool
button_get_singlepressed(Button *button) {
    return ((button->memory.state1.state == BTN_ST_DOWN) &&
            (val_is_within_range(button->memory.state1.state_life,BTN_DEL_MIN,BTN_DEL_HOLD)));
}

bool
button_get_held(Button *button) {
    return ((button->memory.state0.state == BTN_ST_DOWN) &&
            (val_is_within_range(button->memory.state0.state_life,BTN_DEL_MIN,BTN_DEL_HOLD)));
}

void
button_interpret_state_memory(Button *button) {
    /* if (button->event == BTN_EV_NN) { */
        if (button_get_doublepressed(button)) {
            button->event = BTN_EV_DP;
            button_clear_button_state_memory(button);
        } else if (button_get_singlepressed(button)) {
            button->event = BTN_EV_SP;
            button_clear_button_state_memory(button);
        } else if (button_get_held(button)) {
            button->event = BTN_EV_HL;
            button_clear_button_state_memory(button);
        } else {
            button->event = BTN_EV_NN;
        }
    /* } else { */
    /*     button->event = BTN_EV_NN; */
    /* } */
    return;
}

void button_update(Button *button, const uint8_t ts) {
    // Update state and its life
    uint8_t read_state = BTN_ST_NONE;
    if (digitalRead(button->pin) == 1) {
        read_state = BTN_ST_UP;
    } else {
        read_state = BTN_ST_DOWN;
    }

    if (read_state == button->currState->state) {
        if (button->currState->state_life+ts < BTN_STATE_LIFE_MAX) {
            button->currState->state_life += ts;
        }
    } else {
        button_update_state(button, read_state, 0);
    }

    // Detect events
    button_interpret_state_memory(button);
    return;
}

bool
val_is_within_range(const int val, const int a, const int b) {
    return ((val >= a) && (val <= b));
}

void
button_clear_button_state_memory(Button *button) {
    button->memory.state0.state = BTN_ST_NONE;
    button->memory.state1.state = BTN_ST_NONE;
    button->memory.state2.state = BTN_ST_NONE;
    button->memory.state3.state = BTN_ST_NONE;
    button->memory.state0.state_life = 0;
    button->memory.state1.state_life = 0;
    button->memory.state2.state_life = 0;
    button->memory.state3.state_life = 0;
    return;
}
