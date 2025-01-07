#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define SHM_KEY 12345
#define SHM_SIZE 1024

typedef struct {
    int counter;
    char message[100];
} SharedData;

// P 操作（等待信号量）
void semWait(int semid) {
    struct sembuf operation;
    operation.sem_num = 0;  // 使用信号量集中的第一个信号量
    operation.sem_op = -1; // 等待信号量
    operation.sem_flg = 0; // 无特殊标志
    semop(semid, &operation, 1);
    }

// V 操作（释放信号量）
void semSignal(int semid) {
    struct sembuf operation;
    operation.sem_num = 0; // 使用信号量集中的第一个信号量
    operation.sem_op = 1; // 释放信号量
    operation.sem_flg = 0; // 无特殊标志
    semop(semid, &operation, 1);
}

int main() {
    int shmid, semid;
    SharedData *sharedData;
    // 获取共享内存
    shmid = shmget(SHM_KEY, sizeof(SharedData), 0);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // 连接共享内存
    sharedData = (SharedData*)shmat(shmid, NULL, 0);
    if (sharedData == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    // 获取信号量
    semid = semget(SHM_KEY, 1, 0);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // 读取共享内存中的数据
    semWait(semid);

    printf("Message: %s", sharedData->message);
    printf("Counter: %d\n", sharedData->counter);

    semSignal(semid);

    // 分离共享内存
    if (shmdt(sharedData) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}

