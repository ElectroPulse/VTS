#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/wait.h>

void writeProcess(){
    for (int i = 0; i < 3; ++i) {
        printf("(%d) Schreibe Variable\n", getpid());
        sleep(1);
        printf("(%d) Unkritischer Bereich\n", getpid());
        sleep(1);
    }
}

void readProcess(){
    for (int i = 0; i < 3; ++i) {
        printf("(%d) Lese Variable\n", getpid());
        sleep(1);
        printf("(%d) Unkritischer Bereich\n", getpid());
        sleep(1);
    }
}

int main() {
    for (int i = 0; i < 7; ++i) {
        switch (fork()) {
            case -1:
                perror("FORK failed");
                exit(1);
            case 0:
                if (i < 5) {
                    writeProcess();
                } else {
                    readProcess();
                }
                exit(0);
            default:
                ;
        }
    }
    for(int i = 0; i < 7; i++){
        wait(NULL);
    }
    return 0;
}