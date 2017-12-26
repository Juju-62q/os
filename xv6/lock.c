#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"

// ロックの状態
#define UNLOCKED   1
#define LOCKED     2

static int locks[NLOCK];
static struct spinlock lock_var;

/*
 *  ロック変数の初期化
 */
void
lock_init(void)
{
	int i;
	initlock(&lock_var, "lock");
	acquire(&lock_var);
	for(i = 0; i < NLOCK; i++){
		locks[i] = UNLOCKED;
	}
	release(&lock_var);	
}

/*
 *  ロック変数の番号の範囲チェック
 */
static int
check_lock(int lkid)
{
	if (lkid < 0 || lkid >= NLOCK) {
		panic("lock: check_lock");
	}
	return 1;
}

/*
 *  指定したロックを確保する
 */
int
lock(int lkid)
{
	if (!check_lock(lkid)) {
		return -1;
	}
	/*
	 *  すでに確保されていたら，ロックが解放されるまで待ってから確保する．
	 */
	acquire(&lock_var);
	if (locks[lkid] == LOCKED) {
		sleep(&locks[lkid], &lock_var);
	}
	locks[lkid] = LOCKED;
	release(&lock_var);
	return 1;
}

/*
 *  指定したロックを解放する（暫定版）
 *
 *  ロックの解放を待つプロセスが複数存在する場合，ここでは１つのプロセ
 *  スだけを起床しなければならないが，食事をする哲学者問題では，ロック
 *  の取得待ちになるプロセス数は最大でも１なので，暫定的にwakeup関数を
 *  用いることとする．
 */
int
unlock(int lkid)
{
	if (!check_lock(lkid)) {
		return -1;
	}
	acquire(&lock_var);
	locks[lkid] = UNLOCKED;
	wakeup(&locks[lkid]); /* このロックの解放を待っていたプロセスを起こす */
	release(&lock_var);
	return 1;
}
