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
  * bash -lc 'cd /home/dmunoz/Keyboard/qmk_firmware && make -j4 crkbd:corne_dvorak_rp2040 CONVERT_TO=sparkfun_pm2040' */

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
    KB_LYR_MOUSE,       /**< Mouse keys */
    /* OLED UI signal layers (transparent passthrough; used only for sync/state) */
    KB_LYR_OLED_SUP,     /**< OLED SUPPORT mode */
    KB_LYR_OLED_ANIM,    /**< OLED ANIM mode */
    KB_LYR_OLED_ANIM2,   /**< OLED animation select bit (0=bounce, 1=spinner) */
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
    D_OLED_MODE,  /**< Cycle OLED mode: INFO -> SUPPORT -> ANIM -> INFO */
    D_ANIM_NEXT,  /**< Next OLED animation */
    D_ANIM_PREV,  /**< Previous OLED animation */
    /* (OLED brightness control removed; using host display brightness keys) */
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
        KC_LCTL, KC_SCLN, KC_Q, KC_J, KC_K, KC_X,                              KC_B, KC_M, KC_W, KC_V, KC_Z, MT(MOD_RSFT, KC_SLSH),
            KC_LGUI, LT(KB_LYR_NUMS, KC_MINS), LT(KB_LYR_NAV, KC_SPC),         LT(KB_LYR_SYM, KC_ENT), LT(KB_LYR_FUNC, KC_LBRC), MT(MOD_RALT, KC_9)
    ),

    [KB_LYR_NUMS] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_LPRN, KC_RPRN, KC_LCBR, KC_RCBR, KC_TILD,                  XXXXXXX, KC_7,   KC_8,   KC_9,   KC_EQL, KC_BSPC,
        MT(MOD_LSFT, KC_TAB), KC_PLUS, KC_EQL, KC_MINS, KC_UNDS, KC_PIPE,      KC_DOT,  KC_4,   KC_5,   KC_6,   XXXXXXX, KC_DEL,
        KC_LCTL, KC_EXLM, KC_ASTR, KC_SLSH, KC_BSLS, KC_AMPR,                  XXXXXXX, KC_1,   KC_2,   KC_3,   XXXXXXX, MT(MOD_RSFT, XXXXXXX),
                                   KC_LGUI, XXXXXXX, MO(KB_LYR_MOUSE),         KC_ENT,  LT(KB_LYR_RGB, KC_0), KC_RALT
    ),

    [KB_LYR_SYM] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                  KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_BSPC,
        MT(MOD_LSFT, KC_TAB), KC_PIPE, KC_BSLS, KC_LBRC, KC_RBRC, KC_TILD,     KC_SLSH, KC_PLUS, KC_EQL,  KC_LT,   KC_GT,   KC_DEL,
        KC_LCTL, KC_LCBR, KC_RCBR, KC_QUOT, KC_DQUO, XXXXXXX,                  KC_NUBS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MT(MOD_RSFT, XXXXXXX),
                                XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, KC_RALT
    ),

    [KB_LYR_NAV] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_APP,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, KC_HOME, KC_UP,   KC_END,  KC_PGUP, KC_BSPC,
        MT(MOD_LSFT, KC_TAB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     C(KC_LEFT), KC_LEFT, KC_DOWN, KC_RGHT, C(KC_RGHT), KC_DEL,
        KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  C(KC_X), C(S(KC_UP)), C(S(KC_BSLS)), C(S(KC_DOWN)), KC_PGDN, MT(MOD_RSFT, KC_ENT),
                                   KC_LGUI, MO(KB_LYR_MOUSE), XXXXXXX,         C(KC_C), C(KC_V), KC_RALT
    ),

    [KB_LYR_FUNC] = LAYOUT_split_3x6_3(
        KC_ESC,  KC_CAPS,  KC_INS,  XXXXXXX,  KC_MSTP, KC_MNXT,               KC_VOLU, KC_F7,  KC_F8,  KC_F9,  KC_F12, KC_BRIU,
        MT(MOD_LSFT, KC_TAB), KC_SCRL, KC_PAUS, KC_APP, KC_MPLY, KC_MPRV,     KC_VOLD, KC_F4,  KC_F5,  KC_F6,  KC_F11, KC_BRID,
        KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                 KC_MUTE, KC_F1,  KC_F2,  KC_F3,  KC_F10, MT(MOD_RSFT, XXXXXXX),
                                   KC_LGUI, MO(KB_LYR_RGB), XXXXXXX,          XXXXXXX, XXXXXXX, KC_RALT
    ),

    [KB_LYR_RGB] = LAYOUT_split_3x6_3(
        KC_ESC,  D_RGB_TOG, D_RGB_HUI, D_RGB_SAI, D_RGB_VAI, D_RGB_MOD,                 D_OLED_MODE, D_ANIM_PREV, D_ANIM_NEXT, XXXXXXX, XXXXXXX, XXXXXXX,
        MT(MOD_LSFT, KC_TAB), D_RGB_RMOD, D_RGB_HUD, D_RGB_SAD, D_RGB_VAD,  XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MT(MOD_RSFT, XXXXXXX),
                                   KC_LGUI, XXXXXXX, XXXXXXX,                           XXXXXXX, XXXXXXX, KC_RALT
    ),

    [KB_LYR_MOUSE] = LAYOUT_split_3x6_3(
        KC_ESC,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, MS_UP, XXXXXXX, MS_WHLU, KC_BSPC,
        MT(MOD_LSFT, KC_TAB), MS_ACL0, MS_ACL1, MS_ACL2, XXXXXXX, XXXXXXX,     MS_WHLL, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLR, KC_DEL,
        KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MS_WHLD, MT(MOD_RSFT, XXXXXXX),
                                   KC_LGUI, XXXXXXX, XXXXXXX,                  MS_BTN1, MS_BTN2, KC_RALT
    ),

    /* OLED UI layers are transparent passthrough so they don't affect typing */
    [KB_LYR_OLED_SUP] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______,                  _______, _______, _______
    ),

    [KB_LYR_OLED_ANIM] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______,                  _______, _______, _______
    ),

    [KB_LYR_OLED_ANIM2] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                  _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______,                  _______, _______, _______
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

