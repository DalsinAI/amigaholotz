#!/usr/bin/env python3
"""msadpcm2pcm.py DIR..: rewrites, in place, every Microsoft ADPCM WAV
(format 2) under DIR as a 16-bit PCM WAV with the same rate and channels.

Why: the game's sounds (res/sound) are MS ADPCM, and the program's
SDL_mixer stand-in (amiga/sdl_mixer_lite.c) reads PCM (and IMA ADPCM)
only. Run by amiga/Makefile.amigaos on the drawer's copy of res/.
The AmiCradle Team. GPL 2 or later, like Holotz's Castle."""
import os
import struct
import sys

ADAPT = [230, 230, 230, 230, 307, 409, 512, 614, 768, 614, 512, 409, 307, 230, 230, 230]
COEF1 = [256, 512, 0, 192, 240, 460, 392]
COEF2 = [0, -256, 0, 64, 0, -208, -232]


def clamp(v):
    return -32768 if v < -32768 else 32767 if v > 32767 else v


def decode(data, channels, block_align, spb, coefs):
    out = []
    for b in range(0, len(data), block_align):
        blk = data[b:b + block_align]
        if len(blk) < 7 * channels:
            break
        p = 0
        pred = list(blk[p:p + channels]); p += channels
        delta = list(struct.unpack_from("<%dh" % channels, blk, p)); p += 2 * channels
        s1 = list(struct.unpack_from("<%dh" % channels, blk, p)); p += 2 * channels
        s2 = list(struct.unpack_from("<%dh" % channels, blk, p)); p += 2 * channels
        c1 = [coefs[min(x, len(coefs) - 1)][0] for x in pred]
        c2 = [coefs[min(x, len(coefs) - 1)][1] for x in pred]
        frames = [s2[:], s1[:]]
        nib = []
        for byte in blk[p:]:
            nib.append(byte >> 4)
            nib.append(byte & 15)
        want = (spb - 2) * channels
        for i, n in enumerate(nib[:want]):
            ch = i % channels
            sn = n - 16 if n & 8 else n
            pv = (s1[ch] * c1[ch] + s2[ch] * c2[ch]) // 256 + sn * delta[ch]
            pv = clamp(pv)
            s2[ch] = s1[ch]
            s1[ch] = pv
            delta[ch] = max(16, (ADAPT[n] * delta[ch]) // 256)
            if ch == 0:
                frames.append([pv] + [0] * (channels - 1))
            else:
                frames[-1][ch] = pv
        for f in frames:
            out.extend(f)
    return out


def convert(path):
    d = open(path, "rb").read()
    if d[:4] != b"RIFF" or d[8:12] != b"WAVE":
        return False
    i, fmt, data = 12, None, None
    while i + 8 <= len(d):
        cid = d[i:i + 4]
        n, = struct.unpack("<I", d[i + 4:i + 8])
        body = d[i + 8:i + 8 + n]
        if cid == b"fmt ":
            fmt = body
        elif cid == b"data":
            data = body
        i += 8 + n + (n & 1)
    if not fmt or data is None:
        return False
    tag, ch, rate, _, align, _bits = struct.unpack("<HHIIHH", fmt[:16])
    if tag != 2:
        return False
    spb, ncoef = struct.unpack("<HH", fmt[18:22])
    coefs = [struct.unpack_from("<hh", fmt, 22 + 4 * k) for k in range(ncoef)] or list(zip(COEF1, COEF2))
    pcm = decode(data, ch, align, spb, coefs)
    raw = struct.pack("<%dh" % len(pcm), *pcm)
    hdr = struct.pack("<HHIIHH", 1, ch, rate, rate * ch * 2, ch * 2, 16)
    out = b"RIFF" + struct.pack("<I", 4 + 8 + len(hdr) + 8 + len(raw)) + b"WAVE"
    out += b"fmt " + struct.pack("<I", len(hdr)) + hdr + b"data" + struct.pack("<I", len(raw)) + raw
    open(path, "wb").write(out)
    return True


if __name__ == "__main__":
    n = 0
    for top in sys.argv[1:]:
        for root, _dirs, files in os.walk(top):
            for f in files:
                if f.lower().endswith(".wav") and convert(os.path.join(root, f)):
                    n += 1
    print("msadpcm2pcm: %d files" % n)
