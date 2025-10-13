/* corne_devorak_rp2040: Dvorak layout + OLED + RGB Matrix */
#include QMK_KEYBOARD_H

/* Layers */
enum { _DVORAK = 0U, _NAV, _SYM, _RGB };

/* Custom keycodes for RGB Matrix controls (safe regardless of feature keycode macros) */
enum custom_keycodes {
    D_RGB_TOG = SAFE_RANGE,
    D_RGB_HUI,
    D_RGB_HUD,
    D_RGB_SAI,
    D_RGB_SAD,
    D_RGB_VAI,
    D_RGB_VAD,
    D_RGB_MOD,
    D_RGB_RMOD,
};

/*
PSEUDOCODE GUIDE:
 - Add combos: enable COMBO_ENABLE in rules.mk, create combos.def, and implement process_combo_event
 - Add tap dance: enable TAP_DANCE_ENABLE, define dance actions
 - Per-key tapping term: implement get_tapping_term
 - Custom keycodes: define enum and handle in process_record_user
 - RGB behaviors: tweak layer_state_set_user to update colors per-layer
 - OLED UI: render layer name, WPM, caps lock, logos, and encoder hints
*/

/* Base: Dvorak */
/* Row order matches LAYOUT_split_3x6_3; comment shows Dvorak positions */
/* clang-format off */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DVORAK] = LAYOUT_split_3x6_3(
    /*   '     ,      .      P      Y                               F      G      C      R      L    Bspc */
    KC_TAB,   KC_QUOT,KC_COMM,KC_DOT, KC_P,  KC_Y,                   KC_F,  KC_G,  KC_C,  KC_R,  KC_L, KC_BSPC,
    /*   A      O      E      U      I                               D      H      T      N      S      / */
    KC_LCTL,  KC_A,   KC_O,   KC_E,  KC_U,  KC_I,                    KC_D,  KC_H,  KC_T,  KC_N,  KC_S,  KC_SLSH,
    /*   ;      Q      J      K      X                               B      M      W      V      Z     Esc */
    KC_LSFT,  KC_SCLN,KC_Q,   KC_J,  KC_K,  KC_X,                    KC_B,  KC_M,  KC_W,  KC_V,  KC_Z,  KC_ESC,
                         KC_LGUI, MO(_NAV), KC_SPC, KC_ENT, MO(_SYM), KC_RALT
  ),

  [_NAV] = LAYOUT_split_3x6_3(
    _______, KC_1, KC_2, KC_3, KC_4, KC_5,                           KC_6, KC_7, KC_8, KC_9, KC_0, KC_DEL,
    _______, _______, _______, _______, _______, _______,              KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______, _______,
    _______, _______, _______, _______, _______, _______,              _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, MO(_RGB), _______
  ),

  [_SYM] = LAYOUT_split_3x6_3(
    _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,             KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
    _______, _______, _______,  _______, _______, _______,              KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
    _______, _______, _______,  _______, _______, _______,              KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
                         _______, MO(_RGB), _______, _______, _______, _______
  ),

  [_RGB] = LAYOUT_split_3x6_3(
    QK_BOOT, D_RGB_TOG, D_RGB_HUI, D_RGB_SAI, D_RGB_VAI, D_RGB_MOD,   _______, _______, _______, _______, _______, _______,
    _______, D_RGB_RMOD,D_RGB_HUD, D_RGB_SAD, D_RGB_VAD, _______,     _______, _______, _______, _______, _______, _______,
    _______, _______,    _______,    _______,    _______,    _______,  _______, _______, _______, _______, _______, _______,
                         _______, _______, _______, _______, _______, _______
  )
};
/* clang-format on */

/* Optional: color-code layers with RGB Matrix */
layer_state_t layer_state_set_user(layer_state_t state) {
#ifdef RGB_MATRIX_ENABLE
    /* Only set a suggested effect/color when entering the RGB layer so
     * user-chosen modes persist on other layers and across reboots.
     */
    if (layer_state_cmp(state, _RGB)) {
        rgb_matrix_mode(RGB_MATRIX_BREATHING); /* persisted */
        rgb_matrix_sethsv(HSV_YELLOW);         /* persisted */
    }
#endif
    return state;
}

/* OLED UI: show side, layer, WPM */
#ifdef OLED_ENABLE
static void render_layer(void) {
    oled_write_ln_P(PSTR("Layer:"), false);
    if (layer_state_cmp(layer_state, _RGB)) {
        oled_write_ln_P(PSTR("RGB"), false);
        return;
    }
    if (layer_state_cmp(layer_state, _SYM)) {
        oled_write_ln_P(PSTR("SYM"), false);
        return;
    }
    if (layer_state_cmp(layer_state, _NAV)) {
        oled_write_ln_P(PSTR("NAV"), false);
        return;
    }
    oled_write_ln_P(PSTR("BASE"), false);
}

bool oled_task_user(void) {
    oled_clear();
    if (is_keyboard_master()) {
        oled_write_ln_P(PSTR("Corne Dv"), false);
        render_layer();
        char wpm_str[16];
        snprintf(wpm_str, sizeof(wpm_str), "WPM:%3u", get_current_wpm());
        oled_write_ln(wpm_str, false);
        /* PSEUDOCODE: show caps/num lock */
        /* oled_write_ln(host_keyboard_led_state().caps_lock ? "CAPS" : "caps", false); */
    } else {
        oled_write_ln_P(PSTR("Slave"), false);
        /* PSEUDOCODE: draw logo or animation */
        /* oled_write_ln_P(PSTR("<logo>"), false); */
    }
    return false;
}
#endif

/* Handle custom RGB Matrix keycodes */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RGB_MATRIX_ENABLE
    if (record->event.pressed) {
        switch (keycode) {
            case D_RGB_TOG:
                rgb_matrix_toggle();
                return false;
            case D_RGB_HUI:
                rgb_matrix_increase_hue();
                return false;
            case D_RGB_HUD:
                rgb_matrix_decrease_hue();
                return false;
            case D_RGB_SAI:
                rgb_matrix_increase_sat();
                return false;
            case D_RGB_SAD:
                rgb_matrix_decrease_sat();
                return false;
            case D_RGB_VAI:
                rgb_matrix_increase_val();
                return false;
            case D_RGB_VAD:
                rgb_matrix_decrease_val();
                return false;
            case D_RGB_MOD:
                rgb_matrix_step();
                return false;
            case D_RGB_RMOD:
                rgb_matrix_step_reverse();
                return false;
            default:
                break;
        }
    }
#endif
    return true;
}
