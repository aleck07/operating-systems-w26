#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define REC_COUNT (1024 * 1024)                              // record count, each 32 bytes
#define REC_SIZE 32                                          // bytes
#define FILE_SIZE (sizeof(int) * 2 + (REC_COUNT * REC_SIZE)) // file size in bytes
#define MAX_COUNT 20000

void add_next_record(int fd, int count, const char *prefix)
{
    void *data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        perror("mmap");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        struct flock header_lock = {
            .l_type = F_WRLCK,
            .l_whence = SEEK_SET,
            .l_start = 0,
            .l_len = sizeof(int) * 2,
        };

        if (fcntl(fd, F_SETLKW, &header_lock) == -1)
        {
            perror("fcntl");
            munmap(data, FILE_SIZE);
            return;
        }

        int *header = (int *)data;
        int offset = header[0];
        header[0] = offset + REC_SIZE;
        header[1] = header[1] + 1;

        header_lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &header_lock);

        off_t record_start = sizeof(int) * 2 + offset;
        struct flock record_lock = {
            .l_type = F_WRLCK,
            .l_whence = SEEK_SET,
            .l_start = record_start,
            .l_len = REC_SIZE,
        };

        if (fcntl(fd, F_SETLKW, &record_lock) == -1)
        {
            perror("fcntl");
            munmap(data, FILE_SIZE);
            return;
        }

        char entry[REC_SIZE];
        memset(entry, 0, sizeof(entry));
        snprintf(entry, sizeof(entry), "%s %d", prefix, i);
        memcpy((char *)data + record_start, entry, sizeof(entry));

        record_lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &record_lock);
    }

    munmap(data, FILE_SIZE);
}

void dump_log(int fd)
{
    void *data = mmap(NULL, FILE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        perror("mmap");
        return;
    }

    struct flock file_lock = {
        .l_type = F_RDLCK,
        .l_whence = SEEK_SET,
        .l_start = 0,
        .l_len = 0,
    };

    if (fcntl(fd, F_SETLKW, &file_lock) == -1)
    {
        perror("fcntl");
        munmap(data, FILE_SIZE);
        return;
    }

    int *header = data;
    int count = header[1];
    char *record = (char *)(header + 2);
    for (int i = 0; i < count; i++)
    {
        printf("%d: %s\n", i, record + (i * REC_SIZE));
    }

    file_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &file_lock);
    munmap(data, FILE_SIZE);
}

int main(int argc, char *argv[])
{
    int fd;

    if (argc < 3 || (argc - 1) % 2 != 0)
    {
        fprintf(stderr, "Usage: %s <count> <prefix> [<count> <prefix> ...]\n", argv[0]);
        return 1;
    }

    if ((fd = open("data.dat", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        perror("open");
        return 1;
    }

    if (ftruncate(fd, FILE_SIZE) == -1)
    {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    int pair_count = (argc - 1) / 2;
    for (int i = 0; i < pair_count; i++)
    {
        int count = atoi(argv[2 * i + 1]);
        const char *prefix = argv[2 * i + 2];

        pid_t pid = fork();
        // Child process
        if (pid == 0)
        {
            add_next_record(fd, count, prefix);
            close(fd);
            return 0; // child process exits after adding records
        }
    }

    for (int i = 0; i < pair_count; i++)
    {
        wait(NULL); // wait for all child processes to finish
    }

    dump_log(fd);
    close(fd);
}