/* Copyright 2021 kuenhlee, Don Kjer, Tyler Tidman
 * Copyright 2021 Simon Arlott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "k310.h"
#include <ch.h>
#include <hal.h>

/* Private Functions */
void off_all_leds(void) {
#ifdef LED_NUM_LOCK_PIN
    gpio_write_Pin_high(LED_NUM_LOCK_PIN);
#endif
    gpio_write_Pin_high(LED_CAPS_LOCK_PIN);
    gpio_write_Pin_high(LED_SCROLL_LOCK_PIN);
    gpio_write_Pin_high(LED_WIN_LOCK_PIN);
    gpio_write_Pin_high(LED_MR_LOCK_PIN);
}

void on_all_leds(void) {
#ifdef LED_NUM_LOCK_PIN
    gpio_write_Pin_low(LED_NUM_LOCK_PIN);
#endif
    gpio_write_Pin_low(LED_CAPS_LOCK_PIN);
    gpio_write_Pin_low(LED_SCROLL_LOCK_PIN);
    gpio_write_Pin_low(LED_WIN_LOCK_PIN);
    gpio_write_Pin_low(LED_MR_LOCK_PIN);
}

/* WinLock and MR LEDs are non-standard. Need to override led init */
void led_init_ports(void) {
#ifdef LED_NUM_LOCK_PIN
    gpio_set_pin_output(LED_NUM_LOCK_PIN);
#endif
    gpio_set_pin_output(LED_CAPS_LOCK_PIN);
    gpio_set_pin_output(LED_SCROLL_LOCK_PIN);
    gpio_set_pin_output(LED_WIN_LOCK_PIN);
    gpio_set_pin_output(LED_MR_LOCK_PIN);
    off_all_leds();
}

#ifndef WINLOCK_DISABLED
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case GU_TOGG:
            if (record->event.pressed) {
                // Toggle LED on key press
                gpio_toggle_Pin(LED_WIN_LOCK_PIN);
            }
            break;
    }
    return process_record_user(keycode, record);
}
#endif /* WINLOCK_DISABLED */

#ifndef HW_RESET_PIN_DISABLED
static void hardware_reset_cb(void *arg) {
    chSysLockFromISR();
    bootloader_jump();
    chSysUnlockFromISR();
}
#endif

void keyboard_pre_init_kb(void) {
    gpio_set_Pin_input_high(HARDWARE_RESET_PIN);

#ifndef HW_RESET_PIN_DISABLED
    /* Jump to bootloader when the hardware reset button is pressed */
    palEnablePadEvent(PAL_PORT(HARDWARE_RESET_PIN), PAL_PAD(HARDWARE_RESET_PIN), PAL_EVENT_MODE_FALLING_EDGE);
    palSetPadCallback(PAL_PORT(HARDWARE_RESET_PIN), PAL_PAD(HARDWARE_RESET_PIN), hardware_reset_cb, NULL);

    /* The interrupt is edge-triggered so check that it's not already pressed */
    if (!gpio_read_Pin(HARDWARE_RESET_PIN)) {
        bootloader_jump();
    }
#endif

    keyboard_pre_init_user();
}
