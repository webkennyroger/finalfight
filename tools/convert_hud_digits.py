"""
Read hud_digits.png (128x32, digits 0-9 as 16x16 white-on-transparent).
Extract each digit, split into 4x8x8 tiles, encode as SNES 2bpp.
Output: assets/hud/hud_digits.pic (40 tiles, 640 bytes)
Palette: index 2 for white, index 0 for transparent.
"""
from PIL import Image
import os, sys

DIGIT_W = 16
DIGIT_H = 16
TILE_SZ = 8
COLS = 8

def encode_tile(pixels, ox, oy):
    tile = bytearray(16)
    for row in range(TILE_SZ):
        b0 = 0
        b1 = 0
        for col in range(TILE_SZ):
            px = ox + col
            py = oy + row
            r, g, b, a = pixels[px, py]
            pal = 2 if (a > 0 and r > 128 and g > 128 and b > 128) else 0
            bit = 7 - col
            b0 |= (pal & 1) << bit
            b1 |= ((pal >> 1) & 1) << bit
        tile[row * 2    ] = b0 & 0xFF
        tile[row * 2 + 1] = b1 & 0xFF
    return tile

def main():
    out_dir = os.path.join(os.path.dirname(__file__), '..', 'assets', 'hud')
    png_path = os.path.join(out_dir, 'hud_digits.png')
    out_path = os.path.join(out_dir, 'hud_digits.pic')

    img = Image.open(png_path).convert('RGBA')
    pixels = img.load()
    w, h = img.size

    tiles = bytearray()
    for idx in range(10):
        col = idx if idx < 6 else idx - 6
        row = 0 if idx < 6 else 1
        ox = col * DIGIT_W
        oy = row * DIGIT_H
        tiles.extend(encode_tile(pixels, ox,       oy))
        tiles.extend(encode_tile(pixels, ox + 8,   oy))
        tiles.extend(encode_tile(pixels, ox,       oy + 8))
        tiles.extend(encode_tile(pixels, ox + 8,   oy + 8))

    with open(out_path, 'wb') as f:
        f.write(tiles)

    print(f'Generated {out_path}: {len(tiles)} bytes ({len(tiles)//16} tiles, digits 0-9)')
    return 0

if __name__ == '__main__':
    sys.exit(main())
