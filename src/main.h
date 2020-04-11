// Project:       kvinlumo
// Project home:  https://github.com/wolterhv/kvinlumo
// License:       See <TOPLEVEL>/LICENSE.txt
// Authors:       See <TOPLEVEL>/AUTHORS.txt

#ifndef H_MAIN
#define H_MAIN

bool val_is_within_range(const int val, 
                         const int a,
                         const int b);

typedef struct _GestureHandler {
        uint8_t state_rbutton;
        uint8_t state_lbutton;
        uint8_t state;
        uint16_t timer;
        uint16_t timeout_spress;
        uint16_t timeout_lpress;
        uint16_t timeout_dpress;
        void (*callback_left_spressed)(void*);
        void (*callback_left_lpressed)(void*);
        void (*callback_left_dpressed)(void*);
        void (*callback_right_spressed)(void*);
        void (*callback_right_lpressed)(void*);
        void (*callback_right_dpressed)(void*);
        void (*callback_both_spressed)(void*);
        void (*callback_both_lpressed)(void*);
        void (*callback_both_dpressed)(void*);
        void *callback_data;
} GestureHandler;

enum {
        GH_BTN_UNKNOWN,
        GH_BTN_SPRESSED,
        GH_BTN_LPRESSED,
        GH_BTN_DPRESSED
};

void callback_toggle_brk (void *callback_data);

#endif // H_MAIN
