#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <Semaphoren/sem.h>

#define PHILOSOPH_COUNT 5

void think(int philosopher) {
    printf("Philosopher %d started thinking\n", philosopher);
    sleep(rand() % 10 + 1);
    printf("Philosopher %d finished thinking\n", philosopher);
}

void eat(int philosopher) {
    printf("Philosopher %d started eating\n", philosopher);
    sleep(rand() % 10 + 1);
    printf("Philosopher %d finished eating\n", philosopher);
}

int main() {
    SemSet *semSet = InitSemSet(5);

    for (int i = 0; i < PHILOSOPH_COUNT; ++i) {
        switch (fork()) {
            case -1:
                perror("FORK failed");
                exit(1);
            case 0:
                /*child*/
                printf("Child process %d (%d) started\n", i, getpid());
                srand(getpid());
                for (int j = 0; j < 10; j++) {
                    // Thinking
                    think(i);
                    // Eating
                    int firstFork = i;
                    int secondFork = (i + 1) % 5;
                    int indices[2] = {firstFork, secondFork};
                    P(semSet, indices, 2);
                    eat(i);
                    V(semSet, indices, 2);
                }
                exit(0);
            default:
                /*father*/
                ;
        }
    }
    for(int i = 0; i < PHILOSOPH_COUNT; i++){
        wait(NULL);
    }
    DisposeSemSet(semSet);
    return 0;
}