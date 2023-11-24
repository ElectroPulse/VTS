#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/wait.h>

int currentReaders = 0;
int writer = 0;
int mutex = 1;

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

    // Set Init
    for (int i = 0; i < sem_count; ++i) {
        if(semctl(semSetId, i, SETVAL, 1) < 0 ) {
            perror("Error in semctl");
            exit(1);
        }
    }

    return semSetId;
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

void writeProcess(int semSetId, int id){
    for (int i = 0; i < 3; ++i) {
        P(writer, semSetId);
        printf("[Writer-%d] Schreibe Variable begonnen\n", id);
        sleep(1);
        printf("[Writer-%d] Schreibe Variable beendet\n", id);
        V(writer, semSetId);

        // printf("[Writer-%d] Unkritischer Bereich begonnen\n", id);
        sleep(1);
        // printf("[Writer-%d] Unkritischer Bereich beendet\n", id);
    }
}

void readProcess(int semSetId, int id){
    for (int i = 0; i < 3; ++i) {
        P(mutex, semSetId);
        currentReaders++;
        if(currentReaders == 1){
            P(writer, semSetId);
        }
        V(mutex, semSetId);

        printf("[Reader-%d] Lese Variable begonnen\n", id);
        sleep(1);
        printf("[Reader-%d] Lese Variable beendet\n", id);


        P(mutex, semSetId);
        currentReaders--;
        if(currentReaders == 0){
            V(writer, semSetId);
        }
        V(mutex, semSetId);

        // printf("[Reader-%d] Unkritischer Bereich begonnen\n", id);
        sleep(1);
        // printf("[Reader-%d] Unkritischer Bereich beendet\n", id);
    }
}

int main() {
    int semSetId = initSem(2, 3);

    /*
        Semaphoren
        [0] : Writer
        [1] : Mutex
    */

    for (int i = 0; i < 7; ++i) {
        switch (fork()) {
            case -1:
                perror("FORK failed");
                exit(1);
            case 0:
                if (i < 5) {
                    writeProcess(semSetId, i);
                } else {
                    readProcess(semSetId, i);
                }
                exit(0);
            default:
                ;
        }
    }
    return 0;
}