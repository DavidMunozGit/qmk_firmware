/**
 * @file config.h
 * @brief Keymap-level configuration for corne_dvorak_rp2040.
 *
 * Sections: Identity, Tapping, RGB Matrix, OLED, Split, Notes.
 */

#pragma once

/* =============================== Identity ================================= */
#undef MANUFACTURER
#undef PRODUCT
#define MANUFACTURER "dmunoz"
#define PRODUCT      "corne_dvorak_rp2040"

/* ========================= Tapping/Behavior (opt) ========================= */
/* Tapping configuration for dual-function Space (LT(NAV, KC_SPC)) */
#define TAPPING_TERM 180          /* ms window to decide tap vs hold */
#define HOLD_ON_OTHER_KEY_PRESS   /* treat as hold if another key is pressed during tap window */
#define PERMISSIVE_HOLD           /* favor hold when rolling quickly to another key */

/* ======================= RGB Matrix Configuration ========================= */
#ifdef RGB_MATRIX_ENABLE
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120
#define RGB_MATRIX_DEFAULT_MODE       RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE        0
#define RGB_MATRIX_DEFAULT_SAT        255
#define RGB_MATRIX_DEFAULT_VAL        120
/* Enable full RGB Matrix feature sets required by many effects */
#define RGB_MATRIX_KEYPRESSES              /* Enable keypress reactive effects */
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS     /* Enable framebuffer-based effects */
/* Enable mode names helper for OLED display */
#define RGB_MATRIX_MODE_NAME_ENABLE        /* enables rgb_matrix_get_mode_name() */

/* Enable ALL core RGB Matrix effects (order is defined by rgb_matrix_effects.inc) */
/* Static/gradient/cycle effects */
#define ENABLE_RGB_MATRIX_SOLID_COLOR
#define ENABLE_RGB_MATRIX_ALPHAS_MODS
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_BAND_SAT
#define ENABLE_RGB_MATRIX_BAND_VAL
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#define ENABLE_RGB_MATRIX_CYCLE_ALL
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#define ENABLE_RGB_MATRIX_DUAL_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_FLOWER_BLOOMING

/* Framebuffer/pattern effects */
#define ENABLE_RGB_MATRIX_RAINDROPS
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#define ENABLE_RGB_MATRIX_HUE_BREATHING
#define ENABLE_RGB_MATRIX_HUE_PENDULUM
#define ENABLE_RGB_MATRIX_HUE_WAVE
#define ENABLE_RGB_MATRIX_PIXEL_RAIN
#define ENABLE_RGB_MATRIX_PIXEL_FLOW
#define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN

/* Key reactive effects */
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#define ENABLE_RGB_MATRIX_SPLASH
#define ENABLE_RGB_MATRIX_SOLID_SPLASH

/* Starlight and flows */
#define ENABLE_RGB_MATRIX_STARLIGHT_SMOOTH
#define ENABLE_RGB_MATRIX_STARLIGHT
#define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT
#define ENABLE_RGB_MATRIX_STARLIGHT_DUAL_HUE
#define ENABLE_RGB_MATRIX_RIVERFLOW
#endif

/* ========================== OLED Configuration ============================ */
#ifdef OLED_ENABLE
#define OLED_TIMEOUT 60000
/* If you want 180-degree rotation on one half: */
/* #define OLED_ROTATION OLED_ROTATION_180 */
#endif

/* ===================== Split Mirroring/Transport ========================== */
#define SPLIT_WPM_ENABLE
#define SPLIT_OLED_ENABLE
/* Ensure layer and modifier state are mirrored to the slave (right) half */
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_MODS_ENABLE

/* ============================ Unicode Mode =============================== */
/* Default Unicode input modes for Windows host via WSL: WinCompose first, Linux second */
#define UNICODE_SELECTED_MODES UNICODE_MODE_WINCOMPOSE, UNICODE_MODE_LINUX

/**
 * @section Notes
 *
 * Split transport tuning:
 *   - #define SPLIT_WATCHDOG_ENABLE
 *   - #define SPLIT_WATCHDOG_TIMEOUT 3000
 *   - #define FORCED_SYNC_THROTTLE_MS 100
 *
 * Handedness options (pick only one strategy):
 *   - #define EE_HANDS (program EEPROM for each half)
 *   - #define MASTER_LEFT
 *   - // #define MASTER_RIGHT
 *
 * Per-key tapping overrides examples:
 *   bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
 *       return keycode == LT(_NAV, KC_SPC);
 *   }
 *   uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
 *       return keycode == MT(MOD_LCTL, KC_A) ? 200 : TAPPING_TERM;
 *   }
 */
