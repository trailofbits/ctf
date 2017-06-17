# Forensics

<script async class="speakerdeck-embed" data-id="424ae340bbfc013182c64af3413e5309" data-ratio="1.33333333333333" src="//speakerdeck.com/assets/embed.js"></script>

In a CTF context, "Forensics" challenges can include file format analysis, steganography, memory dump analysis, or network packet capture analysis. Any challenge to examine and process a hidden piece of information out of static data files (as opposed to executable programs or remote servers) could be considered a Forensics challenge (unless it involves cryptography, in which case it probably belongs in the Crypto category). 

Forensics is a broad CTF category that does not map well to any particular job role in the security industry, although some challenges model the kinds of tasks seen in Incident Response (IR). Even in IR work, computer forensics is usually the domain of law enforcement seeking evidentiary data and attribution, rather than the commercial incident responder who may just be interested in expelling an attacker and/or restoring system integrity.

Unlike most CTF forensics challenges, a real-world computer forensics task would hardly ever involve unraveling a scheme of cleverly encoded bytes, hidden data, mastroshka-like files-within-files, or other such brain-teaser puzzles. One would typically not bust a criminal case by carefully reassembling a corrupted PNG file, revealing a photo of a QR code that decodes to a password for a zip archive containing an NES rom that when played will output the confession. Rather, real-world forensics typically requires that a practictioner find indirect evidence of maliciousness: either the traces of an attacker on a system, or the traces of "insider threat" behavior. Real-world computer forensics is largely about knowing where to find incriminating clues in logs, in memory, in filesystems/registries, and associated file and filesystem metadata. Also, network (packet capture) forensics is more about metadata analysis than content analysis, as most network sessions are TLS-encrypted between endpoints now.

This disconnect between the somewhat artificial puzzle-game CTF "Forensics" and the way that forensics is actually done in the field might be why this category does not receive as much attention as the vulnerability-exploitation style challenges. It may also lack the "black hat attacker" appeal that draws many players to participate in CTFs. Regardless, many players enjoy the variety and novelty in CTF forensics challenges. It can also be a more beginner friendly category, in which the playing field is evened out by the fact that there are no $5,000 professional tools like IDA Pro Ultimate Edition with Hex-Rays Decompiler that would give a huge advantage to some players but not others, as is the case with executable analysis challenges.

## Requisite Skills

For solving forensics CTF challenges, the three most useful abilities are probably:
* Knowing a scripting language (e.g., Python)
* Knowing how to manipulate binary data (byte-level manipulations) in that language
* Recognizing formats, protocols, structures, and encodings

And of course, like most CTF play, the ideal environment is a Linux system with – occasionally – Windows in a VM. MacOS is not a bad environment to substitute for Linux, if you can accept that some open-source tools may not install or compile correctly.

## Manipulating Binary Data in Python

Assuming you have already picked up some Python programming, you still may not know how to effectively work with binary data. Low-level languages like C might be more naturally suited for this task, but Python's many useful packages from the open-source community outweigh its learning curve for working with binary data.

TODO: crash-course in manipulating binary data in Python

## Common Challenge Types, Strategies

What follows is a high-level overview of some common tasks in forensics CTF challenges, and some recommended tools for tackling these problems.

### File format identification (and "magic bytes")

Almost every forensics challenge will involve a file, usually without any context that would give you a guess as to what the file is. Filetypes, as a concept for users, have historically been indicated either with filetype extensions (e.g., readme.md for MarkDown), MIME types (as on the web, with Content-Type headers), or with metadata stored in the filesystem (as with the `mdls` command in MacOS). In a CTF, part of the game is to identify the file ourselves, using a heuristic approach.

The traditional heuristic for identifying filetypes on UNIX is `libmagic`, which is a library for identifying so-called "magic numbers" or "magic bytes," the unique identifying marker bytes in filetype headers. The `libmagic` libary is the basis for the `file` command.

```
$ file screenshot.png 
screenshot.png: PNG image data, 1920 x 1080, 8-bit/color RGBA, non-interlaced
```

Keep in mind that heuristics, and tools that employ them, can be easily fooled. Because it is a CTF, you may be presented with a file that has been intentionally crafted to mislead `file`. Also, if a file contains another file embedded somewhere inside it, the `file` command is only going to identify the containing filetype. In scenarios such as these you may need to examine the file content more closely.

