#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

int SEM_ID;

int initSem() {
    int sem_key;
    int sem_id;

    // sem erzeugen
    if ((sem_key = ftok("/home/markus", '1')) < 0) {
        perror("Error in ftok");
        exit(1);
    }
    if ((sem_id = semget(sem_key, 1, IPC_CREAT | 0666)) < 0) {
        perror("Error in semget");
        exit(1);
    }

    int sem_num = 0;
    if (semctl(sem_id, sem_num, SETVAL, 1) < 0) {
        perror("Error in semctl");
        exit(1);
    }
    return sem_id;
}

void V(int sem_num) {
    struct sembuf semaphore;
    semaphore.sem_num = sem_num;
    semaphore.sem_op = 1; // Unblock
    semaphore.sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
    if (semop(SEM_ID, &semaphore, 1)) { // 1 size of array
        perror("Error in semop V()");
        exit(1);
    }
}

void P(int sem_num) {
    struct sembuf semaphore;
    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1; // Block
    semaphore.sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
    if (semop(SEM_ID, &semaphore, 1)) { // 1 size of array
        perror("Error in semop P()");
        exit(1);
    }
}

void startSimulation(int process) {
    // Critical
    for (int i = 0; i < 3; ++i) {
        P(0);
        printf("Process %d enters a critical Zone\n", process);
        sleep(1);
        printf("Process %d leaves the critical Zone\n", process);
        V(0);
    }
    // Non-Critical
    printf("Process %d enters a non-critical Zone\n", process);
    sleep(1);
    printf("Process %d leaves the non-critical Zone\n", process);
}

int main() {
    int NUMBER_OF_PROCESSES = 3;
    // sem init
    SEM_ID = initSem();

    for (int process = 0; process < NUMBER_OF_PROCESSES; process++) {
        switch (fork()) {
            case -1:
                perror("FORK failed");
                exit(1);
            case 0:
                /*child*/
                printf("Child process %d (%d) started\n", process, getpid());
                startSimulation(process);
                exit(0);
            default:
                /*father*/
                ;

        }
    }
    sleep(15);
    printf("Father process stopped\n");

    return 0;
}