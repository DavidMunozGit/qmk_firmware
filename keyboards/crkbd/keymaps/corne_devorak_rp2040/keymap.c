/*
 * Module: corne_devorak_rp2040
 * Brief : Dvorak layout with OLED UI + RGB Matrix helpers for crkbd (RP2040)
 * Notes : Structured in sections (types, macros, globals, prototypes, data, functions)
 */

/* ========================================================================== */
/*                                Includes                                    */
/* ========================================================================== */
#include QMK_KEYBOARD_H

/* ========================================================================== */
/*                                Types                                       */
/* ========================================================================== */
/* Keyboard layers */
typedef enum KB_LAYERS_E {
    KB_LYR_DVORAK = 0U, /* Base: Dvorak */
    KB_LYR_NAV,         /* Navigation */
    KB_LYR_SYM,         /* Symbols */
    KB_LYR_RGB,         /* RGB controls */
    KB_LYR_MAX          /* Number of layers */
} KB_LAYERS_E;

/* Custom keycodes (RGB Matrix controls) */
typedef enum custom_keycodes_e {
    D_RGB_TOG = SAFE_RANGE,
    D_RGB_HUI,
    D_RGB_HUD,
    D_RGB_SAI,
    D_RGB_SAD,
    D_RGB_VAI,
    D_RGB_VAD,
    D_RGB_MOD,
    D_RGB_RMOD,
} custom_keycodes_e;

/* ========================================================================== */
/*                          Private Macros/Constants                          */
/* ========================================================================== */
#ifdef OLED_ENABLE
#    define ANIM_COLS        21U  /* approx columns on 128px width with default font */
#    define ANIM_INTERVAL_MS 120U /* pac-man tick interval */
#endif

/* ========================================================================== */
/*                           Module-scope Variables                           */
/* ========================================================================== */
#ifdef OLED_ENABLE
static uint32_t anim_last_ms = 0U;
static uint8_t  pac_pos_master = 0U; /* moves left->right */
static uint8_t  pac_pos_slave  = 0U; /* moves left->right */
static bool     pac_mouth_open = true;
static bool     anim_inited    = false;
static uint8_t  pellets_master[ANIM_COLS];
static uint8_t  pellets_slave[ANIM_COLS];
#endif

/* ========================================================================== */
/*                           Function Prototypes                              */
/* ========================================================================== */
/* QMK hooks */
layer_state_t layer_state_set_user(layer_state_t state);
bool          process_record_user(uint16_t keycode, keyrecord_t* record);
#ifdef OLED_ENABLE
bool          oled_task_user(void);
#endif

/* Internal helpers (OLED) */
#ifdef OLED_ENABLE
static void anim_reset_half(bool is_master);
static void anim_clear_line(uint8_t row);
static void render_pacman_line(bool is_master);
static void render_layer(void);
#endif

/* ========================================================================== */
/*                              Keymap Layers                                 */
/* ========================================================================== */
/* clang-format off */
const uint16_t PROGMEM keymaps[KB_LYR_MAX][MATRIX_ROWS][MATRIX_COLS] = {
    [KB_LYR_DVORAK] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,   KC_Y,                      KC_F,   KC_G,   KC_C,   KC_R,   KC_L,   KC_BSPC,
        KC_LCTL, KC_A,    KC_O,    KC_E,    KC_U,   KC_I,                      KC_D,   KC_H,   KC_T,   KC_N,   KC_S,   KC_SLSH,
        KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,   KC_X,                      KC_B,   KC_M,   KC_W,   KC_V,   KC_Z,   KC_ESC,
                                  KC_LGUI, MO(KB_LYR_NAV), KC_SPC,             KC_ENT, MO(KB_LYR_SYM), KC_RALT
    ),

    [KB_LYR_NAV] = LAYOUT_split_3x6_3(
        _______, KC_1,    KC_2,    KC_3,    KC_4,   KC_5,                      KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_DEL,
        _______, _______, _______, _______, _______, _______,                   KC_LEFT,KC_DOWN,KC_UP,  KC_RGHT,_______,_______,
        _______, _______, _______, _______, _______, _______,                   _______,_______,_______,_______,_______,_______,
                                  _______, _______, _______,                    _______,MO(KB_LYR_RGB),_______
    ),

    [KB_LYR_SYM] = LAYOUT_split_3x6_3(
        _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR, KC_PERC,                    KC_CIRC,KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,KC_DEL,
        _______, _______, _______,  _______, _______, _______,                   KC_MINS,KC_EQL, KC_LBRC,KC_RBRC,KC_BSLS,KC_GRV,
        _______, _______, _______,  _______, _______, _______,                   KC_UNDS,KC_PLUS,KC_LCBR,KC_RCBR,KC_PIPE,KC_TILD,
                                  _______, MO(KB_LYR_RGB), _______,             _______,_______,_______
    ),

    [KB_LYR_RGB] = LAYOUT_split_3x6_3(
        QK_BOOT, D_RGB_TOG, D_RGB_HUI, D_RGB_SAI, D_RGB_VAI, D_RGB_MOD,         _______,_______,_______,_______,_______,_______,
        _______, D_RGB_RMOD, D_RGB_HUD, D_RGB_SAD, D_RGB_VAD, _______,          _______,_______,_______,_______,_______,_______,
        _______, _______,    _______,    _______,    _______,    _______,        _______,_______,_______,_______,_______,_______,
                                  _______, _______, _______,                    _______,_______,_______
    )
};
/* clang-format on */

/* ========================================================================== */
/*                          QMK Hook Implementations                          */
/* ========================================================================== */
/*
 * layer_state_set_user
 * Brief: When entering the RGB layer, suggest a mode/color (persisted).
 */
