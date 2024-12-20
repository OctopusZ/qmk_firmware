#include QMK_KEYBOARD_H

enum layers {
  _BASE,
  _FUNC
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BASE] = LAYOUT(
  KC_ESC, KC_GRV,  KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC,
 KC_PGUP, KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
 KC_PGDN, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
          KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, MO(1),
          KC_LCTL,    SC_LAPO, KC_BSPC, KC_LGUI,     KC_SPC,       SC_RAPC,                   KC_RCTL
   ),
[_FUNC] = LAYOUT(
 UG_TOGG, VK_TOGG,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, XXXXXXX, QK_BOOT,
 UG_NEXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
 UG_PREV, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_UP, XXXXXXX,
          KC_LSFT, UG_HUEU, UG_HUED, UG_SATU, UG_SATD, UG_VALU, UG_VALD, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN,KC_RIGHT, XXXXXXX,
          _______,          KC_LALT, _______, _______,          _______,                   KC_RALT,                            _______
   )
};

#ifdef USE_LEDS_FOR_LAYERS
// example of how to use LEDs as layer indicators
static uint8_t top = 1;
static uint8_t middle = 0;
static uint8_t bottom = 0;

layer_state_t layer_state_set_user(layer_state_t state) {
    top = middle = bottom = 0;
    switch (get_highest_layer(state)) {
    case _BASE:
        top = 1;
        break;
    case _FUNC:
        middle = 1;
        break;
    default: //  for any other layers, or the default layer
        break;
    }
  return state;
}

// override kb level function
bool led_update_user(led_t usb_led) {
    gpio_write_Pin(B1, !top);
    gpio_write_Pin(B2, !middle);
    gpio_write_Pin(B3, !bottom);
    return false; // we are using LEDs for something else override kb
}
#endif
