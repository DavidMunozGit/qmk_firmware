# corne_devorak_rp2040

Personal Dvorak-like layout for Corne (crkbd) targeting RP2040 Pro Micro replacement.

Build:
- qmk compile -kb crkbd/rev1 -km corne_devorak_rp2040

Flash (RP2040 UF2):
- Put each half into UF2 boot mode (double-tap reset or BOOTSEL+reset)
- Copy the generated UF2 to the RPI-RP2 drive for each half
