/*
Copyright 2017 Damien Broqua <contact@darkou.fr>

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

#include "quantum.h"

inline void dk60_caps_led_on(void) { gpio_write_Pin_high(E6); }
inline void dk60_esc_led_on(void) { gpio_write_Pin_high(F0); }

inline void dk60_caps_led_off(void) { gpio_write_Pin_low(E6); }
inline void dk60_esc_led_off(void) { gpio_write_Pin_low(F0); }

inline void dk60_led_all_on(void) {
    dk60_caps_led_on();
    dk60_esc_led_on();
}

inline void dk60_led_all_off(void) {
    dk60_caps_led_off();
    dk60_esc_led_off();
}
