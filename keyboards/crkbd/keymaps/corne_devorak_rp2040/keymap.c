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
#include <stdio.h>
#include <string.h>

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
enum unicode_names
{
    U_DUMMY = 0
};
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
    KB_LYR_NUMS,        /**< Numbers */
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
                        KC_LGUI, MO(KB_LYR_NUMS), KC_SPC,                      KC_ENT, MO(KB_LYR_SYM), KC_RALT
    ),

    [KB_LYR_NUMS] = LAYOUT_split_3x6_3(
        _______, _______, KC_LPRN, KC_RPRN, _______, _______,                  KC_PAST, KC_P7,  KC_P8,  KC_P9,  KC_PSLS, KC_BSPC,
        _______, _______, KC_UNDS, KC_PIPE, _______, _______,                  KC_PDOT, KC_P4,  KC_P5,  KC_P6,  KC_PMNS, KC_PENT,
        _______, _______, _______, _______, _______, _______,                  KC_P0,   KC_P1,  KC_P2,  KC_P3,  KC_PPLS, KC_BSLS,
                                   _______, XXXXXXX, _______,                  KC_EQL,  MO(KB_LYR_RGB), _______
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
#ifdef OLED_ENABLE

/* ------------------------------ OLED layout ------------------------------- */
#define OLED_COLS              (21U)
#define OLED_ROWS              (4U)

/* ---------------------------- Stick animation ---------------------------- */
#define STICK_W                (8U)
#define STICK_H                (8U)
#define ANIM_INTERVAL_MS       (200U)

/* Two very small 8x8 stick-figure frames to "walk" */
static const uint8_t PROGMEM g_stick_frame0[STICK_W] = {
    /* Each byte is a vertical column of 8 pixels (LSB at top) */
    0x00U, /* ........ */
    0x18U, /* ...XX... head/torso */
    0x18U, /* ...XX... */
    0x18U, /* ...XX... torso */
    0x24U, /* ..X..X.. arms */
    0x24U, /* ..X..X.. */
    0x42U, /* .X....X. legs pose A */
    0x00U  /* ........ */
};

static const uint8_t PROGMEM g_stick_frame1[STICK_W] = {
    0x00U, /* ........ */
    0x18U, /* ...XX... head/torso */
    0x18U, /* ...XX... */
    0x18U, /* ...XX... torso */
    0x24U, /* ..X..X.. arms */
    0x24U, /* ..X..X.. */
    0x81U, /* X......X legs pose B */
    0x00U  /* ........ */
};

static uint32_t g_anim_timer = 0U;
static uint8_t  g_anim_frame = 0U; /* 0 or 1 */

/* ------------------------------- Formatting ------------------------------ */
/**
 * @brief Right-pad/truncate a C string in-place to exactly OLED_COLS chars.
 * @param buf mutable buffer (null-terminated on entry)
 */
static void pad_or_trunc_to_oled_cols(char *buf)
{
    size_t len = strlen(buf);
    if ( len >= OLED_COLS )
    {
        buf[OLED_COLS] = '\0';
    }
    else
    {
        /* pad with spaces */
        while ( len < OLED_COLS )
        {
            buf[len] = ' ';
            ++len;
        }
        buf[OLED_COLS] = '\0';
    }
}

/**
 * @brief Map layer id to compact name.
 * @param layer_id Current highest layer id
 * @return const char* to static string literal
 */
static const char *get_layer_name(uint8_t layer_id)
{
    const char *name = "?";

    switch ( layer_id )
    {
    case KB_LYR_DVORAK:
        name = "DVORAK";
        break;
    case KB_LYR_NUMS:
        name = "NUMS";
        break;
    case KB_LYR_SYM:
        name = "SYM";
        break;
    case KB_LYR_RGB:
        name = "RGB";
        break;
    default:
        /* leave as '?' */
        break;
    }
    return name;
}

/**
 * @brief Draw a single 8x8 stick figure frame at bottom-right.
 */
static void draw_stick_bottom_right(void)
{
    /* Update frame based on time */
    if ( timer_elapsed32(g_anim_timer) > ANIM_INTERVAL_MS )
    {
        g_anim_timer = timer_read32();
        g_anim_frame ^= 1U;
    }

    /* Place at last column (will exactly fit within 128px wide area) */
    oled_set_cursor((uint8_t)(OLED_COLS - 1U), (uint8_t)(OLED_ROWS - 1U));
    if ( g_anim_frame == 0U )
    {
        oled_write_raw_P((const char *)g_stick_frame0, STICK_W);
    }
    else
    {
        oled_write_raw_P((const char *)g_stick_frame1, STICK_W);
    }
}

#endif /* OLED_ENABLE */

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
    // if ( layer_state_cmp(state, KB_LYR_RGB) )
    // {
    //     rgb_matrix_mode(RGB_MATRIX_BREATHING); /* persisted */
    //     rgb_matrix_sethsv(HSV_YELLOW);         /* persisted */
    // }
#endif
    return state;
}

#ifdef OLED_ENABLE
/**
 * @brief OLED frame task: 21x4 layout
 * Row 0: ID:[Left|Right] Layer:<name>
 * Row 1: RGB:[On|Off]  Mode:<name> (truncated)
 * Row 2: Hue:FF Sat:FF Val:FF (hex)
 * Row 3: 8x8 stick figure animation at bottom-right corner
 * @return false to allow default handling to continue
 */
bool oled_task_user(void)
{
    char    line[OLED_COLS + 1U];
    uint8_t lyr = get_highest_layer(layer_state);
    const char *layer_name = get_layer_name(lyr);

    oled_clear();

    /* Row 0 */
    (void)snprintf(line, sizeof(line), "ID:%s Layer:%s",
                   (is_keyboard_master() ? "Left" : "Right"),
                   layer_name);
    pad_or_trunc_to_oled_cols(line);
    oled_set_cursor(0, 0);
    oled_write_ln(line, false);

    /* Row 1 */
#ifdef RGB_MATRIX_ENABLE
    {
        const char *onoff = (rgb_matrix_config.enable != 0U) ? "On" : "Off";
        const char *mode_name = rgb_matrix_get_mode_name(rgb_matrix_get_mode());
        (void)snprintf(line, sizeof(line), "RGB:%s  Mode:%s", onoff, mode_name);
    }
#else
    (void)snprintf(line, sizeof(line), "RGB:%s  Mode:%s", "Off", "N/A");
#endif
    pad_or_trunc_to_oled_cols(line);
    oled_set_cursor(0, 1);
    oled_write_ln(line, false);

    /* Row 2 */
#ifdef RGB_MATRIX_ENABLE
    (void)snprintf(line, sizeof(line), "Hue:%02X Sat:%02X Val:%02X",
                   (unsigned int)rgb_matrix_config.hsv.h,
                   (unsigned int)rgb_matrix_config.hsv.s,
                   (unsigned int)rgb_matrix_config.hsv.v);
#else
    (void)snprintf(line, sizeof(line), "Hue:%02X Sat:%02X Val:%02X", 0U, 0U, 0U);
#endif
    pad_or_trunc_to_oled_cols(line);
    oled_set_cursor(0, 2);
    oled_write_ln(line, false);

    /* Row 3: leave text blank, draw animation at bottom-right */
    memset(line, ' ', OLED_COLS);
    line[OLED_COLS] = '\0';
    oled_set_cursor(0, 3);
    oled_write(line, false);
    draw_stick_bottom_right();

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
