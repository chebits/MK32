/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * Copyright 2018 Gal Zaidenstein.
 */

#ifndef KEYPRESS_HANDLES_C
#define KEYPRESS_HANDLES_C
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "keymap.h"
#include "matrix.h"
#include "hal_ble.h"
#include "oled_tasks.h"
#include "nvs_keymaps.h"
#include "plugin_manager.h"
#include "keyboard_config.h"
#include "key_definitions.h"

#include <stdlib.h>
#include "mk32_main.h"

#define KEY_PRESS_TAG "KEY_PRESS"

/*
 * Current state of the keymap,each cell will hold the location of the key in the key report,
 *if a key is not in the report it will be set to 0
 */

uint8_t KEY_STATE[MATRIX_ROWS][KEYMAP_COLS] = { 0 };
KEY_HANDLER *pushed[MATRIX_ROWS][KEYMAP_COLS] = { 0 };
uint16_t led_status = 0;
uint8_t modifier = 0;
uint16_t keycode = 0;

// Sizing the report for N-key rollover
uint8_t current_report[REPORT_LEN] = { 0 };

// Array to send when releasing macros
uint8_t macro_release[3] = { 0 };

// Flag in order to know when to ignore layer change on layer hold
uint8_t layer_hold_flag = 0;
uint8_t prev_layout = 0;

// checking if a modifier key was pressed
uint8_t check_modifier(uint16_t key) {

	uint8_t cur_mod = 0;
	// these are the modifier keys
	if ((KEY_LEFTCTRL <= key) && (key <= KEY_RIGHTMETA)) {
		cur_mod = (1 << (key - KEY_LEFTCTRL));
		return cur_mod;
	}
	return 0;

}

// checking if a led status key was pressed, returning current led status
uint16_t check_led_status(uint16_t key) {

	switch (key) {
	case KEY_NUMLOCK:
		return 1;

	case KEY_CAPSLOCK:
		return 2;

	case KEY_SCROLLLOCK:
		return 3;

	}
	return 0;
}

int findKeyPosition(uint8_t key) {
  uint8_t offset = 0;
  uint8_t skipped = 0;
  int result = -1;
  for (int i = 2; i < REPORT_LEN; i++) {
    if (current_report[i]) {
      if (skipped > 0) {
        offset += skipped;
        skipped = 0;
      }
      
      if (offset > 0) {
        current_report[i-offset] = current_report[i];
        current_report[i] = 0;
      }

      if (current_report[i - offset] == key) {
        result = i - offset;
      } 
    } else {
      skipped++;
    }
  }
  if (!key) {
    return REPORT_LEN - skipped;
  }

  return result;
}

uint8_t get_report_size() {
  int size = findKeyPosition(0);
  if (size < 0) {
    size = 0;
  }

  return size;
}

void set_key(uint8_t key) {
  int ri = findKeyPosition(key);
  if (ri == -1) {
    ri = findKeyPosition(0);
    current_report[ri] = key;
  }
  ESP_LOGI(KEY_PRESS_TAG, "Set key: %d (position: %d)", key, ri);
  send_report_now();
}

void unset_key(uint8_t key) {
  int ri = findKeyPosition(key);
  if (ri != -1) {
    current_report[ri] = 0;
  }
  ESP_LOGI(KEY_PRESS_TAG, "unSet key: %d (position: %d)", key, ri);
  send_report_now();
}

//used for debouncing
static uint32_t millis() {
	return esp_timer_get_time() / 1000;
}

uint32_t prev_time = 0;

// set current layer
void set_layer(uint8_t layer) {
  current_layout = layer;
}

uint8_t get_layer() {
  return current_layout;
}

uint8_t layer_stack[LAYERS] = {0};
uint8_t layer_stack_top() {
  for (int i = 0; i < LAYERS; i++) {
    if (layer_stack[i] == 0) {
      return i;
    }
  }

  // stack overflow will override the last layer... or should I panic?
  return 4;
}

void push_layer(uint8_t layer) {
  uint8_t top = layer_stack_top();
  layer_stack[top] = current_layout;
  current_layout = layer;
  ESP_LOGI("LAYER", "pushing layer %d to top %d", layer, top);
}

void pop_layer() {
  uint8_t top = layer_stack_top();
  ESP_LOGI("LAYER", "popping layer %d from top %d over %d", layer_stack[top], top, current_layout);
  current_layout = layer_stack[top];
  layer_stack[top] = 0;
}

uint8_t get_mods() {
  return modifier;
}

void add_mods(uint8_t mods) {
  modifier |= mods;
  send_report_now();
}

void rm_mods(uint8_t mods) {
  modifier ^= mods;
  send_report_now();
}

uint8_t matrix_prev_state[MATRIX_ROWS][MATRIX_COLS] = { 0 };

// checking the state of each key in the matrix
uint8_t *check_key_state() {
	scan_matrix();
  uint8_t changeCount = 0;
	for (uint8_t pad = 0; pad < KEYPADS && !changeCount; pad++) {

		uint8_t matrix_state[MATRIX_ROWS][MATRIX_COLS] = { 0 };
		memcpy(matrix_state, matrix_states[pad], sizeof(matrix_state));


		for (uint8_t col = (MATRIX_COLS * pad); col < ((pad + 1) * (MATRIX_COLS)) && !changeCount; col++) {
			for (uint8_t row = 0; row < MATRIX_ROWS && !changeCount; row++) {
				uint8_t pressed = matrix_state[row][col];
        uint8_t changed = pressed != (matrix_prev_state[row][col]);
        uint8_t layer = get_layer();
				KEY_HANDLER *handler = KEYMAP[layer][row][col];
        
        if (handler == NULL) {
          // KC_TRNS :)
          for (int lst = layer_stack_top(); handler == NULL && lst > -1; lst--) {
//            ESP_LOGI(KEY_PRESS_TAG, "%d,%d,%d: TRNS drop to layer %d", layer, row, col, lst);
            handler = KEYMAP[layer_stack[lst]][row][col];
          }
        }

        if (changed) {
          changeCount++;
          ESP_LOGI(KEY_PRESS_TAG, "%d,%d,%d (%p) is now %d", layer, col, row, handler, pressed);
          if (pressed) {
            pushed[row][col] = handler;
          } else if (pushed[row][col] != 0) {
            handler = pushed[row][col];
//            ESP_LOGI(KEY_PRESS_TAG, "Restored pressed handler for %d,%d", row, col);
            pushed[row][col] = 0;
          }
        } else if (pushed[row][col] != 0) {
//          ESP_LOGI(KEY_PRESS_TAG, "Restored pressed handler for %d,%d", row, col);
          handler = pushed[row][col];
        }
        
        handler(pressed, changed);
			}
		}
		memcpy(matrix_prev_state, matrix_state, sizeof(matrix_state));
	}
	current_report[0] = modifier;
	current_report[1] = led_status;
  if (changeCount > 0) {
    ESP_LOGI(KEY_PRESS_TAG, "change count: %d", changeCount);
  }
	return current_report;

}

#endif
