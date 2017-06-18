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

## Common Forensics Concepts and Tools

What follows is a high-level overview of some of the common concepts in forensics CTF challenges, and some recommended tools for performing common tasks.

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

Example of file-carving with `dd` from an file-offset of 1335205 for a length of 40668937 bytes:
```
$ dd if=./file_with_a_file_in_it.xxx of=./extracted_file.xxx bs=1 skip=1335205 count=40668937
```

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

Binary is 1's and 0's, but often is transmitted as text. It would be wasteful to transmit actual sequences of 101010101, so the data is first encoded using one of a variety of methods. This is what is referred to as [binary-to-text encoding]( https://en.wikipedia.org/wiki/Binary-to-text_encoding), a popular trope in CTF challenges. When doing a `strings` analysis of a file as discussed above, you may uncover this binary data encoded as text strings.

We mentioned that to excel at forensics CTF challenges, it is important to be able to recognize encodings. Some can be identifed at a glance, such as [Base64](https://en.wikipedia.org/wiki/Base64) encoded content, identifiable by its alphanumeric charset and its "=" padding suffix (when present). There are many [Base64 encoder/decoders](https://www.base64decode.org) online, or you can use the `base64` command:

```
$ echo aGVsbG8gd29ybGQh | base64 -D
hello world!
```

ASCII-encoded hexadecimal is also identifiable by its charset (0-9, A-F). ASCII characters themselves occupy a certain range of bytes (0x00 through 0x7f, see `man ascii`), so if you are examining a file and find a string like `68 65 6c 6c 6f 20 77 6f 72 6c 64 21`, it's important to notice the preponderance of 0x60's here: this is ASCII. Technically, it's text ("hello world!") encoded as ASCII (binary) encoded as hexadecimal (text again). Confused yet? 😉

There are [several sites](http://www.yellowpipe.com/yis/tools/encrypter/index.php) that provide online encoder-decoders for a variety of encodings. For a more local converter, try the `xxd` command.

Example of using `xxd` to do text-as-ascii-to-hex encoding:
```
$ echo hello world\! | xxd -p
68656c6c6f20776f726c64210a
```

## Common File formats

We've discussed the fundamental concepts and the tools for the more generic forensics tasks. Now, we'll discuss more specific categories of forensics challenges, and the recommended tools for analyzing challenges in each category.

It would be impossible to prepare for every possible data format, but there are some that are especially popular in CTFs. If you were prepared with tools for analyzing the following, you would be prepared for the majority of Forensics challenges:
* Archive files (ZIP, TGZ)
* Image file formats (JPG, GIF, BMP, PNG)
* Filesystem images (especially EXT4)
* Packet captures (PCAP, PCAPNG)
* Memory dumps
* PDF
* Video (especially MP4) or Audio (especially WAV, MP3)
* Microsoft's Office formats (RTF, OLE, OOXML)

Some of the harder CTF challenges pride themselves on requiring players to analyze an especially obscure format for which no publicly available tools exist. You will need to learn to quickly locate documentation and tools for unfamiliar formats. Many file formats are well-described in the public documentation you can find with a web search, but having some familiarity with the file format specifications will also help, so we include links to those here.

TODO: include links to file format specficiations

When analyzing file formats, a file-format-aware (a.k.a. templated) hex-editor like [010 Editor](http://www.sweetscape.com/010editor/) is invaluable. An open-source alternative has emerged called [Kaitai](http://kaitai.io). Additionally, a lesser-known feature of [the Wireshark network protocol analyzer](https://wiki.wireshark.org/FrontPage) is [its ability to analyze certain media file formats like GIF, JPG, and PNG](https://wiki.wireshark.org/MediaTypesFamily). All of these tools, however, are made to analyze non-corrupted and well-formatted files. Many CTF challenges task you with reconstructing a file based on missing or zeroed-out format fields, etc.

You also ought to check out the wonderful [file-formats illustrated visually](https://github.com/corkami/pics/tree/master/binary) by Ange Albertini.

### Archive files

Most CTF challenges are contained in a zip, 7z, rar, tar or tgz file, but only in a forensics challenge will the archive container file be a part of the challenge itself. Usually the goal here is to extract a file from a damaged archive, or find data embedded somewhere in an unused field (a common forensics challenge). Zip is the most common in the real world, and the most common in CTFs.

There are a handful of command-line tools for zip files that will be useful to know about.
* `unzip` will often output helpful information on why a zip will not decompress.
* `zipdetails -v` will provide in-depth information on the values present in the various fields of the format.
* `zipinfo` lists information about the zip file's contents, without extracting it.
* `zip -F input.zip --out output.zip` and `zip -FF input.zip --out output.zip` attempt to repair a corrupted zip file.
* [fcrackzip](https://github.com/hyc/fcrackzip) brute-force guesses a zip password (for passwords <7 characters or so).

[Zip file format specification](https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT)

One important security-related note about password-protected zip files is that they do not encrypt the filenames and original file sizes of the compressed files they contain, unlike password-protected RAR or 7z files.

Another note about zip cracking is that if you have an unencrypted/uncompressed copy of any one of the files that is compressed in the encrypted zip, you can perform a "plaintext attack" and crack the zip, as [detailed here](https://www.hackthis.co.uk/articles/known-plaintext-attack-cracking-zip-files), and explained in [this paper](https://www.cs.auckland.ac.nz/~mike/zipattacks.pdf). The newer scheme for password-protecting zip files (with AES-256, rather than "ZipCrypto") does not have this weakness.

### Image file format analysis

CTFs are supposed to be fun, and image files are good for containing hacker memes, so of course image files often appear in CTF challenges. Image file formats are complex and can be abused in many ways that make for interesting analysis puzzles involving metadata fields, lossy and lossless compression, checksums, steganography, or visual data encoding schemes.

The easy initial analysis step is to check an image file's metadata fields with [exiftool](http://www.sno.phy.queensu.ca/~phil/exiftool/). If an image file has been abused for a CTF, its EXIF might identify the original image dimensions, camera type, embedded thumbnail image, comments and copyright strings, GPS location coordinates, etc. There might be a gold mine of metadata, or there might be almost nothing. It's worth a look.

Example of exiftool output, truncated:
```
$ exiftool screenshot.png 
ExifTool Version Number         : 10.53
File Name                       : screenshot.png
Directory                       : .
File Size                       : 750 kB
File Modification Date/Time     : 2017:06:13 22:34:05-04:00
File Access Date/Time           : 2017:06:17 13:19:58-04:00
File Inode Change Date/Time     : 2017:06:13 22:34:05-04:00
File Permissions                : rw-r--r--
File Type                       : PNG
File Type Extension             : png
MIME Type                       : image/png
Image Width                     : 1482
Image Height                    : 648
Bit Depth                       : 8
Color Type                      : RGB with Alpha
Compression                     : Deflate/Inflate
...
Primary Platform                : Apple Computer Inc.
CMM Flags                       : Not Embedded, Independent
Device Manufacturer             : APPL
Device Model                    : 
...
Exif Image Width                : 1482
Exif Image Height               : 648
Image Size                      : 1482x648
Megapixels                      : 0.960
```

PNG files, in particular, are popular in CTF challenges, probably for their lossless compression suitable for hiding non-visual data in the image. PNG files can be dissected in Wireshark. To verify correcteness or attempt to repair corrupted PNGs you can use [pngcheck](http://libpng.org/pub/png/apps/pngcheck.html). If you need to dig into PNG a little deeper, the [pngtools](http://www.stillhq.com/pngtools/) package might be useful.

[Steganography](https://en.wikipedia.org/wiki/Steganography), the practice of concealing some amount of secret data within an unrelated data as its vessel (a.k.a. the "cover text"), is extraordinarily rare in the real world (made effectively obsolete by strong cryptography), but is another popular trope in CTF forensics challenges. Steganography could be implemented using any kind of data as the "cover text," but media file formats are ideal because they tolerate a certain amount of unnoticeable data loss (the same characteristic that makes lossy compression schemes possible). The difficulty with steganography is that extracting the hidden message requires not only a detection that steganography has been used, but also the exact [steganographic tool](https://en.wikipedia.org/wiki/Steganography_tools#Tools_comparison) used to embed it. Given a challenge file, if we suspect steganography, we must do at least a little guessing to check if it's present.

(see steghide; https://github.com/zed-0xff/zsteg; http://www.caesum.com/handbook/Stegsolve.jar)

* Gimp, screwing around with the Hue/Saturation/Luminance values, color channels
* ImageMagick tools http://www.imagemagick.org/script/index.php
* Python Image Library
* For QR codes, the qrtools module for Python

### Filesystems analysis
* Strategies for finding a needle in a haystack of data
* the `tree` command for a quick look at directory structure

Example of mounting a CD-ROM filesystem image:
```
mkdir /mnt/challenge
mount -t iso9660 challengefile /mnt/challenge
```

* The Sleuth Kit
* hidden volumes
* Windows' "alternate data streams" http://www.nirsoft.net/utils/alternate_data_streams.html
* undeleting files (see extundelete)
* embedded device filesystems (squashfs; see firmware-mod-kit or binwalk)
* http://www.cgsecurity.org/wiki/TestDisk

### Packet Capture (PCAP) file analysis
* Wireshark (and tshark) and using filters, capinfos command
* "packet carving", Wireshark's Extract Objects Feature (e.g., File -> Export Objects -> HTTP -> Save all)
* dpkt Python package for pcap manipulation
* http://f00l.de/hacking/pcapfix.php

### Memory dump analysis
* Volatility (specifying the relevant "profile")

### PDF file analysis

PDF is an extremely complicated document file format, with enough tricks and hiding places [to write about for years](https://www.sultanik.com/pocorgtfo/). This also makes it popular for CTF forensics challenges. The NSA wrote a guide to these hiding places in 2008 titled "Hidden Data and Metadata in Adobe PDF Files: Publication Risks and Countermeasures." It's no longer available at its original URL, but you can [find a copy here](http://www.itsecure.hu/library/file/Biztonsági%20útmutatók/Alkalmazások/Hidden%20Data%20and%20Metadata%20in%20Adobe%20PDF%20Files.pdf). Ange Albertini also keeps a wiki on GitHub of [PDF file format tricks](https://github.com/corkami/docs/blob/master/PDF/PDF.md).

The PDF format is partially plain-text, like HTML, but with many binary "objects" in the contents. Didier Stevens has written [good introductory material](https://blog.didierstevens.com/2008/04/09/quickpost-about-the-physical-and-logical-structure-of-pdf-files/) about the format. The binary objects can be compressed or even encrypted data, and include content in scripting languages like JavaScript or Flash. To display the structure of a PDF, you can either browse it with a text editor, or open it with a PDF-aware file-format editor like Origami.

[qpdf](https://github.com/qpdf/qpdf) is one tool that can be useful for exploring a PDF and transforming or extracting information from it. Another is a framework in Ruby called [Origami](https://github.com/mobmewireless/origami-pdf).

When exploring PDF content for hidden data, some of the hiding places to check include:
* non-visible layers
* Adobe's metadata format "XMP"
* the "incremental generation" feature of PDF wherein a previous version is retained but not visible to the user
* white text on a white background
* text behind images
* an image behind an overlapping image
* non-displayed comments

There are also several Python packages for working with the PDF file format, like [PeepDF](https://github.com/jesparza/peepdf), that enable you to write your own parsing scripts.

### Video and Audio file analysis
* Audacity and viewing waveforms in the spectogram view
* Refer back to steg section of [Image file format analysis](### Image file format analysis)
* DTMF tone decoding http://dialabc.com/sound/detect/index.html
* Morse code

### Office file analysis
* oletools
