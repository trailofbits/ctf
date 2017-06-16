# Forensics

<script async class="speakerdeck-embed" data-id="424ae340bbfc013182c64af3413e5309" data-ratio="1.33333333333333" src="//speakerdeck.com/assets/embed.js"></script>

Forensics is a broad CTF category that does not map well to any particular job role in the security industry, although some challenges model the kinds of tasks seen in Incident Response (IR). Even in IR work, computer forensics is usually the domain of law enforcement seeking evidentiary data and attribution, rather than the commercial incident responder who may just be interested in expelling an attacker and/or restoring system integrity. 

In a CTF however, "Forensics" challenges can include file format analysis, steganography, memory dump analysis, or network packet capture analysis. Any challenge to examine and process a hidden piece of information out of static data files (as opposed to executable programs) could be considered a Forensics challenge. Rarely will a CTF challenge look like real-world computer forensics, which would hardly ever involve a scheme of cleverly encoded bytes, hidden data, mastroshka-like files-within-files, or other such brain-teaser puzzles. One would typically not bust a criminal by carefully reassembling a corrupted PNG file, revealing a photo of a QR code that decodes to a password for a zip archive containing the confession. Rather, real-world forensics typically requires that a practictioner find indirect evidence of maliciousness: either the traces of an attacker on a system, or the traces of "insider threat" behavior. Real-world computer forensics is largely about knowing where to find incriminating clues in logs, in memory, in filesystems/registries, and associated file and filesystem metadata. Also, network (packet capture) forensics is more about metadata analysis than content analysis, as most network sessions are TLS-encrypted between endpoints now.

This disconnect between the somewhat artificial puzzle-game CTF "Forensics" and the way that forensics is actually done in the field might be why this category does not receive as much attention as the vulnerability-exploitation style challenges. It may also lack the "blackhat" appeal that draws many to play CTFs. Regardless, many players enjoy the variety and novelty in forensics CTF challenges. It can also be more beginner friendly, and the playing field is evened out by the fact that there are no $5,000 professional tools like IDA Pro with Hex-Rays Decompiler that would give a huge advantage to some players but not others, as is the case with executable analysis challenges.

## Skills

For solving forensics CTF challenges, the three most useful abilities are probably:
* Knowing a scripting language (e.g., Python)
* Knowing how to manipulate binary formats in that language
* ...

And of course, like most CTF play, the ideal environment is a Linux system with – occasionally – Windows in a VM.

TODO: crash-course in manipulating binary data in Python

## Common Challenge Types, Strategies

TODO: list out some recommended tools and strategies for each challenge type

### File format identification (and "magic bytes")
* the file command (libmagic)

### File carving
* binwalk
* zlib streams
* foremost

### Strings analysis
* strings
* grep, bgrep

### Binary-as-text encodings
* https://en.wikipedia.org/wiki/Binary-to-text_encoding
* ASCII-encoded hexadecimal, identifiable by its charset (0-9, A-F)
* Base64, identifiable by its charset and the "=" suffix

### Obfuscated Data
* XOR

### Image formats
* EXIF data
* Compression
* Steganography
* PNG (can be opened in Wireshark)
* Gimp

### Other file formats
* PDF (qpdf package, PeepDF)
* Kaitai.io
* File-format-aware / templated hex-editors
* "zip -FF" command

### Filesystems
* Strategies for finding a needle in a haystack of data

### PCAP analysis
* Wireshark (and tshark) and using filters
* dpkt Python package for pcap manipulation

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
