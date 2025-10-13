#pragma once

// Personal branding and product string (override keyboard defaults)
#undef MANUFACTURER
#undef PRODUCT
#define MANUFACTURER "dmunoz"
#define PRODUCT "corne_devorak_rp2040"

// Optional: tweak tapping behavior here
//#define TAPPING_TERM 180
//#define PERMISSIVE_HOLD

/*
PSEUDOCODE/PLAYGROUND (what you can tweak)
-----------------------------------------
// Tapping behavior per-key:
//   bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) { return keycode == LT(_NAV, KC_SPC); }
//   uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) { return keycode == MT(MOD_LCTL, KC_A) ? 200 : TAPPING_TERM; }

// Layer indicators with RGB Matrix:
//   layer_state_set_user(state) {
//     if (layer_state_cmp(state, 1)) rgb_matrix_sethsv_noeeprom(HSV_BLUE);
//     else if (layer_state_cmp(state, 2)) rgb_matrix_sethsv_noeeprom(HSV_GREEN);
//     else rgb_matrix_sethsv_noeeprom(HSV_RED);
//     return state;
//   }

// OLED render hooks:
//   void oled_task_user(void) {
//     if (is_keyboard_master()) {
//        // Left: status (layer, WPM, caps)
//        // Right: logo or matrix mode
//     } else {
//        // Slave: logo or mirrored info
//     }
//   }

// Split transport tuning:
//   #define SPLIT_WATCHDOG_ENABLE
//   #define SPLIT_WATCHDOG_TIMEOUT 3000
//   #define FORCED_SYNC_THROTTLE_MS 100

// Handedness options (pick only one strategy):
//   #define EE_HANDS
//     and program EEPROM for each half; OR
//   #define MASTER_LEFT
//   // #define MASTER_RIGHT

*/

// RGB Matrix configuration (per-key + underglow)
#ifdef RGB_MATRIX_ENABLE
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120
    #define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
    #define RGB_MATRIX_DEFAULT_HUE 0
    #define RGB_MATRIX_DEFAULT_SAT 255
    #define RGB_MATRIX_DEFAULT_VAL 120
    // Useful effects to try; enable/disable as desired
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

// OLED configuration
#ifdef OLED_ENABLE
    #define OLED_TIMEOUT 60000
    // If you want 180-degree rotation on one half:
    // #define OLED_ROTATION OLED_ROTATION_180
#endif

// Split feature flags to mirror runtime features across halves
#define SPLIT_WPM_ENABLE
#define SPLIT_OLED_ENABLE
