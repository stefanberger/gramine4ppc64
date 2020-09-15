#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define FR_MAIN 3.141592
#define FR_HANDLER 2.71828

static bool good = false;

static void handler(int signum, siginfo_t* si, void* puc) {
    ucontext_t* uc = puc;
    mcontext_t* mc = &uc->uc_mcontext;

    double *fpnum = (double*)&mc->__fp_regs[9];
    if (*fpnum == FR_MAIN)
        good = true;

    *fpnum = FR_HANDLER;
}

int main() {
    struct sigaction action = {
        .sa_sigaction = handler,
        .sa_flags = SA_SIGINFO,
    };

    int ret = sigaction(SIGINT, &action, NULL);
    if (ret < 0) {
        fprintf(stderr, "sigaction failed\n");
        return 1;
    }

    register double fpnum __asm__("fr9") = FR_MAIN;

    kill(getpid(), SIGINT);

    if (!good) {
        printf("FR5 did not show proper value in signal handler\n");
        ret = 1;
    }

    if (fpnum != FR_HANDLER) {
        printf("Context switch failed: Wrong double value from handler\n");
        ret = 1;
    }

    if (!ret)
        printf("TEST OK\n");

    return ret;
}
