"""Download TensorFlow detection models into PROJECT_ROOT/models."""
import shutil
import tarfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MODELS = ROOT / "models"
CODE = ROOT / "code"

DOWNLOADS = [
    (
        "https://storage.googleapis.com/download.tensorflow.org/models/object_detection/"
        "ssd_mobilenet_v2_coco_2018_03_29.tar.gz",
        "frozen_inference_graph.pb",
        MODELS / "ssd_mobilenet_v2_coco_2018_03_29.pb",
    ),
    (
        "https://storage.googleapis.com/download.tensorflow.org/models/object_detection/"
        "mask_rcnn_inception_v2_coco_2018_01_28.tar.gz",
        "frozen_inference_graph.pb",
        MODELS / "mask_rcnn_inception_v2_coco_2018_01_28" / "frozen_inference_graph.pb",
    ),
]

CONFIG_FILES = [
    "ssd_mobilenet_v2_coco_2018_03_29.pbtxt",
    "mask_rcnn_inception_v2_coco_2018_01_28.pbtxt",
    "mscoco_labels.names",
    "colors.txt",
]


def copy_configs():
    MODELS.mkdir(parents=True, exist_ok=True)
    for name in CONFIG_FILES:
        src = CODE / name
        dst = MODELS / name
        if src.exists() and not dst.exists():
            shutil.copy2(src, dst)
            print(f"copied config {name}")


def download_one(url: str, member: str, dest: Path):
    dest.parent.mkdir(parents=True, exist_ok=True)
    if dest.exists():
        print(f"skip existing {dest.name}")
        return
    archive = MODELS / (Path(url).name)
    print(f"downloading {url}")
    urllib.request.urlretrieve(url, archive)
    with tarfile.open(archive, "r:gz") as tf:
        for m in tf.getmembers():
            if m.name.endswith(member):
                f = tf.extractfile(m)
                if f:
                    dest.write_bytes(f.read())
                    print(f"wrote {dest}")
                    break
    archive.unlink(missing_ok=True)


def main():
    copy_configs()
    for url, member, dest in DOWNLOADS:
        download_one(url, member, dest)
    print("models ready under", MODELS)


if __name__ == "__main__":
    main()
