#include "types.h"
#include "stat.h"
#include "user.h"


int main(int argc, char *argv[]){
  int source_file, output_file;
  char buf[256];

  if(argc < 3){
    printf(2,"cp error : not enough argument\n");
    exit();
  }
  
  if((source_file = open(argv[1], 0) == -1) || (output_file = open(argv[2], 1) == -1)){
    printf(2,"cp error : can't open file\n");
    exit();
  }

  while(read(source_file, buf, sizeof(buf))){
    write(output_file, buf, sizeof(buf));
  }
  exit();
}
