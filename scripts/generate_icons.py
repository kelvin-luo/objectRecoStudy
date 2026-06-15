"""Generate application icon (res/objectRecoStudy.ico)."""
from pathlib import Path

try:
    from PIL import Image, ImageDraw
except ImportError:
    print("Pillow not installed; skip icon generation.")
    raise SystemExit(0)

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "res" / "objectRecoStudy.ico"

def make_frame(size):
    img = Image.new("RGBA", (size, size), (30, 58, 95, 255))
    d = ImageDraw.Draw(img)
    margin = size // 8
    d.rounded_rectangle(
        [margin, margin, size - margin, size - margin],
        radius=size // 6,
        fill=(52, 120, 190, 255),
        outline=(220, 235, 255, 255),
        width=max(1, size // 32),
    )
    cx, cy = size // 2, size // 2
    r = size // 5
    d.ellipse([cx - r, cy - r, cx + r, cy + r], outline=(255, 210, 80, 255), width=max(1, size // 24))
    d.rectangle([cx - r // 2, cy + r // 3, cx + r // 2, cy + r], fill=(255, 210, 80, 200))
    return img

sizes = [16, 32, 48, 64, 128, 256]
frames = [make_frame(s) for s in sizes]
OUT.parent.mkdir(parents=True, exist_ok=True)
frames[0].save(OUT, format="ICO", sizes=[(s, s) for s in sizes])
print(f"Wrote {OUT}")
