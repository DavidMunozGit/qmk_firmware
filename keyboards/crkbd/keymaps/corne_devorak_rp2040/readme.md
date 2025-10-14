<!--
 @file readme.md
 @brief Overview and usage notes for the corne_devorak_rp2040 keymap.
-->

## corne_devorak_rp2040

Personal Dvorak layout for Corne (crkbd) using RP2040 Pro Micro-compatible controllers.

### Features
- Dvorak base layer + NAV, SYM, RGB layers
- RGB Matrix enabled (per-key + underglow); RGBlight disabled
- OLED UI with a simple Pac-Man text animation and WPM readout
- Split keyboard with WPM/OLED data mirrored between halves

### Build
- qmk compile -kb crkbd/rev1 -km corne_devorak_rp2040

This produces a UF2 suitable for RP2040 when used with the converter target (see `rules.mk`).

### Flash (RP2040 UF2)
1) Put each half into UF2 boot mode:
	- Double-tap reset, or hold BOOTSEL and press reset
2) Copy the generated UF2 to the RPI-RP2 drive for each half

### Notes
- RGB Matrix mode and HSV chosen on the RGB layer are set to be persistent.
- You can tweak RGB defaults and enabled effects in `config.h`.
