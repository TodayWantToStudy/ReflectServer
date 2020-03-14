#include"unp.h"

Sigfunc*
signal (int signo, Sigfunc* func){
	struct sigaction act, oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo == SIGALRM){
	}
	else{
		act.sa_flags |= SA_RESTART;
	}
	if(sigaction(signo, &act, &oact) < 0)
		return (SIG_ERR);
	return oact.sa_handler;
}

void Signal(int signo, Sigfunc* func){
	Sigfunc* rtn = signal(signo, func);
	if(rtn == SIG_ERR)
		err_sys("Signal error");
}
