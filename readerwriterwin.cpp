#include <stdio.h>

#include <windows.h>

#include <stdint.h>

#define NUM_READERS 7

#define NUM_WRITERS 5

typedef HANDLE semaphore;

semaphore s;

semaphore wrt;

int rc = 0;

int item = 1;

void down(semaphore *sem) {

    WaitForSingleObject(*sem, INFINITE);

}

void up(semaphore *sem) {

    ReleaseSemaphore(*sem, 1, NULL);

}

DWORD WINAPI reader(void *args) {

    int id = (intptr_t)args;

    down(&s);

    rc++;

    if (rc == 1) {

        down(&wrt);  // first reader locks writer

    }

    up(&s);

    printf("Reader %d enters (reading item = %d)\n", id, item);

    down(&s);

    rc--;

    if (rc == 0) {

        up(&wrt);   // last reader unlocks writer

    }

    up(&s);

    return 0;

}

DWORD WINAPI writer(void *args) {

    int id = (intptr_t)args;

    down(&wrt);

    printf("Writer %d enters (writing item = %d)\n", id, item++);

    up(&wrt);

    return 0;

}

int main() {

    HANDLE r[NUM_READERS], w[NUM_WRITERS];

    s = CreateSemaphore(NULL, 1, 1, NULL);

    wrt = CreateSemaphore(NULL, 1, 1, NULL);

    int read_id[NUM_READERS];

    int write_id[NUM_WRITERS];

    for (int i = 0; i < NUM_READERS; i++) {

        read_id[i] = i + 1;

        r[i] = CreateThread(NULL, 0, reader, (void*)(intptr_t)read_id[i], 0, NULL);

    }

    for (int i = 0; i < NUM_WRITERS; i++) {

        write_id[i] = i + 1;

        w[i] = CreateThread(NULL, 0, writer, (void*)(intptr_t)write_id[i], 0, NULL);

    }

    for (int i = 0; i < NUM_READERS; i++) {

        WaitForSingleObject(r[i], INFINITE);

        CloseHandle(r[i]);

    }

    for (int i = 0; i < NUM_WRITERS; i++) {

        WaitForSingleObject(w[i], INFINITE);

        CloseHandle(w[i]);

    }

    CloseHandle(s);

    CloseHandle(wrt);

    return 0;

}

