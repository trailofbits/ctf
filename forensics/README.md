# Forensics

<script async class="speakerdeck-embed" data-id="424ae340bbfc013182c64af3413e5309" data-ratio="1.33333333333333" src="//speakerdeck.com/assets/embed.js"></script>

In a CTF context, "Forensics" challenges can include file format analysis, steganography, memory dump analysis, or network packet capture analysis. Any challenge to examine and process a hidden piece of information out of static data files (as opposed to executable programs or remote servers) could be considered a Forensics challenge (unless it involves cryptography, in which case it probably belongs in the Crypto category). 

Forensics is a broad CTF category that does not map well to any particular job role in the security industry, although some challenges model the kinds of tasks seen in Incident Response (IR). Even in IR work, computer forensics is usually the domain of law enforcement seeking evidentiary data and attribution, rather than the commercial incident responder who may just be interested in expelling an attacker and/or restoring system integrity.

Unlike most CTF forensics challenges, a real-world computer forensics task would hardly ever involve unraveling a scheme of cleverly encoded bytes, hidden data, mastroshka-like files-within-files, or other such brain-teaser puzzles. One would typically not bust a criminal case by carefully reassembling a corrupted PNG file, revealing a photo of a QR code that decodes to a password for a zip archive containing an NES rom that when played will output the confession. Rather, real-world forensics typically requires that a practitioner find indirect evidence of maliciousness: either the traces of an attacker on a system, or the traces of "insider threat" behavior. Real-world computer forensics is largely about knowing where to find incriminating clues in logs, in memory, in filesystems/registries, and associated file and filesystem metadata. Also, network (packet capture) forensics is more about metadata analysis than content analysis, as most network sessions are TLS-encrypted between endpoints now.

This disconnect between the somewhat artificial puzzle-game CTF "Forensics" and the way that forensics is actually done in the field might be why this category does not receive as much attention as the vulnerability-exploitation style challenges. It may also lack the "black hat attacker" appeal that draws many players to participate in CTFs. Regardless, many players enjoy the variety and novelty in CTF forensics challenges. It can also be a more beginner friendly category, in which the playing field is evened out by the fact that there are no $5,000 professional tools like IDA Pro Ultimate Edition with Hex-Rays Decompiler that would give a huge advantage to some players but not others, as is the case with executable analysis challenges.

## Requisite Skills

For solving forensics CTF challenges, the three most useful abilities are probably:
* Knowing a scripting language (e.g., Python)
* Knowing how to manipulate binary data (byte-level manipulations) in that language
* Recognizing formats, protocols, structures, and encodings

The first and second you can learn and practice outside of a CTF, but the third may only come from experience. Hopefully with this document, you can at least get a good headstart.

And of course, like most CTF play, the ideal environment is a Linux system with – occasionally – Windows in a VM. MacOS is not a bad environment to substitute for Linux, if you can accept that some open-source tools may not install or compile correctly.

## Manipulating Binary Data in Python

Assuming you have already picked up some Python programming, you still may not know how to effectively work with binary data. Low-level languages like C might be more naturally suited for this task, but Python's many useful packages from the open-source community outweigh its learning curve for working with binary data.

Here are some examples of working with binary data in Python.

Writing or reading a file in binary mode:
```python
f = open('Reverseit', "rb")
s = f.read()
f.close()
f = open('ItsReversed', "wb")
f.write(s[::-1])
f.close()
```

