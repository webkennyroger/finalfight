from PIL import Image
import sys, os

def main():
    img = Image.open("assets/hud/hud.png")
    w, h = img.size
    pixels = img.load()
    alpha = img.split()[3]

    # Fix tiles with >4 colors (gold -> yellow)
    gold_r, gold_g, gold_b = 224, 160, 0
    fixed = 0
    for ty in range(0, min(h, 40), 8):
        for tx in range(0, min(w, 256), 8):
            colors = set()
            for y in range(ty, ty+8):
                for x in range(tx, tx+8):
                    if x < w and y < h:
                        a = alpha.getpixel((x, y))
                        if a > 0:
                            r, g, b, _ = pixels[x, y]
                            colors.add((r, g, b))
            if len(colors) > 4:
                for y in range(ty, ty+8):
                    for x in range(tx, tx+8):
                        if x < w and y < h:
                            r, g, b, a = pixels[x, y]
                            if a > 0 and r == gold_r and g == gold_g and b == gold_b:
                                pixels[x, y] = (255, 255, 0, 255)
                fixed += 1

    # Crop to 256x40 and paste on black background
    crop = img.crop((0, 0, 256, 40))
    bg = Image.new("RGB", (256, 40), (0, 0, 0))
    if crop.mode == "RGBA":
        bg.paste(crop, mask=crop.split()[3])
    else:
        bg.paste(crop)

    # Build exact palette from remaining colors
    all_colors = set()
    bg_pixels = bg.load()
    for y in range(40):
        for x in range(256):
            r, g, b = bg_pixels[x, y]
            all_colors.add((r, g, b))

    colors_list = [(0, 0, 0)]
    color_to_idx = {(0, 0, 0): 0}
    for c in sorted(all_colors):
        if c not in color_to_idx:
            color_to_idx[c] = len(colors_list)
            colors_list.append(c)

    # Create indexed image
    indexed = Image.new("P", (256, 40))
    flat_pal = []
    for c in colors_list[:256]:
        flat_pal.extend(c)
    flat_pal += [0] * (256 - len(colors_list)) * 3
    indexed.putpalette(flat_pal)

    for y in range(40):
        for x in range(256):
            r, g, b = bg_pixels[x, y]
            idx = color_to_idx.get((r, g, b), 0)
            indexed.putpixel((x, y), idx)

    indexed.save("assets/hud/hud.bmp")
    print(f"Saved assets/hud/hud.bmp ({fixed} tiles fixed)")
    return 0

if __name__ == "__main__":
    sys.exit(main())
