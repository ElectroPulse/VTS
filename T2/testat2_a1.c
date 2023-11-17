#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define PHILOSOPH_COUNT 5

int createKey(char *p_path) {
    int key;
    if ((key = ftok(p_path, '3')) < 0) {
        perror("Error in ftok");
        exit(1);
    }
    return key;
}

int initSem(int semCount) {
    int semKey= createKey("/home/markus");
    int semSetId;

    if ((semSetId = semget(semKey, semCount, IPC_CREAT | 0666)) < 0) {
        perror("Error in semget");
        exit(1);
    }

    // Set Init sem-value to 1
    for (int i = 0; i < semCount; i++) {
        if (semctl(semSetId, i, SETVAL, 1) < 0) {
            perror("Error in semctl");
            exit(1);
        }
    }
    return semSetId;
}

void P(int semSetId, int firstSem, int secondSem) {
    int semAction = -1;
    struct sembuf semaphore;

    //First Sem
    semaphore.sem_num = firstSem;
    semaphore.sem_op = semAction; // Block
    semaphore.sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
    if (semop(semSetId, &semaphore, 1)) { // 1 size of array
        perror("Error in semop P()");
        exit(1);
    }

    //Second Sem
    semaphore.sem_num = secondSem;
    semaphore.sem_op = semAction; // Block
    semaphore.sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
    if (semop(semSetId, &semaphore, 1)) { // 1 size of array
        perror("Error in semop P()");
        exit(1);
    }
}

void V(int semSetId, int firstSem, int secondSem) {
    int semAction = 1;
    struct sembuf semaphore;

    //First Sem
    semaphore.sem_num = firstSem;
    semaphore.sem_op = semAction; // Block
    semaphore.sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
    if (semop(semSetId, &semaphore, 1)) { // 1 size of array
        perror("Error in semop P()");
        exit(1);
    }

    //Second Sem
    semaphore.sem_num = secondSem;
    semaphore.sem_op = semAction; // Block
    semaphore.sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
    if (semop(semSetId, &semaphore, 1)) { // 1 size of array
        perror("Error in semop P()");
        exit(1);
    }
}

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
    int semSetId = initSem(5);

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
                    P(semSetId, firstFork, secondFork);
                    eat(i);
                    V(semSetId, firstFork, secondFork);
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
    return 0;
}