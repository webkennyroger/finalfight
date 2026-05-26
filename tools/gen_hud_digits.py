"""
Generate digit tiles matching the HUD art style (Final Fight arcade).
Digit tiles use palette index 2 (white) for the digit body,
palette index 0 for transparent background.
Output: assets/hud/digits.pic (10 tiles, 320 bytes, SNES 4bpp)
"""
import struct, os, sys

# A bold arcade-style digit set using palette 2 (white) on transparent (0)
# Each digit is 8x8 pixels drawn with thick strokes matching the HUD "99" style
DIGITS = [
    # 0
    [
        0,0,2,2,2,2,0,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,0,2,2,2,2,0,0,
    ],
    # 1
    [
        0,0,0,2,2,0,0,0,
        0,2,2,2,2,0,0,0,
        0,0,0,2,2,0,0,0,
        0,0,0,2,2,0,0,0,
        0,0,0,2,2,0,0,0,
        0,0,0,2,2,0,0,0,
        0,0,0,2,2,0,0,0,
        0,2,2,2,2,2,2,0,
    ],
    # 2
    [
        0,0,2,2,2,2,0,0,
        0,2,2,0,0,2,2,0,
        0,0,0,0,0,2,2,0,
        0,0,0,0,2,2,0,0,
        0,0,0,2,2,0,0,0,
        0,0,2,2,0,0,0,0,
        0,2,2,0,0,0,0,0,
        0,2,2,2,2,2,2,0,
    ],
    # 3
    [
        0,0,2,2,2,2,0,0,
        0,2,2,0,0,2,2,0,
        0,0,0,0,0,2,2,0,
        0,0,0,2,2,2,0,0,
        0,0,0,0,0,2,2,0,
        0,0,0,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,0,2,2,2,2,0,0,
    ],
    # 4
    [
        0,0,0,0,2,2,0,0,
        0,0,0,2,2,2,0,0,
        0,0,2,0,2,2,0,0,
        0,2,2,0,2,2,0,0,
        0,2,2,2,2,2,2,0,
        0,0,0,0,2,2,0,0,
        0,0,0,0,2,2,0,0,
        0,0,0,0,2,2,0,0,
    ],
    # 5
    [
        0,2,2,2,2,2,2,0,
        0,2,2,0,0,0,0,0,
        0,2,2,0,0,0,0,0,
        0,2,2,2,2,2,0,0,
        0,0,0,0,0,2,2,0,
        0,0,0,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,0,2,2,2,2,0,0,
    ],
    # 6
    [
        0,0,2,2,2,2,0,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,0,0,0,
        0,2,2,2,2,2,0,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,0,2,2,2,2,0,0,
    ],
    # 7
    [
        0,2,2,2,2,2,2,0,
        0,0,0,0,0,2,2,0,
        0,0,0,0,0,2,2,0,
        0,0,0,0,2,2,0,0,
        0,0,0,2,2,0,0,0,
        0,0,0,2,2,0,0,0,
        0,0,0,2,2,0,0,0,
        0,0,0,2,2,0,0,0,
    ],
    # 8
    [
        0,0,2,2,2,2,0,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,0,2,2,2,2,0,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,0,2,2,2,2,0,0,
    ],
    # 9
    [
        0,0,2,2,2,2,0,0,
        0,2,2,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,0,2,2,2,2,2,0,
        0,0,0,0,0,2,2,0,
        0,0,0,0,0,2,2,0,
        0,2,2,0,0,2,2,0,
        0,0,2,2,2,2,0,0,
    ],
]

def encode_tile(pixels):
    """Encode 64-pixel list to SNES 4bpp tile (32 bytes)."""
    tile = bytearray(32)
    for row in range(8):
        b0 = 0
        b1 = 0
        for col in range(8):
            pal = pixels[row * 8 + col]
            bit = 7 - col
            b0 |= (pal & 1) << bit
            b1 |= ((pal >> 1) & 1) << bit
        tile[row * 2] = b0
        tile[row * 2 + 1] = b1
    return tile

def main():
    out_dir = os.path.join(os.path.dirname(__file__), '..', 'assets', 'hud')
    out_file = os.path.join(out_dir, 'digits.pic')

    tiles = bytearray()
    for d in range(10):
        tiles.extend(encode_tile(DIGITS[d]))

    with open(out_file, 'wb') as f:
        f.write(tiles)

    print(f'Generated {out_file}: {len(tiles)} bytes ({len(tiles)//32} tiles)')
    return 0

if __name__ == '__main__':
    sys.exit(main())
