from PIL import Image
import os

DIGIT_W = 16
DIGIT_H = 16
COLS = 8
ROWS = 2
SHEET_W = COLS * DIGIT_W
SHEET_H = ROWS * DIGIT_H

FILL = (255, 255, 255, 255)
EMPTY = (0, 0, 0, 0)
OUTLINE = (0, 0, 0, 128)

DIGITS = {
    '0': [
        "  xxxxxxxxx  ",
        " xx      xx ",
        "xx  xxxx  xx",
        "xx xxxxxx xx",
        "xx xxxxxx xx",
        "xx xxxxxx xx",
        "xx xxxxxx xx",
        "xx xxxxxx xx",
        "xx xxxxxx xx",
        "xx xxxxxx xx",
        "xx xxxxxx xx",
        "xx  xxxx  xx",
        " xx      xx ",
        "  xxxxxxxxx  ",
        "            ",
        "            ",
    ],
    '1': [
        "      xx     ",
        "     xxx     ",
        "    xxxx     ",
        "   xxxxx     ",
        "     xx      ",
        "     xx      ",
        "     xx      ",
        "     xx      ",
        "     xx      ",
        "     xx      ",
        "     xx      ",
        "     xx      ",
        "   xxxxxx    ",
        "   xxxxxx    ",
        "            ",
        "            ",
    ],
    '2': [
        "  xxxxxxxxx  ",
        " xx      xx ",
        "xx        xx",
        "          xx",
        "          xx",
        "         xx ",
        "       xx   ",
        "      xx    ",
        "     xx     ",
        "    xx      ",
        "   xx       ",
        "  xx        ",
        " xx        ",
        "xxxxxxxxxxxx",
        "            ",
        "            ",
    ],
    '3': [
        "  xxxxxxxxx  ",
        " xx      xx ",
        "xx        xx",
        "          xx",
        "          xx",
        "       xxx  ",
        "   xxxxx    ",
        "       xxx  ",
        "          xx",
        "          xx",
        "          xx",
        "xx        xx",
        " xx      xx ",
        "  xxxxxxxxx  ",
        "            ",
        "            ",
    ],
    '4': [
        "       xx    ",
        "      xxx    ",
        "     xxxx    ",
        "    x xxx    ",
        "   xx xxx    ",
        "  xx  xxx    ",
        " xx   xxx    ",
        "xxxxxxxxxxxxx",
        "       xxx    ",
        "       xxx    ",
        "       xxx    ",
        "       xxx    ",
        "       xxx    ",
        "       xxx    ",
        "            ",
        "            ",
    ],
    '5': [
        "xxxxxxxxxxxx",
        "xx          ",
        "xx          ",
        "xx          ",
        "xx          ",
        "xx  xxxxxx  ",
        " xxx     xx ",
        "          xx",
        "          xx",
        "          xx",
        "          xx",
        "xx        xx",
        " xxx     xx ",
        "  xxxxxxxxx  ",
        "            ",
        "            ",
    ],
    '6': [
        "  xxxxxxxxx  ",
        " xx      xx ",
        "xx        xx",
        "xx          ",
        "xx          ",
        "xx  xxxxxx  ",
        "xxx      xx ",
        "xxx       xx",
        "xxx       xx",
        "xxx       xx",
        "xxx       xx",
        "xx        xx",
        " xx      xx ",
        "  xxxxxxxxx  ",
        "            ",
        "            ",
    ],
    '7': [
        "xxxxxxxxxxxx",
        "          xx",
        "          xx",
        "         xx ",
        "        xx  ",
        "        xx  ",
        "       xx   ",
        "       xx   ",
        "      xx    ",
        "      xx    ",
        "     xx     ",
        "     xx     ",
        "    xx      ",
        "    xx      ",
        "            ",
        "            ",
    ],
    '8': [
        "  xxxxxxxxx  ",
        " xx      xx ",
        "xx        xx",
        "xx        xx",
        "xx        xx",
        " xx      xx ",
        "  xxxxxxxxx  ",
        " xx      xx ",
        "xx        xx",
        "xx        xx",
        "xx        xx",
        "xx        xx",
        " xx      xx ",
        "  xxxxxxxxx  ",
        "            ",
        "            ",
    ],
    '9': [
        "  xxxxxxxxx  ",
        " xx      xx ",
        "xx        xx",
        "xx        xx",
        "xx        xx",
        "xx        xx",
        " xx      xx ",
        "  xxxxxxxxxx",
        "          xx",
        "          xx",
        "          xx",
        "xx        xx",
        " xx      xx ",
        "  xxxxxxxxx  ",
        "            ",
        "            ",
    ],
}

def main():
    out_dir = os.path.join(os.path.dirname(__file__), '..', 'assets', 'hud')
    os.makedirs(out_dir, exist_ok=True)
    out = os.path.join(out_dir, 'hud_digits.png')

    img = Image.new('RGBA', (SHEET_W, SHEET_H), (0, 0, 0, 0))
    pixels = img.load()

    layout = list('012345')

    for row in range(ROWS):
        for col in range(COLS):
            idx = row * COLS + col
            if idx >= 10:
                break
            digit = str(idx)
            pattern = DIGITS[digit]
            ox = col * DIGIT_W
            oy = row * DIGIT_H
            for py in range(DIGIT_H):
                for px in range(DIGIT_W):
                    if py < len(pattern) and px < len(pattern[py]):
                        ch = pattern[py][px]
                        if ch == 'x':
                            pixels[ox + px, oy + py] = FILL

    img.save(out)
    print(f'Generated {out}: {SHEET_W}x{SHEET_H}px, {len(layout)+4} digits (0-9)')
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main())
