#include "types.h"
#include "stat.h"
#include "user.h"

int g_opt[256];

void
more(int fd, int num)
{
  int n, count = 0; 
  char buf[1];
  char key_input[256];
  while((n = read(fd, buf, sizeof(buf))) > 0){
    if(count >= num){
      count = 0;
      while(key_input[0] != ' '){
        gets(key_input, sizeof(key_input));
        wait();
      }
      memset(key_input, 0, sizeof(key_input));
    }
    if(buf[0] == '\n'){
      ++count;
    }
    write(1, buf, n);
  }
  if(n < 0){
    printf(1, "more: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i, opt_pos;
  int update_num = 30;

  if(argc <= 1){
    more(0, 0);
    exit();
  }

  if(argc >= 2){
    for(i = 1, opt_pos = 0; i < argc ; i++){
      if(argv[i][0] == '-'){
        switch (argv[i][1]){
          case 'n':
            g_opt[opt_pos++] = i;
            g_opt[opt_pos++] = i + 1;
            update_num = atoi(argv[i + 1]);
            if(update_num == 0){
              printf(1, "not adequate use %s\n",argv[i]);
              exit();
            }
            //printf(1,"-n found at %d %d num = %d\n",i,opt[opt_pos -1], update_num);
            break;
          default:
            printf(1, "undefined option %s\n",argv[i]);
            exit();
        }
      }
    }
  }

  for(i = 1, opt_pos = 0; i < argc; i++){
    
    if(i == g_opt[opt_pos]){
      opt_pos++;
      continue;
    }

    if((fd = open(argv[i], 0)) < 0){
      printf(1, "more: cannot open %s\n", argv[i]);
      exit();
    }
    more(fd, update_num);
    close(fd);
  }
  exit();
}