layer_state_t layer_state_set_user(layer_state_t state) {
#ifdef RGB_MATRIX_ENABLE
    /* Apply only on RGB layer entry so user selections persist elsewhere. */
    if (layer_state_cmp(state, KB_LYR_RGB)) {
        rgb_matrix_mode(RGB_MATRIX_BREATHING); /* persisted */
        rgb_matrix_sethsv(HSV_YELLOW);         /* persisted */
    }
#endif
    return state;
}

#ifdef OLED_ENABLE
/* ========================================================================== */
/*                               OLED Helpers                                 */
/* ========================================================================== */
/* Reset animation state for the requested half. */
static void anim_reset_half(bool is_master) {
    if (is_master) {
        pac_pos_master = 0U;
        for (uint8_t i = 0U; i < ANIM_COLS; ++i) pellets_master[i] = 1U;
    } else {
        pac_pos_slave = 0U;
        for (uint8_t i = 0U; i < ANIM_COLS; ++i) pellets_slave[i] = 1U;
    }
}

/* Clear a full text row. */
static void anim_clear_line(uint8_t row) {
    oled_set_cursor(0, row);
    for (uint8_t i = 0U; i < ANIM_COLS; ++i) oled_write_P(PSTR(" "), false);
}

/* Advance state and draw a pac-man line for master/slave halves. */
static void render_pacman_line(bool is_master) {
    if (!anim_inited) {
        anim_reset_half(true);
        anim_reset_half(false);
        anim_inited = true;
    }

    if (timer_elapsed32(anim_last_ms) >= ANIM_INTERVAL_MS) {
        anim_last_ms   = timer_read32();
        pac_mouth_open = !pac_mouth_open;

        if (is_master) {
            if ((pac_pos_master + 1U) < ANIM_COLS) pac_pos_master++;
            pellets_master[pac_pos_master] = 0U; /* eat pellet */

            bool any_left = false;
            for (uint8_t i = 0U; i < ANIM_COLS; ++i) {
                if (pellets_master[i] != 0U) { any_left = true; break; }
            }
            if (!any_left) anim_reset_half(true);
        } else {
            if ((pac_pos_slave + 1U) < ANIM_COLS) pac_pos_slave++;
            pellets_slave[pac_pos_slave] = 0U;

            bool any_left = false;
            for (uint8_t i = 0U; i < ANIM_COLS; ++i) {
                if (pellets_slave[i] != 0U) { any_left = true; break; }
            }
            if (!any_left) anim_reset_half(false);
        }
    }

    /* Draw current frame */
    anim_clear_line(0U);
    oled_set_cursor(0, 0);
    for (uint8_t col = 0U; col < ANIM_COLS; ++col) {
        bool is_pac = is_master ? (col == pac_pos_master) : (col == pac_pos_slave);
        if (is_pac) {
            char ch = pac_mouth_open ? 'C' : 'O'; /* pac glyph */
            oled_write_char(ch, false);
        } else {
            bool pellet = is_master ? (pellets_master[col] != 0U) : (pellets_slave[col] != 0U);
            oled_write_P(pellet ? PSTR(".") : PSTR(" "), false);
        }
    }
}

/* Write layer label. */
static void render_layer(void) {
    oled_write_ln_P(PSTR("Layer:"), false);
    if (layer_state_cmp(layer_state, KB_LYR_RGB)) { oled_write_ln_P(PSTR("RGB"), false); return; }
    if (layer_state_cmp(layer_state, KB_LYR_SYM)) { oled_write_ln_P(PSTR("SYM"), false); return; }
    if (layer_state_cmp(layer_state, KB_LYR_NAV)) { oled_write_ln_P(PSTR("NAV"), false); return; }
    oled_write_ln_P(PSTR("BASE"), false);
}

/*
 * oled_task_user
 * Brief: Draw animation + status (layer, WPM) on master; simple marker on slave.
 */
bool oled_task_user(void) {
    oled_clear();
    bool master = is_keyboard_master();

    /* Row 0: animation (both halves) */
    render_pacman_line(master);

    /* Rows 1-2: status panel */
    if (master) {
        oled_set_cursor(0, 1); oled_write_P(PSTR("Corne Dv"), false);
        oled_set_cursor(0, 2); render_layer();
        char wpm_str[16];
        snprintf(wpm_str, sizeof(wpm_str), "WPM:%3u", get_current_wpm());
        oled_write_ln(wpm_str, false);
    } else {
        oled_set_cursor(0, 1); oled_write_P(PSTR("Slave"), false);
        oled_set_cursor(0, 2); oled_write_P(PSTR("<anim>"), false);
    }
    return false;
}
#endif /* OLED_ENABLE */

/*
 * process_record_user
 * Brief: Handle custom RGB Matrix keycodes.
 */
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
#ifdef RGB_MATRIX_ENABLE
    if (record->event.pressed) {
        switch (keycode) {
            case D_RGB_TOG:  rgb_matrix_toggle();          return false;
            case D_RGB_HUI:  rgb_matrix_increase_hue();    return false;
            case D_RGB_HUD:  rgb_matrix_decrease_hue();    return false;
            case D_RGB_SAI:  rgb_matrix_increase_sat();    return false;
            case D_RGB_SAD:  rgb_matrix_decrease_sat();    return false;
            case D_RGB_VAI:  rgb_matrix_increase_val();    return false;
            case D_RGB_VAD:  rgb_matrix_decrease_val();    return false;
            case D_RGB_MOD:  rgb_matrix_step();            return false;
            case D_RGB_RMOD: rgb_matrix_step_reverse();    return false;
            default: break;
        }
    }
#endif
    return true;
}
