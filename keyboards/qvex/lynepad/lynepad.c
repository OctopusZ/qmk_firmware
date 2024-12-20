/*
Copyright 2020 KemoNine
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
#include "quantum.h"

void keyboard_pre_init_kb(void) {
    // Encoder pins
    gpio_set_Pin_input(PIN_TW_SW);
    gpio_set_Pin_input(PIN_RJ_SW);
    gpio_set_Pin_input(PIN_RJ_DIR_A);
    gpio_set_Pin_input(PIN_RJ_DIR_C);
    gpio_set_Pin_input(PIN_RJ_DIR_B);
    gpio_set_Pin_input(PIN_RJ_DIR_D);

    keyboard_pre_init_user();
}

int16_t enc1Center = 1;
int16_t enc1CenterPrev = 1;
int16_t enc2Center = 1;
int16_t enc2CenterPrev = 1;
int16_t enc2Up = 1;
int16_t enc2UpPrev = 1;
int16_t enc2Down = 1;
int16_t enc2DownPrev = 1;
int16_t enc2Left = 1;
int16_t enc2LeftPrev = 1;
int16_t enc2Right = 1;
int16_t enc2RightPrev = 1;

void matrix_scan_kb(void) {
    enc1CenterPrev = enc1Center;
    enc1Center = gpio_read_Pin(PIN_TW_SW);

    enc2CenterPrev = enc2Center;
    enc2Center = gpio_read_Pin(PIN_RJ_SW);
    enc2UpPrev = enc2Up;
    enc2Up = gpio_read_Pin(PIN_RJ_DIR_A);
    enc2DownPrev = enc2Down;
    enc2Down = gpio_read_Pin(PIN_RJ_DIR_C);
    enc2LeftPrev = enc2Left;
    enc2Left = gpio_read_Pin(PIN_RJ_DIR_B);
    enc2RightPrev = enc2Right;
    enc2Right = gpio_read_Pin(PIN_RJ_DIR_D);

    // Ensure any user customizations are called (for some reason this wasn't happening by default)
    matrix_scan_user();
}
