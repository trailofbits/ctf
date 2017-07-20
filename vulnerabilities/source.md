# Auditing Source Code
This module is about getting familiar with vulnerabilities that manifest in applications that compile to native code. An accurate and complete understanding of an application written in a compiled language cannot be achieved without learning about how the compiler transforms source to machine code and how processors execute that code. An easy way to gain experience with these transforms is by reverse engineering compiled variants of your own code or of projects with source code available. At the end of this module you will be able to identify vulnerabilities in compiled languages like C and C++.

Vulnerabilities are commonly identified in large software packages due to their use of third-party software libraries. Common examples include libraries like libxml, libpng, libpoppler, and libfreetype that parse complicated file formats and protocols. Each of these libraries have historically been prone to software flaws that make the applications using them vulnerable to attack. It doesn't help that most software packages fail to update these libraries when new versions come out, making it significant easier to find known vulnerabilities to apply in these cases.

## Lecture

* [Source Code Auditing I](http://vimeo.com/30001189)
* [Source Code Auditing II](http://vimeo.com/29702192)

## Workshop

In order to practice your skills, we recommend going through the process of identifying as many vulnerabilities as you can in an intentionally vulnerable toy application and then moving on to a real application and doing the same.

The Newspaper application is a small server written in C that allows authenticated users to read and write articles to a remote file system. Newspaper is written in such a way that it is vulnerable to many different attacks. You should be capable of identifying at least 10 bugs or potential vulnerabilities in this code.

* [Newspaper App](/vulnerabilities/source_workshop/news_server.c)
* [Newspaper App Installer](/vulnerabilities/source_workshop/news_install.sh)

Wireshark, however, is an industry standard network protocol analyzer that has been under continuous development since 1998. Vulnerabilities in this code base are much fewer and far between than in the Newspaper app however many still exist. Take a look at the [wireshark security page](http://wireshark.org/security), find the name of a protocol dissector and see if you can independently discover the vulnerability without looking at the details. Dissectors are located in /epan/dissectors/ folder.

* [Wireshark 1.8.5](http://www.wireshark.org/download/src/all-versions/wireshark-1.8.5.tar.bz2)

## Tools

When auditing, it is helpful to use a tool design to profile and navigate the codebase. Below are two tools, Source Navigator and Understand, designed to help analysts get familiar with code quickly by collecting and displaying information about data relationships, API usage, design patterns and control flow. An example of a useful diffing tool is also listed below. One example of a free, open source code auditing tool is the Clang Static Analyzer, which can help you track down programming errors in common APIs and vulnerable programming patterns.

* [Source Navigator](http://sourcenav.sourceforge.net/)
* [Scitools Understand](http://www.scitools.com/)
* [Meld](http://meldmerge.org/)
* [Clang Static Analyzer](http://clang-analyzer.llvm.org/)

## Resources

Make sure you’re intimately familiar with the internals of the languages you target for analysis.  Vulnerabilities are found by auditors who are more familiar with the language and the codebase than the programmers that originally developed it. In some cases, this level of understanding can be achieved simply by paying attaching to optional compiler warnings or through the use of third-party analysis tools that help track down common programming errors. Computer security is tantamount to computer mastery. Without a rigorous understanding of your targets you can never hope to defeat them.

* [Essential C](/vulnerabilities/references/EssentialC.pdf) - Programming in C primer
* [TAOSSA Chapter 6: C Language Issues](/vulnerabilities/references/Dowd_ch06.pdf) - Strongly recommended reading
* [Integer Overflow](http://en.wikipedia.org/wiki/Integer_overflow)
* [Wireshark Security](https://wireshark.org/security/) - Examples of lots of vulnerablities
* [Gera's Insecure Programming by Example](https://github.com/deadbits/InsecureProgramming) - Examples of small vulnerable C programs
