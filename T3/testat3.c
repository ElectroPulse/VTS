#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/wait.h>

int currentReaders = 0;

int initSem(int sem_count, char proj_id) {
    int semKey;
    int semSetId;

    if ((semKey = ftok("/home/markus/", proj_id)) < 0) {
        perror("Error in ftok");
        exit(1);
    }

    if ((semSetId = semget(semKey, sem_count, IPC_CREAT | 0666)) < 0) {
        perror("Error in semget");
        exit(1);
    }

    // Set Init sem-value to 1
    for (int i = 0; i < sem_count; i++) {
        if (semctl(semSetId, i, SETVAL, 1) < 0) {
            perror("Error in semctl");
            exit(1);
        }
    }
    return semSetId;
}

void V(int sem_num, int sem_id) {
    struct sembuf semaphore;
    semaphore.sem_num = sem_num;
    semaphore.sem_op = 1; // Unblock
    semaphore.sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
    if (semop(sem_id, &semaphore, 1)) { // 1 ist Groesse Array
        perror("Error in semop V()");
        exit(1);
    }
}

void P(int sem_num, int sem_id) {
    struct sembuf semaphore;
    semaphore.sem_num = sem_num;
    semaphore.sem_op = -1; // Block
    semaphore.sem_flg = ~(IPC_NOWAIT | SEM_UNDO);
    if (semop(sem_id, &semaphore, 1)) { // 1 ist Groesse Array
        perror("Error in semop P()");
        exit(1);
    }
}

void writeProcess(int writer){
    for (int i = 0; i < 3; ++i) {
        P(writer, 0);

        printf("(%d) Schreibe Variable\n", getpid());
        sleep(1);
        printf("(%d) Unkritischer Bereich\n", getpid());
        sleep(1);

        V(writer, 0);
    }
}

void readProcess(int mutex, int writer){
    for (int i = 0; i < 3; ++i) {
        P(mutex, 0);
        currentReaders++;
        if(currentReaders == 1){
            P(writer, 0);
        }
        V(mutex, 0);

        printf("(%d) Lese Variable\n", getpid());
        sleep(1);
        printf("(%d) Unkritischer Bereich\n", getpid());
        sleep(1);

        P(mutex, 0);
        currentReaders--;
        if(currentReaders == 0){
            V(writer, 0);
        }
        V(mutex, 0);
    }
}

int main() {
    int writer = initSem(1, 1);
    int mutex = initSem(1, 2);

    for (int i = 0; i < 7; ++i) {
        switch (fork()) {
            case -1:
                perror("FORK failed");
                exit(1);
            case 0:
                if (i < 5) {
                    writeProcess(writer);
                } else {
                    readProcess(mutex, writer);
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