/* ---------------------------- OLED mode control --------------------------- */
typedef enum oled_mode_e {
    OLED_MODE_INFO = 0U,
    OLED_MODE_SUPPORT,
    OLED_MODE_ANIM
} oled_mode_e;

typedef enum oled_anim_e {
    OLED_ANIM_BOUNCE = 0U,
    OLED_ANIM_SPINNER,
    OLED_ANIM_COUNT
} oled_anim_e;
static uint32_t g_oled_anim_timer = 0U;
static uint8_t g_oled_anim_step = 0U;

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
    case KB_LYR_MOUSE:
        name = "MOUSE";
        break;
    default:
        /* leave as '?' */
        break;
    }
    return name;
}

static uint8_t get_highest_real_layer(void)
{
    layer_state_t ls = layer_state;
    ls &= ~((layer_state_t)1UL << KB_LYR_OLED_SUP);
    ls &= ~((layer_state_t)1UL << KB_LYR_OLED_ANIM);
    ls &= ~((layer_state_t)1UL << KB_LYR_OLED_ANIM2);
    return get_highest_layer(ls);
}

static oled_mode_e get_oled_mode(void)
{
    if ( layer_state_is(KB_LYR_OLED_ANIM) )
    {
        return OLED_MODE_ANIM;
    }
    if ( layer_state_is(KB_LYR_OLED_SUP) )
    {
        return OLED_MODE_SUPPORT;
    }
    return OLED_MODE_INFO;
}

static oled_anim_e get_oled_anim_sel(void)
{
    return layer_state_is(KB_LYR_OLED_ANIM2) ? OLED_ANIM_SPINNER : OLED_ANIM_BOUNCE;
}

