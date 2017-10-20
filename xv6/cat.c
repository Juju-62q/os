#include "types.h"
#include "stat.h"
#include "user.h"

char buf[1];
char n_flag = 0;
int opt[256];

void
cat(int fd)
{
  int n ,line = 1, flag = 0;
  
  printf(1, "%d ", line++);

  while((n = read(fd, buf, sizeof(buf))) > 0){
    if(flag){
      flag = 0;      
      printf(1, "%d ", line++);
    }
    if(n_flag && buf[0] == '\n'){
      flag = 1;
    }
    write(1, buf, n);
  }
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i, opt_pos;

  if(argc <= 1){
    cat(0);
    exit();
  }

  if(argc >= 2){
    for(i = 1, opt_pos = 0; i < argc ; i++){
      if(argv[i][0] == '-'){
        switch (argv[i][1]){
          case 'n':
            opt[opt_pos++] = i;
            n_flag = 1;
            //printf(1,"-n found at %d %d\n",i,opt[opt_pos -1]);
            break;
          default:
            printf(1, "undefined command %s\n",argv[i]);
            exit();
        }
      }
    }
  }

  for(i = 1, opt_pos = 0; i < argc; i++){
    
    if(i == opt[opt_pos]){
      opt_pos++;
      continue;
    }

    if((fd = open(argv[i], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
    cat(fd);
    close(fd);
  }
  exit();
}
