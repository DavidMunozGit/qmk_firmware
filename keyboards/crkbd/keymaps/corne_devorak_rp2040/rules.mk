###############################################################################
# @file rules.mk
# @brief Build rules for corne_devorak_rp2040 keymap.
###############################################################################

# Target: RP2040 Pro Micro-compatible (SparkFun Pro Micro RP2040)
CONVERT_TO = sparkfun_pm2040

# Core features
CONSOLE_ENABLE = no

# RGB: Matrix vs. RGBlight (mutually exclusive) — pick Matrix
RGB_MATRIX_ENABLE = yes
RGBLIGHT_ENABLE   = no

# OLED display support
OLED_ENABLE = yes

# Optional extras
# NKRO_ENABLE = yes
WPM_ENABLE = yes

# Split settings
SPLIT_KEYBOARD = yes
