# Auditing Binaries
You’ve made it all the way down to the native layer, this is what software is after you pull off all the covers. The flavor of native code we’re going to focus on today is 32-bit Intel x86. Intel processors have been a powerful force in personal computing since the 80’s and currently predominate desktop and server market. Understanding this instruction set will give you some insight into how the programs you use every day operate as well as provide a reference point for when you encounter other instruction sets like ARM, MIPS, PowerPC and SPARC.

This module is about becoming familiar with the native layer and developing strategies for understanding, analyzing and interpreting native code. By the end of this module you should be capable of performing a “reverse compilation” -- going from assembly fragments to statements in higher level languages -- and, in the process, deriving meaning and programmer intent.

## Lecture
Learning x86 can appear daunting at first and requires some dedicated study to master. We recommend reading Chapter 3 of "Computer Systems: A Programmer's Perspective" to learn how C programs get compiled into machine code. Once you you have some basic, working knowledge of this process then keep a handy reference guide around like the x86 Assembly Guide from the University of Virginia. We've found this video series from Quinn Liu to be a quick and painless introduction too.

* CS:APP Chapter 3: [Machine-Level Representation of Programs](http://gec.di.uminho.pt/DISCIP/MaisAC/CS-APP_Bryant/csapp.preview3.pdf)
* [x86 Assembly Guide](http://www.cs.virginia.edu/~evans/cs216/guides/x86.html)
* [Introduction to x86 Assembly](https://www.youtube.com/watch?v=qn1_dRjM6F0&list=PLPXsMt57rLthf58PFYE9gOAsuyvs7T5W9)

## Workshop
The following programs are both “binary bombs.” Reverse engineer the following linux programs to identify the series of inputs that will “defuse” the bomb. Each successive level of the bomb focuses on a different aspect of native code. For example, in the lab from CMU you will encounter different data structures (linked lists, trees) as well as how control flow structures (switches, loops) manifest in native code. While reversing these programs you may find it useful to keep track of your progress by transforming what you see into C or another high level language.

You should aim to solve at least eight stages between the two labs. The CMU bomb lab has a secret phase and the RPI bomb lab has a phase that involves memory corruption, can you find and solve them?

* [CMU Binary Bomb Lab](http://csapp.cs.cmu.edu/2e/bomb32.tar)
* [RPI Binary Bomb Lab](http://www.cs.rpi.edu/academics/courses/spring10/csci4971/rev2/bomb)

## Tools
The two essential tools for working with native code are the debugger and the disassembler. We recommend you become familiar with the industry standard disassembler: IDA Pro. IDA will divide code into discrete chunks corresponding to the functions defined in the program's source code. Each function is then further subdivided into "basic blocks" defined by instructions that alter control flow. This makes it easy to identify loops, conditionals, and other control flow constructs at a glance.

Debuggers allow you to interact with and examine the state of running code by setting breakpoints and examining memory and register contents. You may find this useful as a sanity check if you are not seeing the results you expect your input to produce but be alert, some programs employ anti-debugger techniques and can modify program behavior in the presence of a debugger. The GNU Debugger (gdb) is the standard debugger for most linux systems. gdb can be acquired through the package manager in your chosen linux distribution.

* [IDA Pro Demo](https://www.hex-rays.com/products/ida/support/download_demo.shtml)
* [gdb](http://www.sourceware.org/gdb/)

## Resources
Many good resources exist for learning x86 assembly and the various tricks employed in capture the flag exercises. In addition to the resources above, the x86 Wikibook and the AMD instruction set manual are more complete reference guides you may want to refer to (we find the AMD manual can be less daunting than the corresponding manual from Intel).

* AMD64 Programmer's Manual: [General-Purpose and System Instructions](http://amd-dev.wpengine.netdna-cdn.com/wordpress/media/2008/10/24594_APM_v3.pdf)
* [x86 Assembly Wikibook](https://en.wikibooks.org/wiki/X86_Assembly)
* [Computer Systems: A Programmer's Perspective](http://csapp.cs.cmu.edu/) (CS:APP)

Some of the tools used for reverse engineering can be as complicated as assembly language itself. Cheatsheets that list out common commands and use cases can be helpful.

* [gdb Quick Reference](http://www.refcards.com/docs/peschr/gdb/gdb-refcard-a4.pdf)
* [IDA Quick Reference](https://www.hex-rays.com/products/ida/support/freefiles/IDA_Pro_Shortcuts.pdf)
* [WinDBG x86 Cheat Sheet](./references/X86_Win32_Reverse_Engineering_Cheat_Sheet.pdf)

Finally, many capture the flag challenges will make use of anti-debugging and anti-disassembly techniques to hide or obfuscate the goal. Several of these techniques are employed by the bomb labs but you may want a more complete reference.

* [Linux anti-debugging techniques](http://vxheavens.com/lib/vsc04.html)
* [The "Ultimate" Anti-Debugging Reference](http://pferrie.host22.com/papers/antidebug.pdf)
