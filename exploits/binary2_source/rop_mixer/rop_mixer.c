#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

extern size_t NUM_GADGETS;
extern char NASM_BEGIN;
extern char NASM_END;
//extern size_t GADGET_SIZE;

#define GADGET_SIZE 16 //kludgy already defined in .s
#define NUM_GADGETS (&NASM_END-&NASM_BEGIN)/ GADGET_SIZE


#define MIXING_ROUNDS 20


typedef struct{
  char code_chunk[GADGET_SIZE];
}ROP_GADGET;



void mix_gadgets(void){
  ROP_GADGET* gadgets=(void*)&NASM_BEGIN;
  srand(time(NULL));
  
  for(int i=0; i < NUM_GADGETS*MIXING_ROUNDS; i++){
    int index = (int)random() % NUM_GADGETS;
    ROP_GADGET temp_gadget = gadgets[i % NUM_GADGETS];
    gadgets[i % NUM_GADGETS] = gadgets[index];
    gadgets[index] = temp_gadget;
  }
  
  //the gadgets are good and mixed up now
  //change the protections so that space is no longer writable
  mprotect( (void*)&NASM_BEGIN, NUM_GADGETS * GADGET_SIZE, PROT_READ | PROT_EXEC);
  return;
}

extern void*  _GLOBAL_OFFSET_TABLE_;
extern void* data_start;

void destroy_GOT(void){
  mprotect(&_GLOBAL_OFFSET_TABLE_,
	   &_GLOBAL_OFFSET_TABLE_-&data_start,
	   PROT_READ|PROT_WRITE); 

  memset(&_GLOBAL_OFFSET_TABLE_, 0, 
	 &_GLOBAL_OFFSET_TABLE_-&data_start);

  return;
}



int main(int argc,char** argv){
  mix_gadgets();
  puts("WELCOME TO THE ROP MIXER");
  printf("There are %d gadgets\n",NUM_GADGETS);
  puts("GOOD LUCK WITH YOUR GADGETS");
  fflush(NULL);
  write(STDOUT_FILENO, &NASM_BEGIN, &NASM_END-&NASM_BEGIN);
  
  char rop_buf[0x1000];
  read(STDIN_FILENO, rop_buf, sizeof(rop_buf));
  __asm__("mov esp,eax"::"r"(rop_buf));
  __asm__("ret");
  
  
}
