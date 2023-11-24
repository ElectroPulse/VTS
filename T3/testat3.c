#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/wait.h>

int WRITER_ID = 0;
int MUTEX_ID = 1;
int READER_ID = 2;

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
    if(semctl(semSetId, WRITER_ID, SETVAL, 1)<0){
        perror("Error in semctl");
        exit(1);
    }
    if(semctl(semSetId, READER_ID, SETVAL, 0)<0){
        perror("Error in semctl");
        exit(1);
    }
    if(semctl(semSetId, MUTEX_ID, SETVAL, 1)<0){
        perror("Error in semctl");
        exit(1);
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
        P(WRITER_ID, semSetId);
        printf("[Writer-%d] Schreibe Variable begonnen\n", id);
        sleep((rand()%5)+1);
        printf("[Writer-%d] Schreibe Variable beendet\n", id);
        V(WRITER_ID, semSetId);
        sleep(1);
        // printf("[Writer-%d] Unkritischer Bereich begonnen\n", id);
        sleep((rand()%5)+1);
        // printf("[Writer-%d] Unkritischer Bereich beendet\n", id);
        sleep(1);
    }
}

void readProcess(int semSetId, int id){
    for (int i = 0; i < 3; ++i) {
        P(MUTEX_ID, semSetId);
        V(READER_ID, semSetId);
        if(semctl(semSetId, READER_ID, GETVAL) == 1){
            P(WRITER_ID, semSetId);
        }
        V(MUTEX_ID, semSetId);

        printf("[Reader-%d] Lese Variable begonnen\n", id);
        sleep((rand()%5)+1);
        printf("[Reader-%d] Lese Variable beendet\n", id);


        P(MUTEX_ID, semSetId);
        P(READER_ID, semSetId);
        if(semctl(semSetId, READER_ID, GETVAL) == 0){
            V(WRITER_ID, semSetId);
        }
        V(MUTEX_ID, semSetId);

        sleep(1);

        // printf("[Reader-%d] Unkritischer Bereich begonnen\n", id);
        sleep((rand()%5)+1);
        // printf("[Reader-%d] Unkritischer Bereich beendet\n", id);
        sleep(1);
    }
}

int main() {
    int semSetId = initSem(3, 3);

    /*
        Semaphoren
        [0] : Writer
        [1] : Mutex
        [2] : Reader
    */

    for (int i = 0; i < 7; ++i) {
        switch (fork()) {
            case -1:
                perror("FORK failed");
                exit(1);
            case 0:
                srand(getpid());
                sleep((rand()%5)+1);
                if (i < 5) {
                    writeProcess(semSetId, i);
                } else {
                    readProcess(semSetId, i);
                }
                printf("Child %d ended\n", i);
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