static void key_label_3(uint16_t kc, char out[4])
{
    /* Default: unknown */
    out[0] = '?';
    out[1] = '?';
    out[2] = '?';
    out[3] = '\0';

    if ( kc == KC_NO )
    {
        memcpy(out, "___", 4);
        return;
    }

    if ( kc == KC_TRNS )
    {
        memcpy(out, "...", 4);
        return;
    }

    /* Decode simple MT/LT wrappers into their tap key. */
    if ( (kc & 0xFF00U) == (QK_MOD_TAP & 0xFF00U) )
    {
        kc = (uint16_t)(kc & 0xFFU);
    }
    else if ( (kc & 0xFF00U) == (QK_LAYER_TAP & 0xFF00U) )
    {
        kc = (uint16_t)(kc & 0xFFU);
    }

    if ( kc >= KC_A && kc <= KC_Z )
    {
        out[0] = ' ';
        out[1] = (char)('A' + (kc - KC_A));
        out[2] = ' ';
        return;
    }

    if ( kc >= KC_1 && kc <= KC_9 )
    {
        out[0] = ' ';
        out[1] = (char)('1' + (kc - KC_1));
        out[2] = ' ';
        return;
    }

    switch ( kc )
    {
    case KC_0:    memcpy(out, " 0 ", 4); break;
    case KC_ESC:  memcpy(out, "ESC", 4); break;
    case KC_TAB:  memcpy(out, "TAB", 4); break;
    case KC_SPC:  memcpy(out, "SPC", 4); break;
    case KC_ENT:  memcpy(out, "ENT", 4); break;
    case KC_BSPC: memcpy(out, "BSP", 4); break;
    case KC_DEL:  memcpy(out, "DEL", 4); break;
    case KC_MINS: memcpy(out, " - ", 4); break;
    case KC_EQL:  memcpy(out, " = ", 4); break;
    case KC_LBRC: memcpy(out, " [ ", 4); break;
    case KC_RBRC: memcpy(out, " ] ", 4); break;
    case KC_LPRN: memcpy(out, " ( ", 4); break;
    case KC_RPRN: memcpy(out, " ) ", 4); break;
    case KC_LCBR: memcpy(out, " { ", 4); break;
    case KC_RCBR: memcpy(out, " } ", 4); break;
    case KC_SLSH: memcpy(out, " / ", 4); break;
    case KC_BSLS: memcpy(out, "\\ ", 3); out[2] = ' '; out[3] = '\0'; break;
    case KC_COMM: memcpy(out, " , ", 4); break;
    case KC_DOT:  memcpy(out, " . ", 4); break;
    case KC_QUOT: memcpy(out, " ' ", 4); break;
    case KC_DQUO: memcpy(out, "\" ", 3); out[2] = ' '; out[3] = '\0'; break;
    case KC_SCLN: memcpy(out, " ; ", 4); break;
    case KC_COLN: memcpy(out, " : ", 4); break;
    case KC_EXLM: memcpy(out, " ! ", 4); break;
    case KC_AT:   memcpy(out, " @ ", 4); break;
    case KC_HASH: memcpy(out, " # ", 4); break;
    case KC_DLR:  memcpy(out, " $ ", 4); break;
    case KC_PERC: memcpy(out, " % ", 4); break;
    case KC_AMPR: memcpy(out, " & ", 4); break;
    case KC_ASTR: memcpy(out, " * ", 4); break;
    case KC_PLUS: memcpy(out, " + ", 4); break;
    case KC_UNDS: memcpy(out, " _ ", 4); break;
    case KC_PIPE: memcpy(out, " | ", 4); break;
    default:
        /* keep ??? */
        break;
    }
}

