#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int str_cmp(char *p1, char *p2){
  for(; *p1 == *p2; p1++, p2++){
    if(*p1 == '\0')return 0;
  }
  return *p1 - *p2;
}

void add_string(char* a, char* b){
  while(*a != '\0') ++a;
  while((*a = *b) != '\0'){
    ++a;++b;
  }
}  

int main(int argc, char *argv[]){
  int source_file, output_file, char_num;
  char buf[256] = "\0";
  struct stat st;

  if(argc < 3){
    printf(2,"mv error : not enough argument\n");
    exit();
  }
  
  if((source_file = open(argv[1], O_RDONLY)) < 0){
    printf(2,"mv error : can't open %s\n",argv[1]);
    exit();
  }
  
  if((output_file = open(argv[2], O_RDONLY)) < 0){
    // fileがない場合
    if((output_file = open(argv[2],O_WRONLY | O_CREATE)) < 0){
      printf(2, "mv error : can't make %s\n",argv[2]);
      exit();
    }

  //ファイルかディレクトリが見つかった
  }else if(fstat(output_file, &st) < 0){
    printf(2, "mv error : can't stat\n",argv[2]);
    exit();
  }else{
    switch(st.type){
      case T_FILE:
        break;
      case T_DIR:
        add_string(buf, argv[2]);
        if(buf[strlen(buf) - 1] != '/') add_string(buf, "/");
        add_string(buf, argv[1]);
        if((output_file = open(buf, O_WRONLY|O_CREATE)) < 0){
          printf(2, "mv error : can't make %s\n",buf);
          exit();
        }
    }
  }

  while((char_num = read(source_file, buf, sizeof(buf)))){
    write(output_file, buf, char_num);
  }

  if(str_cmp(argv[1],argv[2]))
    unlink(argv[1]);
  exit();
}
