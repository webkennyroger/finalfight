import os
import sys
from PIL import Image

CANVAS_W = 64
CANVAS_H = 96
BLOCK = 16

def process_vertical_strip(path):
    img = Image.open(path)
    w, h = img.size
    nframes = h // CANVAS_H
    print(f"  {path}: {w}x{h}, {nframes} frames")
    out = Image.new('P', (CANVAS_W, h), 0)
    out.putpalette(img.getpalette())
    for f in range(nframes):
        sy = f * CANVAS_H
        frame = img.crop((0, sy, CANVAS_W, sy + CANVAS_H))
        out.paste(frame, (0, sy))
    out.save(path)
    print(f"    -> saved {CANVAS_W}x{CANVAS_H*nframes}")

def process_idle(path):
    img = Image.open(path)
    w, h = img.size
    print(f"  {path}: {w}x{h}")
    out = Image.new('P', (CANVAS_W, h), 0)
    out.putpalette(img.getpalette())
    frame = img.crop((0, 0, CANVAS_W, h))
    out.paste(frame, (0, 0))
    out.save(path)
    print(f"    -> saved {CANVAS_W}x{h}")

def process_walk(path):
    img = Image.open(path)
    w, h = img.size
    frame_w = w // 6
    print(f"  {path}: {w}x{h}, frame_width={frame_w}")
    nframes = 6
    out = Image.new('P', (CANVAS_W, h), 0)
    out.putpalette(img.getpalette())
    for f in range(nframes):
        sx = f * frame_w
        frame = img.crop((sx, 0, sx + CANVAS_W, h))
        out.paste(frame, (0, f * CANVAS_H))
    out.save(path)
    print(f"    -> saved {CANVAS_W}x{CANVAS_H*nframes}")

def main():
    base = "assets/sprites/guy"
    animations = [
        ("idle/guy_idle.png", process_idle),
        ("walk/guy_walk.png", process_walk),
        ("punch/guy_punch.png", process_vertical_strip),
        ("kick/guy_kick.png", process_vertical_strip),
        ("damage/guy_damage.png", process_vertical_strip),
        ("grab/guy_grab.png", process_vertical_strip),
        ("jump/guy_jump.png", process_vertical_strip),
        ("jump_kick/guy_jump_kick.png", process_vertical_strip),
    ]
    for rel, func in animations:
        path = os.path.join(base, rel)
        if os.path.exists(path):
            func(path)
        else:
            print(f"  {path}: NOT FOUND")

if __name__ == "__main__":
    main()
