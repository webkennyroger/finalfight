import os
import sys
from PIL import Image

CANVAS_W = 64
CANVAS_H = 96

def main():
    src = sys.argv[1]
    dst = sys.argv[2]
    frames = int(sys.argv[3])

    img = Image.open(src)
    w, h = img.size
    frame_w = w // frames

    out = Image.new("P", (CANVAS_W, CANVAS_H * frames), 0)
    palette = img.getpalette()
    if palette:
        out.putpalette(palette[:768])

    for i in range(frames):
        sx = i * frame_w
        crop = img.crop((sx, 0, sx + frame_w, h))
        cx = (frame_w - CANVAS_W) // 2
        crop64 = crop.crop((cx, 0, cx + CANVAS_W, h))
        y_offset = (CANVAS_H - h) // 2
        out.paste(crop64, (0, i * CANVAS_H + y_offset))

    os.makedirs(os.path.dirname(dst), exist_ok=True)
    out.save(dst)
    print(f"{src} -> {dst} ({CANVAS_W}x{CANVAS_H*frames})")


if __name__ == "__main__":
    main()
