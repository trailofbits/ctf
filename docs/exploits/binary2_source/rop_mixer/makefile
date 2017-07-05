

all: rop_mixer rop_page compile

rop_page: rop.s
	nasm -f elf rop.s -o rop.o

rop_mixer: rop_mixer.c
	gcc  rop_mixer.c -o rop_mixer.o -c -std=gnu99 -m32  -masm=intel  

compile: rop.o rop_mixer.o
	gcc -m32 rop.o rop_mixer.o -o rop_mixer -std=gnu99

clean:
	rm -f rop.o rop_mixer.o rop_mixer