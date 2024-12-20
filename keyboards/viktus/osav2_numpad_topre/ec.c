/* Copyright 2023 Viktus Design LLC
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

#include "ec.h"
#include <avr/interrupt.h>
#include "analog.h"
//#include "debug.h"  // needed for debugging

// sensing channel definitions
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7

// analog connection settings
#define DISCHARGE_PIN B5
#define ANALOG_PORT B6

#ifndef MUX_SEL_PIN
#   define MUX_SEL_PINS \
        { D6, D7, B4 }
#endif

// pin connections
const uint8_t row_channels[] = MATRIX_ROW_PINS;
const uint8_t col_pins[]     = MATRIX_COL_PINS;
const uint8_t mux_sel_pins[] = MUX_SEL_PINS;

_Static_assert(sizeof(mux_sel_pins) == 3, "invalid MUX_SEL_PINS");

static ec_config_t config;
static uint16_t    ec_sw_value[MATRIX_COLS][MATRIX_ROWS];

static inline void discharge_capacitor(void) { gpio_set_pin_output(DISCHARGE_PIN); }
static inline void charge_capacitor(uint8_t col) {
    gpio_set_Pin_input(DISCHARGE_PIN);
    gpio_write_Pin_high(col_pins[col]);
}

static inline void clear_all_col_pins(void) {
    for (int col = 0; col < sizeof(col_pins); col++) {
        gpio_write_Pin_low(col_pins[col]);
    }
}

void init_mux_sel(void) {
    for (int idx = 0; idx < sizeof(mux_sel_pins); idx++) {
        gpio_set_pin_output(mux_sel_pins[idx]);
    }
}

void select_mux(uint8_t row) {
    uint8_t ch = row_channels[row];
    gpio_write_Pin(mux_sel_pins[0], ch & 1);
    gpio_write_Pin(mux_sel_pins[1], ch & 2);
    gpio_write_Pin(mux_sel_pins[2], ch & 4);
}

void init_col(void) {
    for (int idx = 0; idx < sizeof(col_pins); idx++) {
        gpio_set_pin_output(col_pins[idx]);
        gpio_write_Pin_low(col_pins[idx]);
    }
}

void ec_init(ec_config_t const* const ec_config) {
    // save config
    config = *ec_config;

    // initialize discharge pin as discharge mode
    gpio_write_Pin_low(DISCHARGE_PIN);
    gpio_set_pin_output(DISCHARGE_PIN);

    // set analog reference
    analogReference(ADC_REF_POWER);

    // initialize drive lines
    init_col();

    // initialize multiplexer select pin
    init_mux_sel();

    // set discharge pin to charge mode
    gpio_set_Pin_input(DISCHARGE_PIN);
}

uint16_t ec_readkey_raw(uint8_t col, uint8_t row) {
    uint16_t sw_value = 0;

    discharge_capacitor();

    select_mux(row);

    clear_all_col_pins();

    cli();

    charge_capacitor(col);

    sw_value = analogReadPin(ANALOG_PORT);

    sei();

    return sw_value;
}

bool ec_update_key(matrix_row_t* current_row, matrix_row_t col, uint16_t sw_value, uint16_t reset_pt, uint16_t actuation_pt) {
    bool current_state = (*current_row >> col) & 1;

    // press to release
    if (current_state && sw_value < reset_pt) {
        *current_row &= ~(MATRIX_ROW_SHIFTER << col);
        return true;
    }

    // rest to press
    if ((!current_state) && sw_value > actuation_pt) {
        *current_row |= (MATRIX_ROW_SHIFTER << col);
        return true;
    }

    return false;
}

bool ec_matrix_scan(matrix_row_t current_matrix[]) {
    bool updated = false;

    for (int row = 0; row < sizeof(row_channels); row++) {
        for (int col = 0; col < sizeof(col_pins); col++) {
            uint16_t reset_pt = config.reset_pt;
            uint16_t actuation_pt = config.actuation_pt;

            //Modifying threshold values for overlapping pads
            switch(row) {
                case 1:
                case 2:
                case 3:
                case 4:
                    switch(col) {
                        case 3: // lower threshold for plus and enter: (37 rest, 61 btm)
                            reset_pt = 45;
                            actuation_pt = 50;
                            break;
                    }
                    break;
            }

            ec_sw_value[col][row] = ec_readkey_raw(col, row);
            updated |= ec_update_key(&current_matrix[row], col, ec_sw_value[col][row], reset_pt, actuation_pt);
        }
    }

    return updated;
}

// console debugging for pad values
/*void ec_dprint_matrix(void) {
    for (int row = 0; row < sizeof(row_channels); row++) {
        for (int col = 0; col < sizeof(col_pins); col++) {
            dprintf("%5d", ec_sw_value[col][row]);
        }
        dprintf("\n");
    }
}*/
