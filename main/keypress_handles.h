/**
 * @author      : chedim (chedim@couchbaser)
 * @file        : keypress_handles
 * @created     : Thursday Jan 27, 2022 03:48:56 EST
 */

#ifndef KEYPRESS_HANDLES_H

#define KEYPRESS_HANDLES_H
#include <stdlib.h>
#include "keyboard_config.h"

extern uint8_t current_report[REPORT_LEN];
typedef void (KEY_HANDLER)(uint8_t pressed, uint8_t changed);

uint8_t check_modifier(uint16_t key);
void set_key(uint8_t key);
void unset_key(uint8_t key);
uint8_t get_mods();
void add_mods(uint8_t mods);
void rm_mods(uint8_t mods);
uint8_t get_report_size();

uint8_t get_layer();
void push_layer(uint8_t layer);
void pop_layer();
void set_layer(uint8_t layer);



#endif /* end of include guard KEYPRESS_HANDLES_H */

