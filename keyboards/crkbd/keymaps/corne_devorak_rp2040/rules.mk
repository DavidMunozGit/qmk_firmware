###############################################################################
# @file rules.mk
# @brief Build rules for corne_devorak_rp2040 keymap.
###############################################################################

# Target: RP2040 Pro Micro-compatible (SparkFun Pro Micro RP2040)
CONVERT_TO = sparkfun_pm2040

# Core features
CONSOLE_ENABLE = no
COMMAND_ENABLE = yes
BOOTMAGIC_ENABLE = lite

# RGB: Matrix vs. RGBlight (mutually exclusive) — pick Matrix
RGB_MATRIX_ENABLE = yes
RGBLIGHT_ENABLE   = no

# OLED display support
OLED_ENABLE = yes

# Optional extras
NKRO_ENABLE = yes
WPM_ENABLE = yes
MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
COMBO_ENABLE = yes
TAP_DANCE_ENABLE = yes
CAPS_WORD_ENABLE = yes
LEADER_ENABLE = yes
SPACE_CADET_ENABLE = yes
SWAP_HANDS_ENABLE = yes
# Unicode: use UNICODEMAP (modern) only; these are mutually exclusive in QMK
UNICODE_ENABLE = no
UNICODEMAP_ENABLE = yes
# UCIS can be used instead of UNICODEMAP, but not together. Leave disabled by default.
# UCIS_ENABLE = yes
AUTO_SHIFT_ENABLE = yes
GRAVE_ESC_ENABLE = yes
KEY_LOCK_ENABLE = yes
DEFERRED_EXEC_ENABLE = yes
DYNAMIC_TAPPING_TERM_ENABLE = yes
MAGIC_ENABLE = yes
LTO_ENABLE = yes

# Split settings
SPLIT_KEYBOARD = yes
SPLIT_TRANSPORT_MIRROR = yes
