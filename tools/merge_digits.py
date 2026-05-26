import sys
import struct

def remap_pixel(val, old=1, new=2):
    """Swap palette index in 4bpp SNES tile data."""
    high = (val >> 4) & 0xF
    low = val & 0xF
    if high == old:
        high = new
    if low == old:
        low = new
    return (high << 4) | low

if __name__ == '__main__':
    digits_pic = sys.argv[1]  # digits.pic (16 tiles, 512 bytes)
    hud_pic    = sys.argv[2]  # hud.pic (160 tiles, 5120 bytes)

    with open(digits_pic, 'rb') as f:
        digits = bytearray(f.read())

    # Remap palette index 1 -> 2 (white in HUD palette)
    for i in range(len(digits)):
        digits[i] = remap_pixel(digits[i], 1, 2)

    # Only first 10 tiles of digits (320 bytes), discard padding
    digits = digits[:320]

    with open(hud_pic, 'rb') as f:
        hud = bytearray(f.read())

    # Append digit tiles to HUD
    hud.extend(digits)

    with open(hud_pic, 'wb') as f:
        f.write(hud)

    print(f"Merged: HUD {len(hud[:-320])} + digits 320 = {len(hud)} bytes ({len(hud)//32} tiles)")
