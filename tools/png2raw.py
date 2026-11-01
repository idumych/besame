#!/usr/bin/env python3
# Copyright 2026 the BeSame contributors
# SPDX-License-Identifier: MIT
"""Converts the PNG pictures in art/ to raw resources the app reads without
the Translation Kit (Haiku arm64 nightlies ship no translators at all).

    python3 tools/png2raw.py art/balls.png art/logo.png art/gameover.png

Writes art/<name>.raw next to each PNG: the magic "BSRW", then width and
height as little-endian uint32, then the pixels as B, G, R, A bytes, row by
row (B_RGBA32's memory layout). Needs Pillow. The .raw files are committed,
so building the app doesn't need Python.
"""
import struct
import sys

from PIL import Image

for png in sys.argv[1:]:
    im = Image.open(png).convert("RGBA")
    w, h = im.size
    r, g, b, a = im.split()
    data = Image.merge("RGBA", (b, g, r, a)).tobytes()
    out = png[:-4] + ".raw"
    with open(out, "wb") as f:
        f.write(b"BSRW" + struct.pack("<II", w, h) + data)
    print(out, w, h)
