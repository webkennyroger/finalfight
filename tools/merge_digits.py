import sys

if __name__ == '__main__':
    digits_pic = sys.argv[1]  # digits.pic (10 tiles, 320 bytes)
    hud_pic    = sys.argv[2]  # hud.pic (160 tiles, 5120 bytes)

    with open(digits_pic, 'rb') as f:
        digits = bytearray(f.read())

    with open(hud_pic, 'rb') as f:
        hud = bytearray(f.read())

    # Append digit tiles to HUD
    hud.extend(digits)

    with open(hud_pic, 'wb') as f:
        f.write(hud)

    print(f"Merged: HUD {len(hud[:-320])} + digits 320 = {len(hud)} bytes ({len(hud)//32} tiles)")