The [bytearray](https://web.archive.org/web/20190507205506/http://infohost.nmt.edu:80/tcc/help/pubs/python/web/bytearray-type.html) type is a mutable sequence of bytes, and is available in both Python 2 and 3:
```python
>>> s = bytearray(b"Hello World")
>>> for c in s: print(c)
...
72
101
108
108
111
32
87
111
114
108
100
>>> 
```

You can also define a bytearray from hexidecimal representation Unicode strings:
```python
example2 = bytearray.fromhex(u'00 ff')
>>> example2
bytearray(b'\x00\xff')
>>> example2[1]
255
```

The bytearray type has most of the same convenient methods as a Python `str` or `list`: split(), insert(), reverse(), extend(), pop(), remove(), etc.

Reading a file into a bytearray for processing:
```python
data = bytearray(open('challenge.png', 'rb').read())
```

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

[TrID](https://mark0.net/soft-trid-e.html) is a more sophisticated version of `file`. Although it's closed-source, it's free and works across platforms. It also uses an identification heuristic, but with certainty percentages. Its advantage is its larger set of known filetypes that include a lot of proprietary and obscure formats seen in the real world.

### File carving

Files-within-files is a common trope in forensics CTF challenges, and also in embedded systems' firmware where primitive or flat filesystems are common. The term for identifying a file embedded in another file and extracting it is "file carving." One of the best tools for this task is the firmware analysis tool [binwalk](https://github.com/ReFirmLabs/binwalk). 

[scalpel](https://github.com/sleuthkit/scalpel), now a part of SleuthKit (discussed further under [Filesystems](###Filesystems)) is another tool for file-carving, formerly known as Foremost.

To manually extract a sub-section of a file (from a known offset to a known offset), you can use the `dd` command. Many hex-editors also offer the ability to copy bytes and paste them as a new file, so you don't need to study the offsets.

Example of file-carving with `dd` from a file-offset of 1335205 for a length of 40668937 bytes:
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

[Other uses of the hexdump command.](https://www.commandlinefu.com/commands/using/hexdump)

### Binary-as-text encodings

Binary is 1's and 0's, but often is transmitted as text. It would be wasteful to transmit actual sequences of 101010101, so the data is first encoded using one of a variety of methods. This is what is referred to as [binary-to-text encoding]( https://en.wikipedia.org/wiki/Binary-to-text_encoding), a popular trope in CTF challenges. When doing a `strings` analysis of a file as discussed above, you may uncover this binary data encoded as text strings.

We mentioned that to excel at forensics CTF challenges, it is important to be able to recognize encodings. Some can be identifed at a glance, such as [Base64](https://en.wikipedia.org/wiki/Base64) encoded content, identifiable by its alphanumeric charset and its "=" padding suffix (when present). There are many [Base64 encoder/decoders](https://www.base64decode.org) online, or you can use the `base64` command:

```
$ echo aGVsbG8gd29ybGQh | base64 -D
hello world!
```

ASCII-encoded hexadecimal is also identifiable by its charset (0-9, A-F). ASCII characters themselves occupy a certain range of bytes (0x00 through 0x7f, see `man ascii`), so if you are examining a file and find a string like `68 65 6c 6c 6f 20 77 6f 72 6c 64 21`, it's important to notice the preponderance of 0x60's here: this is ASCII. Technically, it's text ("hello world!") encoded as ASCII (binary) encoded as hexadecimal (text again). Confused yet? 😉

There are [several sites](https://gchq.github.io/CyberChef/) that provide online encoder-decoders for a variety of encodings. For a more local converter, try the `xxd` command.

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

When analyzing file formats, a file-format-aware (a.k.a. templated) hex-editor like [010 Editor](https://www.sweetscape.com/010editor/) is invaluable. An open-source alternative has emerged called [Kaitai](https://kaitai.io). Additionally, a lesser-known feature of [the Wireshark network protocol analyzer](https://wiki.wireshark.org/FrontPage) is [its ability to analyze certain media file formats like GIF, JPG, and PNG](https://wiki.wireshark.org/MediaTypesFamily). All of these tools, however, are made to analyze non-corrupted and well-formatted files. Many CTF challenges task you with reconstructing a file based on missing or zeroed-out format fields, etc.

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

Another note about zip cracking is that if you have an unencrypted/uncompressed copy of any one of the files that is compressed in the encrypted zip, you can perform a "plaintext attack" and crack the zip, as [detailed here](https://www.hackthis.co.uk/articles/known-plaintext-attack-cracking-zip-files), and explained in [this paper](https://web.archive.org/web/20171028074139/https://www.cs.auckland.ac.nz/~mike/zipattacks.pdf). The newer scheme for password-protecting zip files (with AES-256, rather than "ZipCrypto") does not have this weakness.

### Image file format analysis

CTFs are supposed to be fun, and image files are good for containing hacker memes, so of course image files often appear in CTF challenges. Image file formats are complex and can be abused in many ways that make for interesting analysis puzzles involving metadata fields, lossy and lossless compression, checksums, steganography, or visual data encoding schemes.

The easy initial analysis step is to check an image file's metadata fields with [exiftool](https://exiftool.org/). If an image file has been abused for a CTF, its EXIF might identify the original image dimensions, camera type, embedded thumbnail image, comments and copyright strings, GPS location coordinates, etc. There might be a gold mine of metadata, or there might be almost nothing. It's worth a look.

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

PNG files, in particular, are popular in CTF challenges, probably for their lossless compression suitable for hiding non-visual data in the image. PNG files can be dissected in Wireshark. To verify correcteness or attempt to repair corrupted PNGs you can use [pngcheck](http://libpng.org/pub/png/apps/pngcheck.html). If you need to dig into PNG a little deeper, the [pngtools](https://www.madebymikal.com/category/pngtools/) package might be useful.

[Steganography](https://en.wikipedia.org/wiki/Steganography), the practice of concealing some amount of secret data within an unrelated data as its vessel (a.k.a. the "cover text"), is extraordinarily rare in the real world (made effectively obsolete by strong cryptography), but is another popular trope in CTF forensics challenges. Steganography could be implemented using any kind of data as the "cover text," but media file formats are ideal because they tolerate a certain amount of unnoticeable data loss (the same characteristic that makes lossy compression schemes possible). The difficulty with steganography is that extracting the hidden message requires not only a detection that steganography has been used, but also the exact [steganographic tool](https://en.wikipedia.org/wiki/Steganography_tools#Tools_comparison) used to embed it. Given a challenge file, if we suspect steganography, we must do at least a little guessing to check if it's present. [Stegsolve (JAR download link)](http://www.caesum.com/handbook/Stegsolve.jar) is often used to apply various steganography techniques to image files in an attempt to detect and extract hidden data. You may also try [zsteg](https://github.com/zed-0xff/zsteg).

[Gimp](https://www.gimp.org) provides the ability to alter various aspects of the visual data of an image file. CTF challenge authors have historically used altered Hue/Saturation/Luminance values or color channels to hide a secret message. Gimp is also good for confirming whether something really *is* an image file: for instance, when you believe you have recovered image data from a display buffer in a memory dump or elsewhere, but you lack the image file header that specifies pixel format, image height and width and so on. Open your mystery data as "raw image data" in Gimp and experiment with different settings.

The [ImageMagick toolset](https://www.imagemagick.org/script/index.php) can be incorporated into scripts and enable you to quickly `identify`, resize, crop, modify, `convert`, and otherwise manipulate image files.  It can also find the visual and data difference between two seemingly identical images with its `compare` tool.

If you are writing a custom image file format parser, import the [Python Image Library (PIL) aka Pillow](https://pillow.readthedocs.io/en/stable/). It enables you to extract frames from animated GIFs or even individual pixels from a JPG – it has native support for most major image file formats.

If working with QR codes (2D barcodes), also check out the qrtools module for Python. You can decode an image of a QR code with less than 5 lines of Python. Of course, if you just need to decode one QR code, any smartphone will do.

### Filesystems analysis

Occasionally, a CTF forensics challenge consists of a full disk image, and the player needs to have a strategy for finding a needle (the flag) in this haystack of data. Triage, in computer forensics, refers to the ability to quickly narrow down what to look at. Without a strategy, the only option is looking at everything, which is time-prohibitive (not to mention exhausting).

Example of mounting a CD-ROM filesystem image:
```
mkdir /mnt/challenge
mount -t iso9660 challengefile /mnt/challenge
```

Once you have mounted the filesystem, the `tree` command is not bad for a quick look at the directory structure to see if anything sticks out to you for further analysis.

You may not be looking for a file in the visible filesystem at all, but rather a hidden volume, unallocated space (disk space that is not a part of any partition), a deleted file, or a non-file filesystem structure like an [NTFS "alternate data stream"](https://www.nirsoft.net/utils/alternate_data_streams.html). For EXT3 and EXT4 filesystems, you can attempt to find deleted files with [extundelete](https://extundelete.sourceforge.net/). For everything else, there's [TestDisk](https://www.cgsecurity.org/wiki/TestDisk): recover missing partition tables, fix corrupted ones, undelete files on FAT or NTFS, etc.

[The Sleuth Kit](https://www.sleuthkit.org/sleuthkit/) and its accompanying web-based user interface, "Autopsy," is a powerful open-source toolkit for filesystem analysis. It's a bit geared toward law-enforcement tasks, but can be helpful for tasks like searching for a keyword across the entire disk image, or looking at the unallocated space.

Embedded device filesystems are a unique category of their own. Made for fixed-function low-resource environments, they can be compressed, single-file, or read-only. [Squashfs](https://en.wikipedia.org/wiki/SquashFS) is one popular implementation of an embedded device filesystem. For images of embedded devices, you're better off analyzing them with [firmware-mod-kit](https://code.google.com/archive/p/firmware-mod-kit/) or [binwalk](https://github.com/devttys0/binwalk).

### Packet Capture (PCAP) file analysis

Network traffic is stored and captured in a PCAP file (Packet capture), with a program like `tcpdump` or [Wireshark](https://www.wireshark.org) (both based on `libpcap`). A popular CTF challenge is to provide a PCAP file representing some network traffic and challenge the player to recover/reconstitute a transferred file or transmitted secret. Complicating matters, the packets of interest are usually in an ocean of unrelated traffic, so analysis triage and filtering the data is also a job for the player.

For initial analysis, take a high-level view of the packets with Wireshark's statistics or conversations view, or its [capinfos](https://www.wireshark.org/docs/man-pages/capinfos.html) command. Wireshark, and its command-line version `tshark`, both support the concept of using "filters," which, if you master the syntax, can quickly reduce the scope of your analysis. There is also an online service called [DynamiteLab (formerly PacketTotal)](https://lab.dynamite.ai/) where you can submit PCAP files up to 50MB, and graphically display some timelines of connections, and SSL metadata on the secure connections. Plus it will highlight file transfers and show you any "suspicious" activity. If you already know what you're searching for, you can do `grep`-style searching through packets using [`ngrep`](https://en.wikipedia.org/wiki/Ngrep).

Just as "file carving" refers to the identification and extraction of files embedded within files, "packet carving" is a term sometimes used to describe the extraction of files from a packet capture. There are expensive commercial tools for recovering files from captured packets, but one open-source alternative is the [Xplico framework](https://www.xplico.org/). Wireshark also has an "Export Objects" feature to extract data from the capture (e.g., File -> Export Objects -> HTTP -> Save all). Beyond that, you can try tcpxtract, [Network Miner](https://www.netresec.com/?page=NetworkMiner), [Foremost](https://foremost.sourceforge.net), or [Snort](https://www.snort.org).

If you want to write your own scripts to process PCAP files directly, the [dpkt Python package for pcap manipulation](https://dpkt.readthedocs.io/en/latest/) is recommended. You could also interface Wireshark from your Python using [Wirepy](https://wirepy.readthedocs.io).

If trying to repair a damaged PCAP file, there is an [online service for repairing PCAP files](https://f00l.de/hacking/pcapfix.php) called PCAPfix.

A note about PCAP vs PCAPNG: there are two versions of the PCAP file format; PCAPNG is newer and not supported by all tools. You may need to convert a file from PCAPNG to PCAP using Wireshark or another compatible tool, in order to work with it in some other tools.

### Memory dump analysis

For years, computer forensics was synonymous with filesystem forensics, but as attackers became more sophisticated, they started to avoid the disk. Also, a snapshot of memory often contains context and clues that are impossible to find on disk because they only exist at runtime (operational configurations, remote-exploit shellcode, passwords and encryption keys, etc). So memory snapshot / memory dump forensics has become a popular practice in incident response. In a CTF, you might find a challenge that provides a memory dump image, and tasks you with locating and extracting a secret or a file from within it.

The premiere open-source framework for memory dump analysis is [Volatility](https://www.volatilityfoundation.org). Volatility is a Python script for parsing memory dumps that were gathered with an external tool (or a VMware memory image gathered by pausing the VM). So, given the memory dump file and the relevant "profile" (the OS from which the dump was gathered), Volatility can start identifying the structures in the data: running processes, passwords, etc. It is also extensible using plugins for extracting various types of artifact.

[Ethscan](https://jamaaldev.blogspot.com/2013/07/ethscan-volatility-memory-forensics.html?m=1) is made to find data in a memory dump that looks like network packets, and then extract it into a pcap file for viewing in Wireshark. There are plugins for extracting [SQL databases](https://blog.superponible.com/2014/08/31/volatility-plugin-sqlite-helper/), [Chrome history](https://blog.superponible.com/2014/08/31/volatility-plugin-chrome-history/), [Firefox history](https://blog.superponible.com/2014/08/31/volatility-plugin-firefox-history/) and much more.

### PDF file analysis

PDF is an extremely complicated document file format, with enough tricks and hiding places [to write about for years](https://www.sultanik.com/pocorgtfo/). This also makes it popular for CTF forensics challenges. The NSA wrote a guide to these hiding places in 2008 titled "Hidden Data and Metadata in Adobe PDF Files: Publication Risks and Countermeasures." It's no longer available at its original URL, but you can [find a copy here](https://web.archive.org/web/20220901044601/http://www.itsecure.hu/library/file/Biztons%C3%A1gi%20%C3%BAtmutat%C3%B3k/Alkalmaz%C3%A1sok/Hidden%20Data%20and%20Metadata%20in%20Adobe%20PDF%20Files.pdf). Ange Albertini also keeps a wiki on GitHub of [PDF file format tricks](https://github.com/corkami/docs/blob/master/PDF/PDF.md).

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

Like image file formats, audio and video file trickery is a common theme in CTF forensics challenges not because hacking or data hiding ever happens this way in the real world, but just because audio and video is fun. As with image file formats, stegonagraphy might be used to embed a secret message in the content data, and again you should know to check the file metadata areas for clues. Your first step should be to take a look with the [mediainfo](https://mediaarea.net/en/MediaInfo) tool (or `exiftool`) and identify the content type and look at its metadata.

[Audacity](https://www.audacityteam.org/) is the premiere open-source audio file and waveform-viewing tool, and CTF challenge authors love to encode text into audio waveforms, which you can see using the spectogram view (although a specialized tool called [Sonic Visualiser](https://www.sonicvisualiser.org) is better for this task in particular). Audacity can also enable you to slow down, reverse, and do other manipulations that might reveal a hidden message if you suspect there is one (if you can hear garbled audio, interference, or static). [Sox](https://sourceforge.net/projects/sox/) is another useful command-line tool for converting and manipulating audio files.

It's also common to check least-significant-bits (LSB) for a secret message. Most audio and video media formats use discrete (fixed-size) "chunks" so that they can be streamed; the LSBs of those chunks are a common place to smuggle some data without visibly affecting the file.

Other times, a message might be encoded into the audio as [DTMF tones](http://dialabc.com/sound/detect/index.html) or morse code. For these, try working with [multimon-ng](https://www.kali.org/tools/multimon-ng/) to decode them.

Video file formats are really container formats, that contain separate streams of both audio and video that are multiplexed together for playback. For analyzing and manipulating video file formats, [ffmpeg](https://ffmpeg.org) is recommended. `ffmpeg -i` gives initial analysis of the file content. It can also de-multiplex or playback the content streams. The power of ffmpeg is exposed to Python using [ffmpy](https://ffmpy.readthedocs.io/latest/examples.html).

### Office file analysis

Microsoft has created dozens of office document file formats, many of which are popular for the distribution of phishing attacks and malware because of their ability to include macros (VBA scripts). Microsoft Office document forensic analysis is not too different from PDF document forensics, and just as relevant to real-world incident response.

Broadly speaking, there are two generations of Office file format: the OLE formats (file extensions like RTF, DOC, XLS, PPT), and the "Office Open XML" formats (file extensions that include DOCX, XLSX, PPTX). Both formats are structured, compound file binary formats that enable Linked or Embedded content (Objects). OOXML files are actually zip file containers (see the section above on archive files), meaning that one of the easiest ways to check for hidden data is to simply `unzip` the document:

```
$ unzip example.docx 
Archive:  example.docx
  inflating: [Content_Types].xml     
  inflating: _rels/.rels             
  inflating: word/_rels/document.xml.rels  
  inflating: word/document.xml       
  inflating: word/theme/theme1.xml   
 extracting: docProps/thumbnail.jpeg  
  inflating: word/comments.xml       
  inflating: word/settings.xml       
  inflating: word/fontTable.xml      
  inflating: word/styles.xml         
  inflating: word/stylesWithEffects.xml  
  inflating: docProps/app.xml        
  inflating: docProps/core.xml       
  inflating: word/webSettings.xml    
  inflating: word/numbering.xml
$ tree
.
├── [Content_Types].xml
├── _rels
├── docProps
│   ├── app.xml
│   ├── core.xml
│   └── thumbnail.jpeg
└── word
    ├── _rels
    │   └── document.xml.rels
    ├── comments.xml
    ├── document.xml
    ├── fontTable.xml
    ├── numbering.xml
    ├── settings.xml
    ├── styles.xml
    ├── stylesWithEffects.xml
    ├── theme
    │   └── theme1.xml
    └── webSettings.xml
```

As you can see, some of the structure is created by the file and folder hierarchy. The rest is specified inside the XML files. [*New Steganographic Techniques for the OOXML File Format*, 2011](https://www.researchgate.net/publication/221548814_New_Steganographic_Techniques_for_the_OOXML_File_Format) details some ideas for data hiding techniques, but CTF challenge authors will always be coming up with new ones.

Once again, a Python toolset exists for the examination and analysis of OLE and OOXML documents: [oletools](https://www.decalage.info/python/oletools). For OOXML documents in particular, [OfficeDissector](https://www.officedissector.com) is a very powerful analysis framework (and Python library). The latter includes a [quick guide to its usage](https://github.com/grierforensics/officedissector/blob/master/doc/html/_sources/txt/ANALYZING_OOXML.txt).

Sometimes the challenge is not to find hidden static data, but to analyze a VBA macro to determine its behavior. This is a more realistic scenario, and one that analysts in the field perform every day. The aforementioned dissector tools can indicate whether a macro is present, and probably extract it for you. A typical VBA macro in an Office document, on Windows, will download a PowerShell script to %TEMP% and attempt to execute it, in which case you now have a PowerShell script analysis task too. But malicious VBA macros are rarely complicated, since VBA is [typically just used as a jumping-off platform to bootstrap code execution](https://www.lastline.com/labsblog/party-like-its-1999-comeback-of-vba-malware-downloaders-part-3/). In the case where you do need to understand a complicated VBA macro, or if the macro is obfuscated and has an unpacker routine, you don't need to own a license to Microsoft Office to debug this. You can use [Libre Office](https://libreoffice.org/): [its interface](https://www.debugpoint.com/debugging-libreoffice-macro-basic-using-breakpoint-and-watch/) will be familiar to anyone who has debugged a program; you can set breakpoints and create watch variables and capture values after they have been unpacked but before whatever payload behavior has executed. You can even start a macro of a specific document from a command line:

```
$ soffice path/to/test.docx macro://./standard.module1.mymacro
```
