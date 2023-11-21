#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define PHILOSOPH_COUNT 5

int createKey(char *p_path) {
    int key;
    if ((key = ftok(p_path, '2')) < 0) {
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

void P(int semSetId, int forks[], int count) {
    struct sembuf semaphore[count];
    for (int i = 0; i < count; ++i) {
        semaphore[i].sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
        semaphore[i].sem_op = -1; // Block
        semaphore[i].sem_num = forks[i];
    }
    if (semop(semSetId, semaphore, count)) { // 1 size of array
        perror("Error in semop P()");
        exit(1);
    }
}

void V(int semSetId, int forks[], int count) {
    struct sembuf semaphore[count];
    for (int i = 0; i < count; ++i) {
        semaphore[i].sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
        semaphore[i].sem_op = 1; // Unblock
        semaphore[i].sem_num = forks[i];
    }
    if (semop(semSetId, semaphore, count)) { // 1 size of array
        perror("Error in semop P()");
        exit(1);
    }
}

void printFormatted (int philosopher, int firstFork, int secondFork, char *p_text){
    printf("[Philosopher-%d] (%d, %d) %s\n", philosopher, firstFork, secondFork, p_text);
}

void think(int philosopher, int firstFork, int secondFork) {
    printFormatted(philosopher, firstFork, secondFork, "started thinking...");
    sleep(rand() % 10 + 1);
    printFormatted(philosopher, firstFork, secondFork, "stopped thinking...");
}

void eat(int philosopher, int firstFork, int secondFork) {
    printFormatted(philosopher, firstFork, secondFork, "started eating...");
    sleep(rand() % 10 + 1);
    printFormatted(philosopher, firstFork, secondFork, "stopped eating...");
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
                for (int j = 0; j < 5; j++) {
                    int forks[2] = {i, (i+1)%5};
                    // Thinking
                    think(i, forks[0], forks[1]);
                    // Eating
                    P(semSetId, forks,  2);
                    eat(i, forks[0], forks[1]);
                    V(semSetId, forks,  2);
                }
                printf("Philosoph %d died\n", i);
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