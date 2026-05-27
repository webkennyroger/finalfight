"""
Ensures hud.pal has white (#7FFF) at index 2 for font/digit tiles.
If white exists elsewhere in the palette, swap it with index 2.
If white doesn't exist, set index 2 to white directly.
"""
import sys, os

def main():
    pal_path = os.path.join(os.path.dirname(__file__), '..', 'assets', 'hud', 'hud.pal')

    with open(pal_path, 'rb') as f:
        data = bytearray(f.read())

    WHITE_BE = 0x7FFF  # big-endian 15-bit white
    WHITE_LE_LO = 0xFF  # little-endian low byte
    WHITE_LE_HI = 0x7F  # little-endian high byte

    idx2_lo = data[4]   # index 2, low byte
    idx2_hi = data[5]   # index 2, high byte
    idx2_val = idx2_lo | (idx2_hi << 8)

    if idx2_val == WHITE_BE:
        print("Index 2 is already white, no fix needed")
        return 0

    for i in range(0, min(len(data), 512), 2):
        lo = data[i]
        hi = data[i + 1]
        val = lo | (hi << 8)
        if val == WHITE_BE:
            white_idx = i // 2
            data[4] = WHITE_LE_LO
            data[5] = WHITE_LE_HI
            data[white_idx * 2] = idx2_lo
            data[white_idx * 2 + 1] = idx2_hi
            print(f"Swapped palette index 2 (#{idx2_val:04X}) with index {white_idx} (#{WHITE_BE:04X})")
            with open(pal_path, 'wb') as f:
                f.write(data)
            return 0

    data[4] = WHITE_LE_LO
    data[5] = WHITE_LE_HI
    with open(pal_path, 'wb') as f:
        f.write(data)
    print(f"Set palette index 2 to white (#{WHITE_BE:04X}) (was #{idx2_val:04X})")
    return 0

if __name__ == '__main__':
    sys.exit(main())