static void draw_info_view(bool is_left)
{
    char line[OLED_COLS + 1U];
    uint8_t lyr = get_highest_real_layer();
    const char *layer_name = get_layer_name(lyr);

    if ( lyr == KB_LYR_RGB )
    {
#ifdef RGB_MATRIX_ENABLE
        /* RGB info screen: split details across halves (avoid duplicating HSV everywhere).
         * Left: Mode + Hue
         * Right: Saturation + Brightness
         * Hand label is intentionally omitted on this screen.
         */
        if ( is_left )
        {
            const char *mode_name = rgb_matrix_get_mode_name(rgb_matrix_get_mode());
            (void)snprintf(line, sizeof(line), "RGB Mode:%s", mode_name);
            pad_or_trunc_to_oled_cols(line);
            oled_set_cursor(0, 0);
            oled_write_ln(line, false);

            (void)snprintf(line, sizeof(line), "Hue:%02X", (unsigned int)rgb_matrix_config.hsv.h);
            pad_or_trunc_to_oled_cols(line);
            oled_set_cursor(0, 1);
            oled_write_ln(line, false);

            memset(line, ' ', OLED_COLS);
            line[OLED_COLS] = '\0';
            oled_set_cursor(0, 2);
            oled_write(line, false);
        }
        else
        {
            (void)snprintf(line, sizeof(line), "Saturation:%02X", (unsigned int)rgb_matrix_config.hsv.s);
            pad_or_trunc_to_oled_cols(line);
            oled_set_cursor(0, 0);
            oled_write_ln(line, false);

            (void)snprintf(line, sizeof(line), "Brightness:%02X", (unsigned int)rgb_matrix_config.hsv.v);
            pad_or_trunc_to_oled_cols(line);
            oled_set_cursor(0, 1);
            oled_write_ln(line, false);

            memset(line, ' ', OLED_COLS);
            line[OLED_COLS] = '\0';
            oled_set_cursor(0, 2);
            oled_write(line, false);
        }
#else
        /* RGB disabled: blank top rows */
        memset(line, ' ', OLED_COLS);
        line[OLED_COLS] = '\0';
        oled_set_cursor(0, 0);
        oled_write(line, false);
        oled_set_cursor(0, 1);
        oled_write(line, false);
        oled_set_cursor(0, 2);
        oled_write(line, false);
#endif
    }
    else
    {
        /* Row 0: Hand (non-RGB INFO screen) */
        (void)snprintf(line, sizeof(line), "Hand: %s", is_left ? "Left" : "Right");
        pad_or_trunc_to_oled_cols(line);
        oled_set_cursor(0, 0);
        oled_write_ln(line, false);

        /* Rows 1-2 blank unless on RGB layer */
        memset(line, ' ', OLED_COLS);
        line[OLED_COLS] = '\0';
        oled_set_cursor(0, 1);
        oled_write(line, false);
        oled_set_cursor(0, 2);
        oled_write(line, false);
    }

    /* Row 3: Layer (always) */
    (void)snprintf(line, sizeof(line), "Layer: %s", layer_name);
    pad_or_trunc_to_oled_cols(line);
    oled_set_cursor(0, 3);
    oled_write(line, false);
}

static void draw_support_view(bool is_left)
{
    /* SUPPORT: 3 rows of 5 keys for the current real layer, plus Layer row */
    char line[OLED_COLS + 1U];
    uint8_t lyr = get_highest_real_layer();
    const char *layer_name = get_layer_name(lyr);
    char lbl[4];

    for ( uint8_t row = 0U; row < 3U; ++row )
    {
        line[0] = '\0';
        if ( is_left )
        {
            for ( uint8_t col = 1U; col < 6U; ++col )
            {
                key_label_3(keymaps[lyr][row][col], lbl);
                strcat(line, lbl);
                if ( col < 5U ) strcat(line, " ");
            }
        }
        else
        {
            /* Right half in LAYOUT_split_3x6_3 uses matrix rows 4..6 and cols 5..0 (mirrored).
             * Match left-half behavior of skipping outer col 0 by showing cols 5..1.
             */
            uint8_t matrix_row = (uint8_t)(row + 4U);
            for ( int8_t col = 5; col >= 1; --col )
            {
                key_label_3(keymaps[lyr][matrix_row][(uint8_t)col], lbl);
                strcat(line, lbl);
                if ( col > 1 ) strcat(line, " ");
            }
        }
        pad_or_trunc_to_oled_cols(line);
        oled_set_cursor(0, row);
        oled_write(line, false);
    }

    (void)snprintf(line, sizeof(line), "Layer: %s", layer_name);
    pad_or_trunc_to_oled_cols(line);
    oled_set_cursor(0, 3);
    oled_write(line, false);
}

