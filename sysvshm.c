#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1048576

int main(void)
{
    int fd = open("/tmp/mem.temp", O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        perror("open mem.temp");
        return 1;
    }
    close(fd);

    key_t key = ftok("/tmp/mem.temp", 1);
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int shm = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shm == -1) {
        perror("shmget");
        return 1;
    }

    void *res = shmat(shm, NULL, NULL);
    if (res == (void *)-1) {
        perror("shmat");
        return 1;
    }

    memset(res, 42, sizeof(char) * SHM_SIZE);

    pause();

    shmdt((void *)res);

    return 0;
}

