/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
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

#include "tractyl_manuform.h"

void keyboard_pre_init_sub(void) { gpio_set_Pin_input_high(A0); }

void matrix_scan_sub_kb(void) {
    if (!gpio_read_Pin(A0)) {
        reset_keyboard();
    }
}

__attribute__((weak)) void bootmagic_scan(void) {
    // We need multiple scans because debouncing can't be turned off.
    matrix_scan();
#if defined(DEBOUNCE) && DEBOUNCE > 0
    wait_ms(DEBOUNCE * 2);
#else
    wait_ms(30);
#endif
    matrix_scan();

    uint8_t row = BOOTMAGIC_ROW;
    uint8_t col = BOOTMAGIC_COLUMN;

#if defined(SPLIT_KEYBOARD) && defined(BOOTMAGIC_ROW_RIGHT) && defined(BOOTMAGIC_COLUMN_RIGHT)
    if (!is_keyboard_left()) {
        row = BOOTMAGIC_ROW_RIGHT;
        col = BOOTMAGIC_COLUMN_RIGHT;
    }
#endif

    if (matrix_get_row(row) & (1 << col) || !gpio_read_Pin(A0)) {
        eeconfig_disable();
        bootloader_jump();
    }
}


#ifdef USB_VBUS_PIN
bool usb_vbus_state(void) {
    gpio_set_Pin_input_low(USB_VBUS_PIN);
    wait_us(5);
    return gpio_read_Pin(USB_VBUS_PIN);
}
#endif

void matrix_output_unselect_delay(uint8_t line, bool key_pressed) {
    for (int32_t i = 0; i < 40; i++) {
        __asm__ volatile("nop" ::: "memory");
    }
}