[TrID](http://mark0.net/soft-trid-e.html) is a more sophisticated version of `file`. Although it's closed-source, it's free and works across platforms. It also uses an identification heuristic, but with certainty percentages. Its advantage is its larger set of known filetypes that include a lot of proprietary and obscure formats seen in the real world.

### File carving

Files-within-files is a common trope in forensics CTF challenges, and also in embedded systems' firmware where primitive or flat filesystems are common. The term for identifying a file embedded in another file and extracting it is "file carving." One of the best tools for this task is the firmware analysis tool [binwalk](https://github.com/devttys0/binwalk). 

[scalpel](https://github.com/sleuthkit/scalpel), now a part of SleuthKit (discussed further under [Filesystems](###Filesystems)) is another tool for file-carving, formerly known as Foremost.

To manually extract a sub-section of a file (from a known offset to a known offset), you can use the `dd` command. Many hex-editors also offer the ability to copy bytes and paste them as a new file, so you don't need to study the offsets.

Although the above tools should suffice, in some cases you may need to programmatically extract a sub-section of a file using Python, using things like Python's re or regex modules to identify magic bytes, and the zlib module to extract zlib streams.

### Initial analysis

At first you may not have any leads, and need to explore the challenge file at a high-level for a clue toward what to look at next. Some of the useful commands to know are `strings` to search for all plain-text strings in the file, `grep` to search for particular strings, [`bgrep`](https://github.com/tmbinc/bgrep) to search for non-text data patterns, and `hexdump`.

Example of using strings to find ASCII strings, with file offsets:
```
$ strings -o screenshot.png
     12 IHDR
     36 $iCCPICC Profile
     88 U2EI4HB
...
     767787 IEND
```

Unicode strings, if they are UTF-8, might show up in the search for ASCII strings. But to search for other encodings, see the documentation for the `-e` flag. Beware the many encoding pitfalls of `strings`: some caution against its use in forensics at all, but for simple tasks it still has its place.

Example of searching for the PNG magic bytes in a PNG file:
```
$ bgrep 89504e47 screenshot.png 
screenshot.png: 00000000
```

Example of using `hexdump`:
```
$ hexdump -C screenshot.png | less
00000000  89 50 4e 47 0d 0a 1a 0a  00 00 00 0d 49 48 44 52  |.PNG........IHDR|
00000010  00 00 05 ca 00 00 02 88  08 06 00 00 00 40 3d c9  |.............@=.|
00000020  a4 00 00 18 24 69 43 43  50 49 43 43 20 50 72 6f  |....$iCCPICC Pro|
00000030  66 69 6c 65 00 00 58 85  95 79 09 38 55 5d f8 ef  |file..X..y.8U]..|
00000040  da 67 9f c9 71 0c c7 41  e6 79 9e 87 0c 99 e7 39  |.g..q..A.y.....9|
:
```

The advantage of `hexdump` is not that it is the best hex-editor (it's not), but that you can pipe output of other commands directly into hexdump, and/or pipe its output to grep, or format its output using format strings.

Example of using `hexdump` format strings to output the first 50 bytes of a file as a series of 64-bit integers in hex:
```
$ hexdump -n 50 -e '"0x%08x "' screenshot.png
0x474e5089 0x0a1a0a0d 0x0d000000 0x52444849 0xca050000 0x88020000 0x00000608 0xc93d4000 0x180000a4 0x43436924 0x43434950 0x6f725020 0x00006966
```

[Other uses of the hexdump command.](http://www.commandlinefu.com/commands/using/hexdump)

### Binary-as-text encodings
* https://en.wikipedia.org/wiki/Binary-to-text_encoding
* http://www.yellowpipe.com/yis/tools/encrypter/index.php
* ASCII-encoded hexadecimal, identifiable by its charset (0-9, A-F)
* Base64, identifiable by its charset and the "=" suffix
* the xxd command

### Image formats
* EXIF data (see exiftool)
* Compression
* Steganography (see steghide; https://github.com/zed-0xff/zsteg; http://www.caesum.com/handbook/Stegsolve.jar)
* PNG (can be opened in Wireshark; pngcheck)
* Gimp
* Python Image Library

### Other file formats
* PDF (qpdf package, PeepDF)
* Kaitai.io
* File-format-aware / templated hex-editors like 010 Editor
* "zip -FF" command

### Filesystems
* Strategies for finding a needle in a haystack of data
* mounting a filesystem
* The Sleuth Kit
* hidden volumes
* undeleting files (see extundelete)
* embedded device filesystems (squashfs; see firmware-mod-kit or binwalk)
* http://www.cgsecurity.org/wiki/TestDisk

### PCAP analysis
* Wireshark (and tshark) and using filters, capinfos command
* dpkt Python package for pcap manipulation
* http://f00l.de/hacking/pcapfix.php

### Memory dump analysis
* Volatility

## Common File formats

It would be impossible to prepare for every possible data format, but there are some that are especially popular in CTFs. If you were prepared with tools for analyzing the following, you would be prepared for the majority of Forensics challenges:
* Archive files (especially zip, tar)
* Filesystem images (especially EXT4)
* PCAP and PCAPNG
* PNG
* PDF
* Video (especially MP4)
* Audio (especially WAV, MP3; see Audacity)
* Microsoft's Office formats (RTF, OLE, OOXML; see oletools)

Some of the harder CTF challenges pride themselves on requiring players to analyze an especially obscure format for which no publicly available tools exist. You will need to learn to quickly locate documentation and tools for unfamiliar formats.
