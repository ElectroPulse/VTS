#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void think();
void eat();

int main() {
    think();
    eat();
    return 0;
}

void think(int philosopher) {
    printf("Philosopher %d started thinking\n", philosopher);
    sleep(rand() % 10);
    printf("Philosopher %d finished thinking\n", philosopher);
}

void eat(int philosopher) {
    printf("Philosopher %d started eating\n", philosopher);
    sleep(rand() % 10);
    printf("Philosopher %d finished eating\n", philosopher);
}