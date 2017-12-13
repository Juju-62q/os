#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(int argc, char *argv[]){
  int source_file, output_file, char_num;
  char buf[256];

  if(argc < 3){
    printf(2,"cp error : not enough argument\n");
    exit();
  }
  
  if((source_file = open(argv[1], O_RDONLY)) < 0){
    printf(2,"cp error : can't open %s\n",argv[1]);
    exit();
  }
  
  if((output_file = open(argv[2],O_CREATE | O_WRONLY)) < 0){
    printf(2, "cp error : can't make %s\n",argv[2]);
    exit();

    output_file = open(argv[2], O_WRONLY);
  }   

  while((char_num = read(source_file, buf, sizeof(buf)))){
    write(output_file, buf, char_num);
  }

  close(source_file);
  close(output_file);
  exit();
}
