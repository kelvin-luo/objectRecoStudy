# Minimal 32x32 ICO (blue app icon) — used when Pillow is unavailable.
import struct
from pathlib import Path

OUT = Path(__file__).resolve().parents[1] / "res" / "objectRecoStudy.ico"
OUT.parent.mkdir(parents=True, exist_ok=True)

# 32x32 32bpp BGRA bitmap (solid blue with lighter border)
w, h = 32, 32
pixels = bytearray()
for y in range(h):
    for x in range(w):
        border = x < 2 or y < 2 or x >= w - 2 or y >= h - 2
        pixels.extend((190, 120, 52, 255) if border else (235, 120, 52, 255))  # BGRA

bmp_header = struct.pack("<IIIHHIIIIII", 40, w, h * 2, 1, 32, 0, len(pixels), 0, 0, 0, 0)
and_mask = bytes(((w + 31) // 32) * 4 * h)
image_data = bmp_header + bytes(pixels) + and_mask

icon_dir = struct.pack("<HHH", 0, 1, 1)
entry = struct.pack("<BBBBHHII", w, h, 0, 0, 1, 32, len(image_data), 22)
OUT.write_bytes(icon_dir + entry + image_data)
print("Wrote fallback icon", OUT)
