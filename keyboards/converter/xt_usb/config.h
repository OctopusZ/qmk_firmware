/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once


/* key matrix size */
#define MATRIX_ROWS 16  // keycode bit: 3-0
#define MATRIX_COLS 8   // keycode bit: 6-4

/* key combination for command */
#define IS_COMMAND() ( \
    get_mods() == (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT)) || \
    get_mods() == (MOD_BIT(KC_LCTL) | MOD_BIT(KC_RSFT)) \
)

#define XT_CLOCK_PIN D1
#define XT_DATA_PIN D0
#define XT_RST_PIN B7

/* hard reset: low pulse for 500ms and after that HiZ for safety */
#define XT_RESET() do { \
    gpio_write_Pin_low(XT_RST_PIN); \
    gpio_set_pin_output(XT_RST_PIN); \
    wait_ms(500); \
    gpio_set_Pin_input(XT_RST_PIN); \
} while (0)

/* INT1 for falling edge of clock line */
#define XT_INT_INIT() do { \
    EICRA |= ((1 << ISC11) | (0 << ISC10)); \
} while (0)

/* clears flag and enables interrupt */
#define XT_INT_ON() do { \
    EIFR  |= (1 << INTF1); \
    EIMSK |= (1 << INT1); \
} while (0)

#define XT_INT_OFF() do { \
    EIMSK &= ~(1 << INT1); \
} while (0)

#define XT_INT_VECT INT1_vect
