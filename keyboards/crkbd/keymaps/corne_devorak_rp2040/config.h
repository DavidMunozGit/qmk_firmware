/**
 * @file config.h
 * @brief Keymap-level configuration for corne_devorak_rp2040.
 *
 * Sections: Identity, Tapping, RGB Matrix, OLED, Split, Notes.
 */

#pragma once

/* =============================== Identity ================================= */
#undef MANUFACTURER
#undef PRODUCT
#define MANUFACTURER "dmunoz"
#define PRODUCT      "corne_devorak_rp2040"

/* ========================= Tapping/Behavior (opt) ========================= */
// #define TAPPING_TERM 180
// #define PERMISSIVE_HOLD

/* ======================= RGB Matrix Configuration ========================= */
#ifdef RGB_MATRIX_ENABLE
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120
#define RGB_MATRIX_DEFAULT_MODE       RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE        0
#define RGB_MATRIX_DEFAULT_SAT        255
#define RGB_MATRIX_DEFAULT_VAL        120
/* Useful effects to try; enable/disable as desired */
#define ENABLE_RGB_MATRIX_SOLID_COLOR
#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_BAND_VAL
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#define ENABLE_RGB_MATRIX_SPLASH
#define ENABLE_RGB_MATRIX_MULTISPLASH
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
