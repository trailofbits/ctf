#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
  char name[0x100];
  char major[0x20];
  char id_number[30];
  char GPA[5];
} Student_Info;

//#define INCOMING_RETURN_ADDR_RTX
Student_Info* get_info(void){
  char verification[4];
  size_t overflow_len=__builtin_frame_address(0)-(void*)verification+sizeof(void*)*2+sizeof(char);
  static Student_Info* student=NULL;
  student=malloc(sizeof(Student_Info));


  
  memset(student,0,sizeof(Student_Info));
  /* char major[20]; */
  /* size_t id_number; */
  /* char name[50]; */
  /* char GPA[4]; */
  
  puts("Welcome to New Student Registration!");
  puts("Please fill out the following information");

  printf("Student name: ");
  //fflush(stdout);
  fgets(student->name, sizeof(student->name), stdin);

  printf("Student ID number: ");
  //fflush(stdout);
  //fscanf(stdin, "%d", &student->id_number);
  fgets(student->id_number, sizeof(student->id_number), stdin);

  printf("Current GPA: ");
  fflush(NULL);
  fgets(student->GPA, sizeof(student->GPA), stdin);

  //printf("%x\n",overflow_len);


  printf("Desired Major: ");
  //fflush(stdout);
  fgets(student->major, sizeof(student->major) , stdin);


  printf("Type YES to verify that all information is correct: ");
  //fflush(stdout);
  fgets(verification,overflow_len,stdin);


  if(strncmp("YES",verification,strlen("YES"))==0){
    return student;
  }
  else{
    puts("Connect again to retry");
    exit(1);
  }
}


int main(){
  char* student_dir="/tmp/";
  Student_Info* student = get_info();
  size_t now=(size_t)time(NULL);
  char full_path[21+strlen(student_dir)];
  memset(full_path,0,sizeof(full_path));
  sprintf(full_path, "%s%d",student_dir, now);
  //puts(full_path);
  FILE* student_record=fopen(full_path,"w");
  if(student_record){
    fwrite(student, sizeof(Student_Info), 1, student_record);
    fclose(student_record);
    puts("Information Recorded");
  }
  else{
    puts("There was a problem saving your record, try again later");
  }
  //fflush(stdout);
  return 0;
}
