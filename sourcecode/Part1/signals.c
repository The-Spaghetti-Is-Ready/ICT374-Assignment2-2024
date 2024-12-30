#include "include/signals.h"

void ClaimChildren() {
    int claim = 1;
    int status;
    pid_t pid;

    while(claim) {
        pid = waitpid(-1, &status, WNOHANG);

        if (pid <= 0) {
            claim = 0;
        }
    }
}

void HandleSigs(int sig_num) {
    if(sig_num == SIGCHLD) { //currently only adding specific behaviour for SIGCHLD
        ClaimChildren();
    }
}

int SetupSigs() {
    struct sigaction act;
    sigset_t sig_set;

    act.sa_flags = 0;
    act.sa_handler = HandleSigs;

    if(sigaction(SIGCHLD, &act, NULL) != 0) {
        perror("sigaction");
        exit(1);
    }

    sigemptyset(&sig_set);
    sigaddset(&sig_set, SIGINT); //ignoring interrupt
    sigaddset(&sig_set, SIGQUIT); //ignoring quit
    sigaddset(&sig_set, SIGTSTP); //ignoring stop

    if(sigprocmask(SIG_SETMASK, &sig_set, NULL) < 0) {
        perror("Error with SEG_SETMASK");
        return -1;
    }

    return 0;
}


// Blocks or unblocks a signal.
int SigBlock(int how, int sig_num) {
    sigset_t sig_set;
    sigemptyset(&sig_set);
    sigaddset(&sig_set, sig_num);

    if(sigprocmask(how, &sig_set, NULL) < 0) {
        perror("Error trying to (un)block signal");
        return -1;
    }else {
        return 0;
    }
}
