/*
Copyright 2023 QVEX Tech

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

#include "matrix.h"
#include "wait.h"

static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

extern matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
extern matrix_row_t matrix[MATRIX_ROWS];     // debounced values

static void select_row(uint8_t row) {
    gpio_set_pin_output(row_pins[row]);
    gpio_write_Pin_low(row_pins[row]);
}

static void unselect_row(uint8_t row) {
    gpio_set_Pin_input_high(row_pins[row]);
}

static void unselect_rows(void) {
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        gpio_set_Pin_input_high(row_pins[x]);
    }
}

static void init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        gpio_set_Pin_input_high(col_pins[x]);
    }
    gpio_set_Pin_input_high(PIN_JU);
    gpio_set_Pin_input_high(PIN_JD);
    gpio_set_Pin_input_high(PIN_JL);
    gpio_set_Pin_input_high(PIN_JR);
    gpio_set_Pin_input_high(PIN_JC);
    gpio_set_Pin_input_high(PIN_TC);
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    if (current_row > 2) return false;

    matrix_row_t last_row_value = current_matrix[current_row];

    current_matrix[current_row] = 0;

    select_row(current_row);
    wait_us(30);

    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {
        uint8_t pin_state = gpio_read_Pin(col_pins[col_index]);
        current_matrix[current_row] |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
    }

    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}

static bool read_encoder_switches(matrix_row_t current_matrix[]) {
    matrix_row_t last_fourth_row = current_matrix[3];
    matrix_row_t last_fifth_row  = current_matrix[4];

    current_matrix[3] = 0;
    current_matrix[4] = 0;

    current_matrix[4] |= !gpio_read_Pin(PIN_TC) ? (1 << 1) : 0;

    if (!gpio_read_Pin(PIN_JC)) {
        if (!gpio_read_Pin(PIN_JU)) {
            current_matrix[3] |= (1 << 0);
        } else if (!gpio_read_Pin(PIN_JD)) {
            current_matrix[3] |= (1 << 1);
        } else if (!gpio_read_Pin(PIN_JL)) {
            current_matrix[3] |= (1 << 2);
        } else if (!gpio_read_Pin(PIN_JR)) {
            current_matrix[3] |= (1 << 3);
        } else {
            current_matrix[4] |= (1 << 0);
        }
    }

    return last_fourth_row != current_matrix[3] || last_fifth_row != current_matrix[4];
}

void matrix_init_custom(void) {
    init_pins();
}

bool matrix_scan_custom(void) {
    bool changed = false;

    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        changed |= read_cols_on_row(raw_matrix, current_row);
    }

    changed |= read_encoder_switches(raw_matrix);

    return changed;
}
