/**
 * @file keymap.c
 * @brief Corne (crkbd) Dvorak keymap for RP2040 with OLED UI and RGB Matrix controls.
 *
 * Provides:
 * - Dvorak base layer with NAV, SYM, and RGB layers
 * - OLED output: LEFT on master half, RIGHT on slave half
 * - RGB Matrix convenience keycodes and suggested effect on RGB layer
 */

/* ========================================================================== */
/*                                Includes                                    */
/* ========================================================================== */
#include QMK_KEYBOARD_H

/* Provide empty stubs for optional features we enabled so the build links. */
#ifdef COMBO_ENABLE
/* Combo support: define an empty combo array so introspection compiles. */
combo_t key_combos[] = {};
uint16_t COMBO_LEN = 0;
#endif

#ifdef TAP_DANCE_ENABLE
/* Tap dance support: define an empty action table. */
tap_dance_action_t tap_dance_actions[] = {};
#endif

#ifdef UNICODEMAP_ENABLE
/* Unicode map: define a single dummy entry so UNICODEMAP links. */
enum unicode_names { U_DUMMY = 0 };
const uint32_t PROGMEM unicode_map[] = {
    [U_DUMMY] = 0x0020 /* space */
};
#endif

/* ========================================================================== */
/*                                Types                                       */
/* ========================================================================== */
/**
 * @enum KB_LAYERS_E
 * @brief Logical layers used by this keymap.
 */
typedef enum KB_LAYERS_E
{
    KB_LYR_DVORAK = 0U, /**< Base: Dvorak */
    KB_LYR_NAV,         /**< Navigation */
    KB_LYR_SYM,         /**< Symbols */
    KB_LYR_RGB,         /**< RGB controls */
    KB_LYR_MAX          /**< Number of layers */
} KB_LAYERS_E;

/**
 * @enum custom_keycodes_e
 * @brief Custom keycodes for RGB Matrix controls.
 */
typedef enum custom_keycodes_e
{
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

/* ========================================================================== */
/*                          Public-scope Variables                            */
/* ========================================================================== */
/**
 * @brief Keymap definition. clang-format is disabled for readability.
 */
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
        _______, D_RGB_TOG, D_RGB_HUI, D_RGB_SAI, D_RGB_VAI, D_RGB_MOD,         _______,_______,_______,_______,_______,_______,
        _______, D_RGB_RMOD, D_RGB_HUD, D_RGB_SAD, D_RGB_VAD, _______,          _______,_______,_______,_______,_______,_______,
        _______, _______,    _______,    _______,    _______,    _______,        _______,_______,_______,_______,_______,_______,
                                  _______, _______, _______,                    _______,_______,_______
    )
};
/* clang-format on */

/* ========================================================================== */
/*                           Module-scope Variables                           */
/* ========================================================================== */

/* ========================================================================== */
/*                           Function Prototypes                              */
/* ========================================================================== */
/* QMK hooks */
layer_state_t layer_state_set_user(layer_state_t state);
bool process_record_user(uint16_t keycode, keyrecord_t *record);
#ifdef OLED_ENABLE
bool oled_task_user(void);
#endif

/* Internal helpers (OLED) */

/* ========================================================================== */
/*                              Keymap Layers                                 */
/* ========================================================================== */


/* ========================================================================== */
/*                          QMK Hook Implementations                          */
/* ========================================================================== */
/**
 * @brief Layer change callback. When entering the RGB layer, suggests a
 *        persisted effect and color.
 * @param state New layer state bitmask
 * @return The (possibly modified) layer state
 */
layer_state_t layer_state_set_user(layer_state_t state)
{
#ifdef RGB_MATRIX_ENABLE
    /* Apply only on RGB layer entry so user selections persist elsewhere. */
    if ( layer_state_cmp(state, KB_LYR_RGB) )
    {
        rgb_matrix_mode(RGB_MATRIX_BREATHING); /* persisted */
        rgb_matrix_sethsv(HSV_YELLOW);         /* persisted */
    }
#endif
    return state;
}

#ifdef OLED_ENABLE
/**
 * @brief Minimal OLED frame task. Shows LEFT on master, RIGHT on slave.
 * @return false to allow default handling to continue
 */
bool oled_task_user(void)
{
    oled_clear();
    oled_set_cursor(0, 0);
    if ( is_keyboard_master() )
    {
        oled_write_ln_P(PSTR("LEFT"), false);
    }
    else
    {
        oled_write_ln_P(PSTR("RIGHT"), false);
    }
    return false;
}
#endif /* OLED_ENABLE */

/**
 * @brief Handle custom RGB Matrix keycodes.
 * @param keycode keycode value
 * @param record key event record
 * @return false if handled; true to propagate
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    bool result = true; /* MISRA: single exit point */

#ifdef RGB_MATRIX_ENABLE
    if ( record->event.pressed )
    {
        switch ( keycode )
        {
        case D_RGB_TOG:
            rgb_matrix_toggle();
            result = false;
            break;
        case D_RGB_HUI:
            rgb_matrix_increase_hue();
            result = false;
            break;
        case D_RGB_HUD:
            rgb_matrix_decrease_hue();
            result = false;
            break;
        case D_RGB_SAI:
            rgb_matrix_increase_sat();
            result = false;
            break;
        case D_RGB_SAD:
            rgb_matrix_decrease_sat();
            result = false;
            break;
        case D_RGB_VAI:
            rgb_matrix_increase_val();
            result = false;
            break;
        case D_RGB_VAD:
            rgb_matrix_decrease_val();
            result = false;
            break;
        case D_RGB_MOD:
            rgb_matrix_step();
            result = false;
            break;
        case D_RGB_RMOD:
            rgb_matrix_step_reverse();
            result = false;
            break;
        default:
            /* no action */
            break;
        }
    }
#endif
    return result;
}