static void draw_anim_view(void)
{
    char line[OLED_COLS + 1U];
    uint8_t lyr = get_highest_real_layer();
    const char *layer_name = get_layer_name(lyr);
    oled_anim_e anim = get_oled_anim_sel();

    if ( timer_elapsed32(g_oled_anim_timer) > 120U )
    {
        g_oled_anim_timer = timer_read32();
        g_oled_anim_step = (uint8_t)(g_oled_anim_step + 1U);
    }

    /* Rows 0-2 animated (simple text animation), row 3 is layer */
    for ( uint8_t r = 0U; r < 3U; ++r )
    {
        memset(line, ' ', OLED_COLS);
        line[OLED_COLS] = '\0';

        if ( anim == OLED_ANIM_BOUNCE )
        {
            uint8_t pos = (uint8_t)(g_oled_anim_step % (OLED_COLS - 2U));
            /* slightly offset each row */
            pos = (uint8_t)((pos + (r * 3U)) % (OLED_COLS - 2U));
            line[pos] = '[';
            line[pos + 1U] = '#';
            line[pos + 2U] = ']';
        }
        else
        {
            static const char spin[4] = {'|', '/', '-', '\\'};
            line[0] = 'A';
            line[1] = 'n';
            line[2] = 'i';
            line[3] = 'm';
            line[4] = ':';
            line[6] = spin[(g_oled_anim_step + r) & 3U];
        }

        oled_set_cursor(0, r);
        oled_write(line, false);
    }

    (void)snprintf(line, sizeof(line), "Layer: %s", layer_name);
    pad_or_trunc_to_oled_cols(line);
    oled_set_cursor(0, 3);
    oled_write(line, false);
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
bool oled_task_user(void)
{
    oled_clear();

    bool is_left = is_keyboard_left();
    oled_mode_e mode = get_oled_mode();

    switch ( mode )
    {
    case OLED_MODE_SUPPORT:
        draw_support_view(is_left);
        break;
    case OLED_MODE_ANIM:
        draw_anim_view();
        break;
    case OLED_MODE_INFO:
    default:
        draw_info_view(is_left);
        break;
    }
    return false;
}
#endif /* OLED_ENABLE */

/* no housekeeping needed; split layer state mirrors our signal layer to both halves */

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
        switch ( keycode )
        {
        case D_OLED_MODE:
        {
            oled_mode_e mode = get_oled_mode();
            if ( mode == OLED_MODE_INFO )
            {
                layer_on(KB_LYR_OLED_SUP);
                layer_off(KB_LYR_OLED_ANIM);
            }
            else if ( mode == OLED_MODE_SUPPORT )
            {
                layer_off(KB_LYR_OLED_SUP);
                layer_on(KB_LYR_OLED_ANIM);
            }
            else
            {
                layer_off(KB_LYR_OLED_SUP);
                layer_off(KB_LYR_OLED_ANIM);
            }
            result = false;
        }
            break;
        case D_ANIM_NEXT:
            /* Two animations: toggle selection bit so it mirrors across halves */
            layer_invert(KB_LYR_OLED_ANIM2);
            result = false;
            break;
        case D_ANIM_PREV:
            /* Two animations: toggle selection bit so it mirrors across halves */
            layer_invert(KB_LYR_OLED_ANIM2);
            result = false;
            break;
        default:
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
