BITS 32

global GADGET_SIZE

%define GADGET_SIZE 16

section .ROP_MIX exec write

	
%define NOPS(SYM) times (GADGET_SIZE-($-(SYM))) nop

global NUM_GADGETS
%define NUM_GADGETS (NASM_END-NASM_BEGIN)/GADGET_SIZE


	;; ---------------------------------
	;; ---------------------------------
	;; ---------------------------------
global NASM_END
global NASM_BEGIN
NASM_BEGIN:	

peax:
	pop eax
	ret
	NOPS(peax)
pebx:
	pop ebx
	ret
	NOPS(pebx)
	
pecx:	pop ecx
	ret
	NOPS(pecx)

pedx:	pop edx
	ret
	NOPS(pedx)

pedi:	pop edi
	ret
	NOPS(pedi)

pesi:	pop esi
	ret
	NOPS(pesi)

pebp:	pop ebp
	ret
	NOPS(pebp)

pesp:	pop esp
	ret
	NOPS(pesp)

	;; ---------------------------------
	;; ---------------------------------
	;; ---------------------------------

pueax:
	push eax
	ret
	NOPS(pueax)
puebx:
	push ebx
	ret
	NOPS(puebx)
	
puecx:	push ecx
	ret
	NOPS(puecx)

puedx:	push edx
	ret
	NOPS(puedx)

puedi:	push edi
	ret
	NOPS(puedi)

puesi:	push esi
	ret
	NOPS(puesi)

puebp:	push ebp
	ret
	NOPS(puebp)

puesp:	push esp
	ret
	NOPS(puesp)


	;; ---------------------------------
	;; ---------------------------------
	;; ---------------------------------

ldeax:
	mov eax, [eax]
	ret
	NOPS(ldeax)
ldebx:
	mov ebx, [eax]
	ret
	NOPS(ldebx)
	
ldecx:	mov ecx, [eax]
	ret
	NOPS(ldecx)

ldedx:	mov edx, [eax]
	ret
	NOPS(ldedx)

ldedi:	mov edi, [eax]
	ret
	NOPS(ldedi)

ldesi:	mov esi, [eax]
	ret
	NOPS(ldesi)

ldebp:	mov ebp, [eax]
	ret
	NOPS(ldebp)

ldesp:	mov esp, [eax]
	ret
	NOPS(ldesp)

	;; ---------------------------------
	;; ---------------------------------
	;; ---------------------------------
	

seax:
	mov [eax], eax
	ret
	NOPS(seax)

sebx:
	mov [ebx], eax
	ret
	NOPS(sebx)

secx:
	mov [ecx], eax
	ret
	NOPS(secx)

sedx:
	mov [edx], eax
	ret
	NOPS(sedx)

sesi:
	mov [esi], eax
	ret
	NOPS(sesi)

sedi:
	mov [edi], eax
	ret
	NOPS(sedi)

sebp:
	mov [ebp], eax
	ret
	NOPS(sebp)

sesp:
	mov [esp], eax
	ret
	NOPS(sesp)

int80:
	int 0x80
	ret
	NOPS(int80)



	;; ---------------------------------
	;; ---------------------------------
	;; ---------------------------------
m1eax:
	mov eax, eax
	ret
	NOPS(m1eax)
m1ebx:
	mov ebx, eax
	ret
	NOPS(m1ebx)
	
m1ecx:	mov ecx, eax
	ret
	NOPS(m1ecx)

m1edx:	mov edx, eax
	ret
	NOPS(m1edx)

m1edi:	mov edi, eax
	ret
	NOPS(m1edi)

m1esi:	mov esi, eax
	ret
	NOPS(m1esi)

m1ebp:	mov ebp, eax
	ret
	NOPS(m1ebp)

m1esp:	mov esp, eax
	ret
	NOPS(m1esp)

	;; ---------------------------------
	;; ---------------------------------
	;; ---------------------------------




m2eax:
	mov eax, eax
	ret
	NOPS(m2eax)
m2ebx:
	mov eax, ebx
	ret
	NOPS(m2ebx)
	
m2ecx:	mov eax, ecx
	ret
	NOPS(m2ecx)

m2edx:	mov eax, edx
	ret
	NOPS(m2edx)

m2edi:	mov eax, edi
	ret
	NOPS(m2edi)

m2esi:	mov eax, esi
	ret
	NOPS(m2esi)

m2ebp:	mov eax, ebp
	ret
	NOPS(m2ebp)

m2esp:	mov eax, esp
	ret
	NOPS(m2esp)


NASM_END:	