/*
 *  食事をする哲学者の問題
 */
#include "types.h"
#include "stat.h"
#include "user.h"

#define N 3                     /* 哲学者の数 */
#define LEFT  (i)				/* 哲学者 i の左側フォーク番号 */
#define RIGHT ((i+1) % N)		/* 哲学者 i の右側のフォーク番号 */

/* 哲学者の状態 */
#define THINKING 0
#define HUNGRY   1
#define EATING   2

#define EX       9				/* 排他制御用ロック変数の番号 */

static int state[N];			/* 哲学者の状態 */

/*
 *  i 番目のフォークを取る
 */
void take_fork(int i)
{
  printf(1, "philosopher %d is trying to take the fork %d.\n", getpid(), i);
  lock(i);
  printf(1, "philosopher %d takes the fork %d.\n", getpid(), i);
}

/*
 *  i 番目のフォークを置く
 */
void put_fork(int i)
{
  unlock(i);	
  printf(1, "philosopher %d put the fork %d.\n", getpid(), i);
}

/*
 *  考える
 *
 *  考えた後，空腹になる．
 */
void think(i)
{
  sleep(3);			/* 考え中 */

  lock(EX);
  state[i] = HUNGRY;
  printf(1, "philosopher %d becomes hungry.\n", getpid());
  unlock(EX);
}

/*
 *  食事をする
 */
void eat(i)
{
  lock(EX);
  state[i] = EATING;
  printf(1, "philosopher %d is eating.\n", getpid());
  unlock(EX);

  sleep(2);		/* 食事中 */

  lock(EX);  
  state[i] = THINKING;
  printf(1, "philosopher %d finished eating and then starts thinking.\n", getpid());
  unlock(EX);  
}

/*
 *  哲学者の行動（デッドロックが発生する例）
 */
void
philosopher1(int i)
{
  while (1) {
	think(i);
  if (LEFT > RIGHT){
	  take_fork(LEFT);				/* 左側のフォークを取る */
	  take_fork(RIGHT);				/* 右側のフォークを取る */
  }else{
	  take_fork(RIGHT);				/* 左側のフォークを取る */
	  take_fork(LEFT);				/* 右側のフォークを取る */
  }
	eat(i);
	put_fork(LEFT);					/* 左側のフォークを置く */
	put_fork(RIGHT);				/* 右側のフォークを置く */
  }
}

int
main(int argc, char *argv[])
{
  int i, pid[N];

  for (i = 0; i < N; i++) {
	pid[i] = fork();
	if (pid[i] == 0) {
	  printf(1, "philosopher %d is created.\n", getpid());
	  philosopher1(i);
	  exit();
	}
  }
  wait();
  exit();
}
