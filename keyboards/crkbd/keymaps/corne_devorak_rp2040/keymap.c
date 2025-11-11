/**
 * @file keymap.c
 * @brief Corne (crkbd) Dvorak keymap for RP2040 with OLED UI and RGB Matrix controls.
 *
 * Provides:
 * - Dvorak base layer with NAV, SYM, and RGB layers
 * - OLED output: LEFT on master half, RIGHT on slave half
 * - RGB Matrix convenience keycodes and suggested effect on RGB layer
 */

 /* Helper for compilation:
  * bash -lc 'cd /home/dmunoz/Keyboard/qmk_firmware && make -j4 crkbd:corne_devorak_rp2040 CONVERT_TO=sparkfun_pm2040' */

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
    KB_LYR_NAV,         /**< Navigation & editor shortcuts */
    KB_LYR_FUNC,        /**< Function keys (F1–F12) and media */
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
    D_ANIM_NEXT,  /**< Cycle to next OLED animation */
    D_ANIM_PREV,  /**< Cycle to previous OLED animation */
    D_ANIM_MODE   /**< Toggle full-screen animation mode */
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
        KC_ESC,  KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y,                         KC_F, KC_G, KC_C, KC_R, KC_L, KC_BSPC,
        MT(MOD_LSFT, KC_TAB), KC_A, KC_O, KC_E, KC_U, KC_I,                    KC_D, KC_H, KC_T, KC_N, KC_S, KC_DEL,
        KC_LCTL, KC_SCLN, KC_Q, KC_J, KC_K, KC_X,                              KC_B, KC_M, KC_W, KC_V, KC_Z, KC_SLSH,
            KC_LGUI, MO(KB_LYR_NUMS), LT(KB_LYR_NAV, KC_SPC),                  LT(KB_LYR_FUNC, KC_ENT), LT(KB_LYR_SYM, KC_LCBR), MT(MOD_RALT, KC_LPRN)
    ),

    [KB_LYR_NUMS] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_LABK, KC_PSLS, KC_PMNS, KC_PIPE, KC_TILD,                  _______, KC_P7,  KC_P8,  KC_P9,  _______, KC_BSPC,
        MT(MOD_LSFT, KC_TAB), KC_RABK, KC_PAST, KC_PPLS, KC_AMPR, KC_EXLM,     KC_PDOT, KC_P4,  KC_P5,  KC_P6,  _______, KC_DEL,
        KC_LCTL, KC_LBRC, KC_LPRN, KC_CIRC, KC_PEQL, KC_PERC,                  _______, KC_P1,  KC_P2,  KC_P3,  _______, _______,
                                   KC_LGUI, XXXXXXX, _______,                  KC_PENT, LT(KB_LYR_RGB, KC_P0), KC_RALT
    ),

    [KB_LYR_SYM] = LAYOUT_split_3x6_3(
        KC_ESC,  _______, _______, KC_MINS, _______, _______,                  KC_BSLS, _______, KC_NUHS, _______, _______, KC_BSPC,
        MT(MOD_LSFT, KC_TAB), KC_RBRC,  KC_ASTR,  _______,  _______,  _______, KC_SLSH, KC_AT,  KC_HASH, _______, KC_DLR, KC_DEL,
        KC_LCTL, KC_LBRC, _______, _______, KC_EQL, _______,                   KC_NUBS, _______, _______, _______, _______,  _______,
                                _______, _______, _______,                     _______, XXXXXXX, KC_RALT
    ),

    [KB_LYR_NAV] = LAYOUT_split_3x6_3(
        KC_ESC,  _______, _______, _______, _______, _______,                  _______, KC_HOME, KC_UP,   KC_END,  KC_PGUP, KC_BSPC,
        MT(MOD_LSFT, KC_TAB), _______, _______, _______, _______, _______,     C(KC_LEFT), KC_LEFT, KC_DOWN, KC_RGHT, C(KC_RGHT), KC_DEL,
        KC_LCTL, _______, _______, _______, _______, _______,                  C(KC_X), C(S(KC_UP)), C(S(KC_BSLS)), C(S(KC_DOWN)), KC_PGDN, KC_ENT,
                                   KC_LGUI, _______, XXXXXXX,                  C(KC_C), _______, MT(MOD_RALT, C(KC_V))
    ),

    [KB_LYR_FUNC] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_CAPS,  KC_INS,  _______,  KC_MSTP, KC_MNXT,               KC_VOLU, KC_F7,  KC_F8,  KC_F9,  KC_F12, KC_PSCR,
        MT(MOD_LSFT, KC_TAB), KC_SCRL, KC_PAUS, KC_APP, KC_MPLY, KC_MPRV,     KC_VOLD, KC_F4,  KC_F5,  KC_F6,  KC_F11, _______,
        KC_LCTL, _______, _______, _______, _______, _______,                  KC_MUTE, KC_F1,  KC_F2,  KC_F3,  KC_F10, _______,
                                   KC_LGUI, _______, _______,                  XXXXXXX, _______, KC_RALT
    ),

    [KB_LYR_RGB] = LAYOUT_split_3x6_3(
        KC_ESC,  D_RGB_TOG, D_RGB_HUI, D_RGB_SAI, D_RGB_VAI, D_RGB_MOD,                 _______, _______, _______, _______, _______, _______,
        MT(MOD_LSFT, KC_TAB), D_RGB_RMOD, D_RGB_HUD, D_RGB_SAD, D_RGB_VAD,  _______,    _______, D_ANIM_MODE, D_ANIM_PREV, D_ANIM_NEXT, _______, _______,
        KC_LCTL, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                   KC_LGUI, XXXXXXX, _______,                           _______, XXXXXXX, KC_RALT
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

/* ---------------------------- Animation timing --------------------------- */
#define ANIM_INTERVAL_MS       (120U)
static uint32_t g_anim_timer = 0U;

/* -------------------------- Animation Framework -------------------------- */
#define OLED_CELL_W            (6U) /* Default font cell width */

typedef enum oled_anim_type_e {
    OLED_ANIM_PACMAN = 0U,
    OLED_ANIM_WAVE,
    OLED_ANIM_SPARKLE,
    OLED_ANIM_GRADIENT,
    OLED_ANIM_COUNT
} oled_anim_type_e;

static uint8_t  g_oled_anim = OLED_ANIM_PACMAN; /* current animation selection */
static bool     g_anim_fullscreen = false;      /* when true: animation replaces all text */
static uint32_t g_anim_frame = 0U;              /* monotonically incremented frame counter */

/* Pac-Man assets (kept as one animation option) */
#define PAC_W                  (OLED_CELL_W)
#define PAC_TRAIL_STEPS        (OLED_COLS + 3U)
static const uint8_t g_pac_open[PAC_W] = { 0x3CU, 0x7EU, 0xE6U, 0xC0U, 0x7EU, 0x3CU };
static const uint8_t g_pac_closed[PAC_W] = { 0x3CU, 0x7EU, 0xE6U, 0xE6U, 0x7EU, 0x3CU };
static uint8_t g_pac_step = 0U;  /* 0..PAC_TRAIL_STEPS-1 */
static uint8_t g_pac_mouth = 0U; /* 0=open, 1=closed */

/* Wave animation state */
static uint8_t g_wave_phase = 0U; /* increments over time */

/* Sparkle animation state (simple LFSR pseudo-random) */
static uint16_t g_spark_lfsr = 0xACE1U; /* seed */

/* Gradient animation state (hue shift) */
static uint8_t g_grad_offset = 0U;

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
    case KB_LYR_NAV:
        name = "NAV";
        break;
    case KB_LYR_FUNC:
        name = "FN";
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
/* (stick-figure animation removed in favor of Pac-Man row) */

/**
 * @brief Draw Pac-Man animation across row 3 on the right OLED.
 * - Fill with 21 cells of food (one middle pixel per cell)
 * - Draw Pac-Man (open/closed) at current cell, facing right
 * - Advance and wrap with an out-of-bounds illusion
 */
static void draw_anim_row(uint8_t row)
{
    /* Throttle frame updates */
    if ( timer_elapsed32(g_anim_timer) > ANIM_INTERVAL_MS )
    {
        g_anim_timer = timer_read32();
        ++g_anim_frame;
        /* Per-animation state advance */
        switch ( g_oled_anim )
        {
        case OLED_ANIM_PACMAN:
            if ( g_pac_step < OLED_COLS )
            {
                g_pac_mouth ^= 1U;
            }
            g_pac_step = (uint8_t)((g_pac_step + 1U) % PAC_TRAIL_STEPS);
            break;
        case OLED_ANIM_WAVE:
            g_wave_phase = (uint8_t)(g_wave_phase + 1U);
            break;
        case OLED_ANIM_SPARKLE:
            /* LFSR update: taps 16,14,13,11 */
            {
                uint16_t lsb = (uint16_t)(((g_spark_lfsr >> 0U) ^ (g_spark_lfsr >> 2U) ^ (g_spark_lfsr >> 3U) ^ (g_spark_lfsr >> 5U)) & 1U);
                g_spark_lfsr = (uint16_t)((g_spark_lfsr >> 1U) | (lsb << 15U));
            }
            break;
        case OLED_ANIM_GRADIENT:
            g_grad_offset = (uint8_t)(g_grad_offset + 1U);
            break;
        default:
            break;
        }
    }

    uint8_t row_buf[OLED_COLS * OLED_CELL_W];
    memset(row_buf, 0, sizeof(row_buf));

    switch ( g_oled_anim )
    {
    case OLED_ANIM_PACMAN:
    {
        uint8_t active_col = (g_pac_step < OLED_COLS) ? g_pac_step : 0xFFU;
        for ( uint8_t col = 0U; col < OLED_COLS; ++col )
        {
            uint8_t *cell = &row_buf[col * OLED_CELL_W];
            if ( col == active_col )
            {
                const uint8_t *glyph = (g_pac_mouth == 0U) ? g_pac_open : g_pac_closed;
                memcpy(cell, glyph, PAC_W);
            }
            else
            {
                const uint8_t dot_mask = 0x18U; /* pellet */
                cell[OLED_CELL_W / 2U] = dot_mask;
            }
        }
    }
        break;
    case OLED_ANIM_WAVE:
    {
        /* Simple vertical sine-ish representation using three brightness levels */
        for ( uint8_t col = 0U; col < OLED_COLS; ++col )
        {
            uint8_t phase = (uint8_t)(col + g_wave_phase);
            uint8_t level = (uint8_t)(phase & 0x0FU); /* 0..15 */
            uint8_t *cell = &row_buf[col * OLED_CELL_W];
            uint8_t pattern = 0x00U;
            if ( level < 5U )
            {
                pattern = 0x10U; /* faint */
            }
            else if ( level < 10U )
            {
                pattern = 0x38U; /* medium */
            }
            else
            {
                pattern = 0x7CU; /* bright */
            }
            cell[2U] = pattern; /* middle vertical slice */
        }
    }
        break;
    case OLED_ANIM_SPARKLE:
    {
        for ( uint8_t col = 0U; col < OLED_COLS; ++col )
        {
            uint16_t rnd = (uint16_t)(g_spark_lfsr + (uint16_t)(col * 73U));
            uint8_t *cell = &row_buf[col * OLED_CELL_W];
            if ( (rnd & 0x0700U) == 0U )
            {
                /* occasional sparkle cluster */
                cell[1U] = 0x18U;
                cell[2U] = 0x3CU;
                cell[3U] = 0x18U;
            }
            else if ( (rnd & 0x0030U) == 0x0030U )
            {
                cell[2U] = 0x18U; /* single sparkle */
            }
        }
    }
        break;
    case OLED_ANIM_GRADIENT:
    {
        /* Horizontal gradient bar: three thin bands varying with offset */
        for ( uint8_t col = 0U; col < OLED_COLS; ++col )
        {
            uint8_t hue_step = (uint8_t)(col + g_grad_offset);
            uint8_t *cell = &row_buf[col * OLED_CELL_W];
            /* Convert pseudo-hue step to brightness pattern */
            uint8_t b0 = (uint8_t)(((hue_step >> 0U) & 1U) ? 0x7CU : 0x10U);
            uint8_t b1 = (uint8_t)(((hue_step >> 1U) & 1U) ? 0x38U : 0x00U);
            uint8_t b2 = (uint8_t)(((hue_step >> 2U) & 1U) ? 0x18U : 0x00U);
            cell[1U] = b0;
            cell[2U] = b1;
            cell[3U] = b2;
        }
    }
        break;
    default:
        break;
    }

    oled_set_cursor(0, row);
    oled_write_raw((const char *)row_buf, sizeof(row_buf));
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
 * Left OLED:
 *   Row 0: "Left Layer <name>"
 *   Row 1: "RGB Mode:<name>" (truncated)
 *   Row 2: "Hue:FF Sat:FF Val:FF" (hex)
 *   Row 3: empty
 * Right OLED:
 *   Row 0: "Right Layer <name>"
 *   Row 1: empty
 *   Row 2: empty
 *   Row 3: Pac-Man animation traversing food pellets
 * @return false to allow default handling to continue
 */
bool oled_task_user(void)
{
    char    line[OLED_COLS + 1U];
    uint8_t lyr = get_highest_layer(layer_state);
    const char *layer_name = get_layer_name(lyr);

    oled_clear();

    if ( g_anim_fullscreen )
    {
        /* Full-screen animation mode: all four rows animated on both halves */
        for ( uint8_t r = 0U; r < OLED_ROWS; ++r )
        {
            draw_anim_row(r);
        }
        return false;
    }

    if ( is_keyboard_master() )
    {
        /* LEFT (master) informational rows */
        (void)snprintf(line, sizeof(line), "Left Layer %s", layer_name);
        pad_or_trunc_to_oled_cols(line);
        oled_set_cursor(0, 0);
        oled_write_ln(line, false);

#ifdef RGB_MATRIX_ENABLE
        {
            const char *mode_name = rgb_matrix_get_mode_name(rgb_matrix_get_mode());
            (void)snprintf(line, sizeof(line), "RGB Mode:%s", mode_name);
        }
#else
        (void)snprintf(line, sizeof(line), "RGB:%s", "N/A");
#endif
        pad_or_trunc_to_oled_cols(line);
        oled_set_cursor(0, 1);
        oled_write_ln(line, false);

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

        /* Row 3: animation (shared across halves) */
        draw_anim_row(3U);
    }
    else
    {
        /* RIGHT (slave) minimal info + animation */
        (void)snprintf(line, sizeof(line), "Right Layer %s", layer_name);
        pad_or_trunc_to_oled_cols(line);
        oled_set_cursor(0, 0);
        oled_write_ln(line, false);

        memset(line, ' ', OLED_COLS);
        line[OLED_COLS] = '\0';
        oled_set_cursor(0, 1);
        oled_write(line, false);

        memset(line, ' ', OLED_COLS);
        line[OLED_COLS] = '\0';
        oled_set_cursor(0, 2);
        oled_write(line, false);

        draw_anim_row(3U);
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

    if ( record->event.pressed )
    {
        /* OLED Animation controls (always available when compiled with OLED) */
        switch ( keycode )
        {
        case D_ANIM_NEXT:
            g_oled_anim = (uint8_t)((g_oled_anim + 1U) % OLED_ANIM_COUNT);
            result = false;
            break;
        case D_ANIM_PREV:
            g_oled_anim = (uint8_t)((g_oled_anim + OLED_ANIM_COUNT - 1U) % OLED_ANIM_COUNT);
            result = false;
            break;
        case D_ANIM_MODE:
            g_anim_fullscreen = !g_anim_fullscreen;
            result = false;
            break;
        default:
            /* continue */
            break;
        }

#ifdef RGB_MATRIX_ENABLE
        switch ( keycode )
        {
        case D_RGB_TOG:
            rgb_matrix_toggle();
            result = false;
            break;
        case D_RGB_HUI:
        case D_RGB_HUD:
        case D_RGB_SAI:
        case D_RGB_SAD:
        case D_RGB_VAI:
        case D_RGB_VAD:
        {
            /* Adjust HSV in steps of 5, clamped to 0..255 */
            hsv_t hsv = rgb_matrix_get_hsv();
            const uint8_t step = 5U;
            switch ( keycode )
            {
            case D_RGB_HUI:
                hsv.h = (uint8_t)(hsv.h + step);
                break;
            case D_RGB_HUD:
                hsv.h = (uint8_t)(hsv.h - step);
                break;
            case D_RGB_SAI:
                hsv.s = (uint8_t)((hsv.s + step > 255U) ? 255U : (hsv.s + step));
                break;
            case D_RGB_SAD:
                hsv.s = (uint8_t)((hsv.s < step) ? 0U : (hsv.s - step));
                break;
            case D_RGB_VAI:
                hsv.v = (uint8_t)((hsv.v + step > 255U) ? 255U : (hsv.v + step));
                break;
            case D_RGB_VAD:
                hsv.v = (uint8_t)((hsv.v < step) ? 0U : (hsv.v - step));
                break;
            default:
                break;
            }
            rgb_matrix_sethsv(hsv.h, hsv.s, hsv.v);
            result = false;
        }
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
#endif
    }
    return result;
}
