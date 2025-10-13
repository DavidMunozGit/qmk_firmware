#pragma once

// Personal branding and product string (override keyboard defaults)
#undef MANUFACTURER
#undef PRODUCT
#define MANUFACTURER "dmunoz"
#define PRODUCT "corne_devorak_rp2040"

// Optional: tweak tapping behavior here
//#define TAPPING_TERM 180
//#define PERMISSIVE_HOLD

#ifdef RGBLIGHT_ENABLE
    #define RGBLIGHT_LIMIT_VAL 120
    #define RGBLIGHT_HUE_STEP 10
    #define RGBLIGHT_SAT_STEP 17
    #define RGBLIGHT_VAL_STEP 17
#endif
