"""Append font tiles (digits + letters) to hud.pic."""
import sys

if __name__ == '__main__':
    font_pic = sys.argv[1]
    hud_pic  = sys.argv[2]

    with open(font_pic, 'rb') as f:
        font = bytearray(f.read())
    with open(hud_pic, 'rb') as f:
        hud = bytearray(f.read())

    hud.extend(font)
    with open(hud_pic, 'wb') as f:
        f.write(hud)

    n = len(font) // 32
    print(f"Merged {n} font tiles ({len(font)} bytes) into {hud_pic} ({len(hud)} bytes total)")
