#ifndef KEY_DEFINITIONS_H
#define KEY_DEFINITIONS_H

#include "usb_hid_keys.h"
#include "keypress_handles.h"
#include "esp_log.h"

// USB HID Keyboard/Keypad Usage(0x07)

#define KC(NAME) \
  void NAME(uint8_t pressed, uint8_t changed);

#define KCI(NAME, CODE) \
  void NAME(uint8_t pressed, uint8_t changed) { \
    if (changed) { \
      if (pressed) { \
        tapCount++; \
        ESP_LOGI("KEY_PRESS", "DOWN: %s", #NAME); \
        set_key(CODE); \
      } else { \
        ESP_LOGI("KEY_PRESS", "  UP: %s", #NAME); \
        unset_key(CODE); \
      } \
    } \
  } 

#define MOD(NAME, CODE) \
  void NAME(uint8_t pressed, uint8_t changed) { \
    if (changed) { \
      if (pressed) { \
        ESP_LOGI("KEY_PRESS", "DOWN: %s", #NAME); \
        add_mods(CODE); \
      } else { \
        ESP_LOGI("KEY_PRESS", "  UP: %s", #NAME); \
        rm_mods(CODE); \
      } \
    } \
  }

#define MKC(NAME, MOD, KEY) \
  void NAME(uint8_t pressed, uint8_t changed) { \
    if (pressed || changed) { \
      add_mods(MOD); \
      KEY(pressed, changed); \
      rm_mods(MOD); \
    } \
  } 

#define LSFT(NAME, KEY) MKC(NAME, KEY_MOD_LSHIFT, KEY)
#define RSFT(NAME, KEY) MKC(NAME, KEY_MOD_RSHIFT, KEY)


extern KEY_HANDLER *tapCodes[TAP_HOLD_BUFFER];
extern uint32_t holdStart[TAP_HOLD_BUFFER];
extern uint8_t sentHelds[TAP_HOLD_BUFFER];
extern uint8_t tapCount;
int8_t findTapCode(KEY_HANDLER *tap);

#define MILLIS esp_timer_get_time() / 1000

#define TH(NAME, TAP, HOLD) \
  void NAME(uint8_t pressed, uint8_t changed) { \
    int8_t idx = 0; \
    if (pressed) { \
      idx = findTapCode(TAP); \
      uint32_t tapInterval = MILLIS - holdStart[idx]; \
      if (idx == -1) { \
        idx = findTapCode(0); \
        tapCodes[idx] = TAP; \
        holdStart[idx] = MILLIS; \
        sentHelds[idx] = 0; \
        tapCount = 0; \
        ESP_LOGI("KEYDEF", "%s hold init %s at %p[%d]: %p %d", #NAME, #HOLD, tapCodes, idx, tapCodes[idx], holdStart[idx]); \
      } else if ((tapInterval > TAP_HOLD_THRESHOLD)) { \
        /* setting modifiers */ \
        if (!sentHelds[idx]) { \
          ESP_LOGI("KEYDEF", "%s hold start %s at %d", #NAME, #HOLD, idx); \
          sentHelds[idx] = 1; \
          HOLD(pressed, true); \
        } else { \
          /* ESP_LOGI("KEYDEF", "hold continue %p", hold); */ \
          HOLD(pressed, false); \
        } \
      } \
    } else if (changed) {  \
      idx = findTapCode(TAP); \
      uint32_t tapInterval = MILLIS - holdStart[idx]; \
      if (idx == -1) { \
        ESP_LOGE("KEYDEF", "%s RELEASED NON-REGISTERED TAP: %s (%p)", #NAME, #TAP, TAP); \
      } \
      if (tapInterval < TAP_HOLD_THRESHOLD * 4 && !tapCount) { \
        ESP_LOGI("KEYDEF", "%s HOLD RESET: no taps in TAP_HOLD_THRESHOLD*4", #NAME); \
        tapInterval = TAP_HOLD_THRESHOLD - 1; \
        if (sentHelds[idx]) { \
          HOLD(false, true); \
        } \
      } \
      \
      if (tapInterval < TAP_HOLD_THRESHOLD) { \
        ESP_LOGI("KEYDEF", "%s tapped %s at %d", #NAME, #TAP, idx); \
        tapCodes[idx] = 0; \
        TAP(true, true); \
        TAP(false, true); \
      } else { \
        ESP_LOGI("KEYDEF", "%s held %s at %d", #NAME, #HOLD, idx); \
        if (!sentHelds[idx]) { \
          HOLD(true, true); \
        } \
        HOLD(false, true); \
        tapCodes[idx] = 0; \
      } \
    } \
  }

#define LT(NAME, LAYER) \
  void NAME(uint8_t pressed, uint8_t changed) { \
    if (changed) { \
      if (pressed) { \
        push_layer(LAYER); \
      } else { \
        pop_layer(); \
      } \
    } \
  } 

void KC_GESC(uint8_t pressed, uint8_t modified);
void KC_NO(uint8_t pressed, uint8_t modified);


KC(KC_A);
KC(KC_B);
KC(KC_C);
KC(KC_D);
KC(KC_E);
KC(KC_F);
KC(KC_G);
KC(KC_H);
KC(KC_I);
KC(KC_J);
KC(KC_K);
KC(KC_L);
KC(KC_M);
KC(KC_N);
KC(KC_O);
KC(KC_P);
KC(KC_Q);
KC(KC_R);
KC(KC_S);
KC(KC_T);
KC(KC_U);
KC(KC_V);
KC(KC_W);
KC(KC_X);
KC(KC_Y);
KC(KC_Z);
KC(KC_1);
KC(KC_2);
KC(KC_3);
KC(KC_4);
KC(KC_5);
KC(KC_6);
KC(KC_7);
KC(KC_8);
KC(KC_9);
KC(KC_0);
KC(KC_ENTER);
//#define KC_ENTER  KC(KEY_ENTER);
KC(KC_ESCAPE);
KC(KC_BSPACE);
KC(KC_TAB);
KC(KC_SPACE);
KC(KC_MINUS);
KC(KC_EQUAL);
KC(KC_LBRACKET);
KC(KC_RBRACKET);
KC(KC_BSLASH);
KC(KC_NONUS_HASH);
KC(KC_SCOLON);
KC(KC_APOSTROPHE);
#define KC_APST KC_APOSTROPHE
KC(KC_GRAVE);
KC(KC_COMMA);
KC(KC_DOT);
KC(KC_SLASH);
KC(KC_CAPSLOCK);
KC(KC_F1);
KC(KC_F2);
KC(KC_F3);
KC(KC_F4);
KC(KC_F5);
KC(KC_F6);
KC(KC_F7);
KC(KC_F8);
KC(KC_F9);
KC(KC_F10);
KC(KC_F11);
KC(KC_F12);
KC(KC_PSCREEN);
KC(KC_SCROLLLOCK);
KC(KC_PAUSE);
KC(KC_INSERT);
KC(KC_HOME);
KC(KC_PGUP);
KC(KC_DELETE);
KC(KC_END);
KC(KC_PGDOWN);
KC(KC_RIGHT);
KC(KC_LEFT);
KC(KC_DOWN);
KC(KC_UP);
KC(KC_NUMLOCK);
KC(KC_KP_SLASH);
KC(KC_KP_ASTERISK);
KC(KC_KP_MINUS);
KC(KC_KP_PLUS);
KC(KC_KP_ENTER);
KC(KC_KP_1);
KC(KC_KP_2);
KC(KC_KP_3);
KC(KC_KP_4);
KC(KC_KP_5);
KC(KC_KP_6);
KC(KC_KP_7);
KC(KC_KP_8);
KC(KC_KP_9);
KC(KC_KP_0);
KC(KC_KP_DOT);
KC(KC_NONUS_BSLASH);
KC(KC_APPLICATION);
KC(KC_POWER);
KC(KC_KP_EQUAL);
KC(KC_F13);
KC(KC_F14);
KC(KC_F15);
KC(KC_F16);
KC(KC_F17);
KC(KC_F18);
KC(KC_F19);
KC(KC_F20);
KC(KC_F21);
KC(KC_F22);
KC(KC_F23);
KC(KC_F24);
KC(KC_EXECUTE);
KC(KC_HELP);
KC(KC_MENU);
KC(KC_SELECT);
KC(KC_STOP);
KC(KC_AGAIN);
KC(KC_UNDO);
KC(KC_CUT);
KC(KC_COPY);
KC(KC_PASTE);
KC(KC_FIND);
KC(KC__MUTE);
KC(KC__VOLUP);
KC(KC__VOLDOWN);
KC(KC_LOCKING_CAPS);
KC(KC_LOCKING_NUM);
KC(KC_LOCKING_SCROLL);
KC(KC_KP_COMMA);
KC(KC_KP_EQUAL_AS400);
KC(KC_INT1);
KC(KC_INT2);
KC(KC_INT3);
KC(KC_INT4);
KC(KC_INT5);
KC(KC_INT6);
KC(KC_INT7);
KC(KC_INT8);
KC(KC_INT9);
KC(KC_LANG1);
KC(KC_LANG2);
KC(KC_LANG3);
KC(KC_LANG4);
KC(KC_LANG5);
KC(KC_LANG6);
KC(KC_LANG7);
KC(KC_LANG8);
KC(KC_LANG9);
KC(KC_ALT_ERASE);
KC(KC_SYSREQ);
KC(KC_CANCEL);
KC(KC_CLEAR);
KC(KC_PRIOR);
KC(KC_RETURN);
KC(KC_SEPARATOR);
KC(KC_OUT);
KC(KC_OPER);
KC(KC_CLEAR_AGAIN);
KC(KC_CRSEL);
KC(KC_EXSEL);
KC(KC_KP_00);
KC(KC_KP_000);
KC(KC_THOUSANDS_SEPARATOR);
KC(KC_DECIMAL_SEPARATOR);
KC(KC_CURRENCY_UNIT);
KC(KC_CURRENCY_SUB_UNIT);
KC(KC_KP_LPAREN);
KC(KC_KP_RPAREN);
KC(KC_LPAREN);
KC(KC_RPAREN);
KC(KC_KP_LCBRACKET);
KC(KC_KP_RCBRACKET);
KC(KC_KP_TAB);
KC(KC_KP_BSPACE);
KC(KC_KP_A);
KC(KC_KP_B);
KC(KC_KP_C);
KC(KC_KP_D);
KC(KC_KP_E);
KC(KC_KP_F);
KC(KC_KP_XOR);
KC(KC_KP_HAT);
KC(KC_KP_PERC);
KC(KC_KP_LT);
KC(KC_KP_GT);
KC(KC_KP_AND);
KC(KC_KP_LAZYAND);
KC(KC_KP_OR);
KC(KC_KP_LAZYOR);
KC(KC_KP_COLON);
KC(KC_KP_HASH);
KC(KC_KP_SPACE);
KC(KC_KP_ATMARK);
KC(KC_KP_EXCLAMATION);
KC(KC_KP_MEM_STORE);
KC(KC_KP_MEM_RECALL);
KC(KC_KP_MEM_CLEAR);
KC(KC_KP_MEM_ADD);
KC(KC_KP_MEM_SUB);
KC(KC_KP_MEM_MUL);
KC(KC_KP_MEM_DIV);
KC(KC_KP_PLUS_MINUS);
KC(KC_KP_CLEAR);
KC(KC_KP_CLEAR_ENTRY);
KC(KC_KP_BINARY);
KC(KC_KP_OCTAL);
KC(KC_KP_DECIMAL);
KC(KC_KP_HEXADECIMAL);
KC(KC_LCTRL);
KC(KC_LSHIFT);
KC(KC_LALT);
KC(KC_LGUI);
KC(KC_RCTRL);
KC(KC_RSHIFT);
KC(KC_RALT);
KC(KC_RGUI);
KC(KC_SYSTEM_POWER);
KC(KC_SYSTEM_SLEEP);
KC(KC_SYSTEM_WAKE);


#define KC_ZHTG  KC_GRV 
#define KC_YEN   KC_INT3
#define KC_LBRC  KC_RBRC
#define KC_RBRC  KC_NUHS
#define KC_BSLS  KC_INT1
#define KC_MHEN  KC_INT5
#define KC_HENK  KC_INT4
#define KC_KANA  KC_INT2


#define SHIFT_MASK 		0x0040
#define ALTGR_MASK 		0x0080

#define KC_LCTL  KC_LCTRL
#define KC_RCTL  KC_RCTRL
#define KC_LSFT  KC_LSHIFT
#define KC_RSFT  KC_RSHIFT
#define KC_ESC   KC_ESCAPE
#define KC_BSPC  KC_BSPACE
#define KC_ENT   KC_ENTER
#define KC_DEL   KC_DELETE
#define KC_INS   KC_INSERT
#define KC_CAPS  KC_CAPSLOCK
#define KC_CLCK  KC_CAPSLOCK
#define KC_RGHT  KC_RIGHT
#define KC_PGDN  KC_PGDOWN
#define KC_PSCR  KC_PSCREEN
#define KC_SLCK  KC_SCROLLLOCK
#define KC_PAUS  KC_PAUSE
#define KC_BRK   KC_PAUSE
#define KC_NLCK  KC_NUMLOCK
#define KC_SPC   KC_SPACE
#define KC_MINS  KC_MINUS
#define KC_EQL   KC_EQUAL
#define KC_GRV   KC_GRAVE
#define KC_RBRC  KC_RBRACKET
#define KC_LBRC  KC_LBRACKET
#define KC_COMM  KC_COMMA
#define KC_BSLS  KC_BSLASH
#define KC_SLSH  KC_SLASH
#define KC_SCLN  KC_SCOLON
#define KC_APP   KC_APPLICATION
#define KC_NUHS  KC_NONUS_HASH
#define KC_NUBS  KC_NONUS_BSLASH
#define KC_LCAP  KC_LOCKING_CAPS
#define KC_LNUM  KC_LOCKING_NUM
#define KC_LSCR  KC_LOCKING_SCROLL
#define KC_ERAS  KC_ALT_ERASE
#define KC_CLR   KC_CLEAR
/* Japanese specific */
#define KC_ZKHK  KC_GRAVE
#define KC_RO    KC_INT1
#define KC_KANA  KC_INT2
#define KC_JYEN  KC_INT3
#define KC_HENK  KC_INT4
#define KC_MHEN  KC_INT5
/* Korean specific */
#define KC_HAEN  KC_LANG1
#define KC_HANJ  KC_LANG2
/* Keypad */
#define KC_P1    KC_KP_1
#define KC_P2    KC_KP_2
#define KC_P3    KC_KP_3
#define KC_P4    KC_KP_4
#define KC_P5    KC_KP_5
#define KC_P6    KC_KP_6
#define KC_P7    KC_KP_7
#define KC_P8    KC_KP_8
#define KC_P9    KC_KP_9
#define KC_P0    KC_KP_0
#define KC_PDOT  KC_KP_DOT
#define KC_PCMM  KC_KP_COMMA
#define KC_PSLS  KC_KP_SLASH
#define KC_PAST  KC_KP_ASTERISK
#define KC_PMNS  KC_KP_MINUS
#define KC_PPLS  KC_KP_PLUS
#define KC_PEQL  KC_KP_EQUAL
#define KC_PENT  KC_KP_ENTER
/* Unix function key */
#define KC_EXEC  KC_EXECUTE
#define KC_SLCT  KC_SELECT
#define KC_AGIN  KC_AGAIN
#define KC_PSTE  KC_PASTE
/* Mousekey */
#define KC_MS_U  KC_MS_UP
#define KC_MS_D  KC_MS_DOWN
#define KC_MS_L  KC_MS_LEFT
#define KC_MS_R  KC_MS_RIGHT
#define KC_BTN1  KC_MS_BTN1
#define KC_BTN2  KC_MS_BTN2
#define KC_BTN3  KC_MS_BTN3
#define KC_BTN4  KC_MS_BTN4
#define KC_BTN5  KC_MS_BTN5
#define KC_WH_U  KC_MS_WH_UP
#define KC_WH_D  KC_MS_WH_DOWN
#define KC_WH_L  KC_MS_WH_LEFT
#define KC_WH_R  KC_MS_WH_RIGHT
#define KC_ACL0  KC_MS_ACCEL0
#define KC_ACL1  KC_MS_ACCEL1
#define KC_ACL2  KC_MS_ACCEL2
/* Sytem Control */
#define KC_PWR   KC_SYSTEM_POWER
#define KC_SLEP  KC_SYSTEM_SLEEP
#define KC_WAKE  KC_SYSTEM_WAKE
/* Consumer Page */
#define KC_MUTE  KC_AUDIO_MUTE
#define KC_VOLU  KC_AUDIO_VOL_UP
#define KC_VOLD  KC_AUDIO_VOL_DOWN
#define KC_MNXT  KC_MEDIA_NEXT_TRACK
#define KC_MPRV  KC_MEDIA_PREV_TRACK
#define KC_MFFD  KC_MEDIA_FAST_FORWARD
#define KC_MRWD  KC_MEDIA_REWIND
#define KC_MSTP  KC_MEDIA_STOP
#define KC_MPLY  KC_MEDIA_PLAY_PAUSE
#define KC_MSEL  KC_MEDIA_SELECT
#define KC_EJCT  KC_MEDIA_EJECT
#define KC_MAIL  KC_MAIL
#define KC_CALC  KC_CALCULATOR
#define KC_MYCM  KC_MY_COMPUTER
#define KC_WSCH  KC_WWW_SEARCH
#define KC_WHOM  KC_WWW_HOME
#define KC_WBAK  KC_WWW_BACK
#define KC_WFWD  KC_WWW_FORWARD
#define KC_WSTP  KC_WWW_STOP
#define KC_WREF  KC_WWW_REFRESH
#define KC_WFAV  KC_WWW_FAVORITES
/* Transparent */
#define _______  KC_TRNS
#define ______  KC_TRNS
#define _____  KC_TRNS
#define XXXXXXX  KC_NO
#define XXXXX  KC_NO
#define KC_TRNS  KC_TRANSPARENT
/* GUI key aliases */
#define KC_LCMD  KC_LGUI
#define KC_LWIN  KC_LGUI
#define KC_RCMD  KC_RGUI
#define KC_RWIN  KC_RGUI

/* Misc key aliases*/
#define KC_NONE  KC_UNDEFINED

#define KC_TRANSPARENT NULL

#define MODS 0

#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LMETA  0x08
#define KEY_MOD_RCTRL  0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT   0x40
#define KEY_MOD_RMETA  0x80

#define HID_MASK_LCTL 0x01
#define HID_MASK_LSFT 0x02
#define HID_MASK_LALT 0x04
#define HID_MASK_LGUI 0x08
#define HID_MASK_RCTL 0x10
#define HID_MASK_RSFT 0x20
#define HID_MASK_RALT 0x40
#define HID_MASK_RGUI 0x80

//Aliases for shifted symbols
KC(KC_EXLM);
KC(KC_DQT);
KC(KC_AT);
KC(KC_HASH);
KC(KC_DLR);
KC(KC_PERC);
KC(KC_CIRC);
KC(KC_AMPR);
KC(KC_QUOT);
KC(KC_ASTR);
KC(KC_LPRN);
KC(KC_RPRN);
KC(KC_PLUS);
KC(KC_PIPE);
KC(KC_LCBR);
KC(KC_RCBR);
KC(KC_UNDS);

KC(KC_TILD);

#define KC_QUOTE KC_QUOT

#endif
