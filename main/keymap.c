#ifndef KEYMAP_C
#define KEYMAP_C

#include "keyboard_config.h"
#include "key_definitions.h"
#include "keymap.h"
#include "plugins.h"

// A bit different from QMK, default returns you to the first layer, LOWER and raise increase/lower layer by order.
#define QWERTY 0
#define RAISE 1
#define NUM 2

KEY_HANDLER *KEYMAP[LAYERS][MATRIX_ROWS][KEYMAP_COLS];

TH(KC_LCTS, KC_SPACE, KC_LCTRL);
TH(KC_RCEN, KC_ENTER, KC_RCTRL);
TH(KC_LSPC, KC_LPAREN, KC_LSHIFT);
TH(KC_RSPC, KC_RPAREN, KC_RSHIFT);
TH(KC_ZALT, KC_Z, KC_LALT);
TH(KC_XWIN, KC_X, KC_LGUI);
TH(KC_DOTW, KC_DOT, KC_RGUI);
TH(KC_SLAL, KC_SLASH, KC_RALT);
LT(KC_RAISE, RAISE)
LT(KC_NUM, NUM)
TH(KC_BRAI, KC_B, KC_RAISE);
TH(KC_NRAI, KC_N, KC_RAISE);
TH(KC_TNUM, KC_T, KC_NUM);
TH(KC_YNUM, KC_Y, KC_NUM);


KEY_HANDLER *KEYMAP[LAYERS][MATRIX_ROWS][KEYMAP_COLS] = {
  { // QWERTY
    { KC_GESC,    KC_Q,     KC_W,     KC_E,     KC_R,     KC_TNUM,                   KC_YNUM,     KC_U,     KC_I,     KC_O,     KC_P,      KC_BSPC }, 
    { KC_LCTS,    KC_A,     KC_S,     KC_D,     KC_F,     KC_G,                      KC_H,        KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_RCEN }, 
    { KC_LSPC,    KC_ZALT,  KC_XWIN,  KC_C,     KC_V,     KC_BRAI,                   KC_NRAI,     KC_M,     KC_COMM,  KC_DOTW,  KC_SLAL,   KC_RSPC },
  },

  { // RAISE
    { KC_GESC,    KC_GRAVE, KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,                   KC_CIRC,    KC_AMPR,   KC_ASTR,  KC_MINUS, ______,    KC_DEL  },
    { KC_TAB,     KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   XXXXX,                     KC_LEFT,    KC_DOWN,   KC_UP,    KC_RIGHT, KC_APST,   KC_EQL  },
    { KC_LBRC,    XXXXX,    XXXXX,    XXXXX,    XXXXX,    XXXXX,                     KC_UNDS,    KC_PLUS,   KC_LBRC,  KC_RBRC,  KC_BSLS,   KC_RBRC }
  },

  { // NUM
    { KC_GESC,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,                     KC_F6,      KC_F7,     KC_F8,    KC_F9,   KC_F10,     ______   },
    { ______,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5,                      KC_6,       KC_7,      KC_8,     KC_9,    KC_0,       ______   },
    { ______,     KC_F11,   KC_F12,   KC_F13,   KC_F14,   KC_F15,                    ______,     ______,    ______,   ______,  ______,     ______   }
  }
};

uint8_t current_layout = 0;

#endif

