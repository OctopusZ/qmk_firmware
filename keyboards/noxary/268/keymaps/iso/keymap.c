#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _BL 0
#define _FL1 1

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* _BL: Base Layer (Default)
   * ,----------------------------------------------------------------.
   * |Esc | 1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|   BSpc| Grv|
   * |----------------------------------------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|Enter| Del|
   * |------------------------------------------------------.    |----|
   * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|    |PgUp|
   * |----------------------------------------------------------------|
   * |Sft |  \|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift | Up|PgDn|
   * |----------------------------------------------------------------|
   * |Ctrl|Win |Alt |        Space           | Alt|  Fn|▓|Lef|Dow|Rght|
   * `----------------------------------------------------------------'
   */
  [_BL] = LAYOUT_iso(
    KC_ESC,  KC_1,    KC_2,    KC_3,   KC_4,    KC_5,  KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,           KC_BSPC, KC_GRV,
    KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,    KC_T,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,                   KC_DEL,
    KC_CAPS, KC_A,    KC_S,    KC_D,   KC_F,    KC_G,  KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  KC_NUHS,          KC_ENT,  KC_PGUP,
    KC_LSFT, KC_NUBS, KC_Z,    KC_X,   KC_C,    KC_V,  KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,          KC_UP,   KC_PGDN,
    KC_LCTL, KC_LGUI, KC_LALT,                         KC_SPC,                             KC_RALT, MO(_FL1),          KC_LEFT, KC_DOWN, KC_RGHT
  ),

  /* _FL1: Function Layer 1
   * ,----------------------------------------------------------------.
   * |  `| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|   PScr|    |
   * |----------------------------------------------------------------|
   * |     |   |   |   |RST|   |   |   |   |   |   |   |   |     | Ins|
   * |------------------------------------------------------.    |----|
   * |      |   |   |   |   |   |   |   |   |   |   |   |   |    |Home|
   * |----------------------------------------------------------------|
   * |    |   |   |   |   |   |   |   |   |Bl-|Bl+|   |Mute |Vol+| End|
   * |----------------------------------------------------------------|
   * |    |    |    |      BL_Toggle         |    |    |▓|  |Vol-|    |
   * `----------------------------------------------------------------'
   */
  [_FL1] = LAYOUT_iso(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_PSCR, _______,
    _______, _______, _______, _______, QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______,                   KC_INS,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, KC_HOME,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, BL_DOWN, BL_UP,   _______, KC_MUTE,          KC_VOLU, KC_END,
    _______, _______, _______,                            BL_TOGG,                            _______, _______,          _______, KC_VOLD, _______
  ),

};

bool led_update_user(led_t led_state) {
  if (led_state.caps_lock) {
    gpio_set_pin_output(B6);
    gpio_write_Pin_high(B6);
  } else {
    gpio_set_Pin_input(B6);
    gpio_write_Pin_low(B6);
  }
  return false;
}
