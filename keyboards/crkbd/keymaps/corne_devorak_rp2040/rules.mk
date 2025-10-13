# Target RP2040 Pro Micro-style boards
CONVERT_TO = sparkfun_pm2040

# Feature set
CONSOLE_ENABLE = no

# IMPORTANT: You cannot use RGBLIGHT and RGB Matrix (WS2812) at the same time.
# We'll use RGB Matrix for per-key/underglow because that provide better 
# animation and as we are using RP2040 they are smoothly handled.
RGB_MATRIX_ENABLE = yes
RGBLIGHT_ENABLE = no

# OLED display support
OLED_ENABLE = yes

# Optional extras
# NKRO_ENABLE = yes
WPM_ENABLE = yes

# Split settings
SPLIT_KEYBOARD = yes
