import os
import sys
from PIL import Image

CANVAS_W = 64
CANVAS_H = 96

ANIMATIONS = [
    ("idle/guy_idle.png", 1),
    ("walk/guy_walk.png", 8),
    ("punch/guy_punch.png", 4),
    ("kick/guy_kick.png", 4),
    ("damage/guy_damage.png", 3),
    ("grab/guy_grab.png", 3),
    ("jump/guy_jump.png", 4),
    ("jump_kick/guy_jump_kick.png", 4),
]


def visible_bbox(frame_img, bg_index):
    pixels = frame_img.load()
    w, h = frame_img.size
    min_x, min_y = w, h
    max_x, max_y = -1, -1

    for y in range(h):
        for x in range(w):
            if pixels[x, y] != bg_index:
                if x < min_x:
                    min_x = x
                if y < min_y:
                    min_y = y
                if x > max_x:
                    max_x = x
                if y > max_y:
                    max_y = y

    if max_x < 0:
        return None
    return min_x, min_y, max_x + 1, max_y + 1


def paste_frame(out, frame_img, y, center_x=False):
    if not center_x:
        out.paste(frame_img, (0, y))
        return

    bg_index = frame_img.getpixel((0, 0))
    bbox = visible_bbox(frame_img, bg_index)
    if bbox is None:
        return

    min_x, _, max_x, _ = bbox
    content = frame_img.crop((min_x, 0, max_x, CANVAS_H))
    x = (CANVAS_W - content.size[0]) // 2
    out.paste(content, (x, y))


def normalize_strip(src_path, dst_path, frames):
    img = Image.open(src_path)
    w, h = img.size
    center_x = "walk" in src_path.replace("\\", "/").lower()

    out = Image.new("P", (CANVAS_W, CANVAS_H * frames), 0)
    palette = img.getpalette()
    if palette:
        out.putpalette(palette)

    if w >= frames and h >= CANVAS_H:
        frame_w = w // frames
        for frame in range(frames):
            sx = frame * frame_w
            crop = img.crop((sx, 0, sx + min(frame_w, CANVAS_W), CANVAS_H))
            paste_frame(out, crop, frame * CANVAS_H, center_x)
    elif w >= CANVAS_W and h >= CANVAS_H * frames:
        for frame in range(frames):
            sy = frame * CANVAS_H
            crop = img.crop((0, sy, CANVAS_W, sy + CANVAS_H))
            paste_frame(out, crop, frame * CANVAS_H, center_x)
    else:
        raise ValueError(
            f"{src_path}: expected horizontal {CANVAS_W * frames}x{CANVAS_H} "
            f"or vertical {CANVAS_W}x{CANVAS_H * frames}, got {w}x{h}"
        )

    os.makedirs(os.path.dirname(dst_path), exist_ok=True)
    out.save(dst_path)
    print(f"{src_path} -> {dst_path} ({CANVAS_W}x{CANVAS_H * frames})")


def normalize_all():
    src_base = os.path.join("assets", "sprites", "guy")
    dst_base = os.path.join("assets", "build", "sprites", "guy")

    for rel_path, frames in ANIMATIONS:
        src_path = os.path.join(src_base, rel_path)
        dst_path = os.path.join(dst_base, rel_path)
        if os.path.exists(src_path):
            normalize_strip(src_path, dst_path, frames)
        else:
            print(f"{src_path}: NOT FOUND")


def main():
    if len(sys.argv) == 4:
        normalize_strip(sys.argv[1], sys.argv[2], int(sys.argv[3]))
        return

    normalize_all()


if __name__ == "__main__":
    main()
