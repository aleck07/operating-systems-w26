#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define DATA_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <int> <float> <string>\n", argv[0]);
        return 1;
    }

    int int_val = atoi(argv[1]);
    float float_val = atof(argv[2]);
    const char *str_val = argv[3];

    int fd = open("./data.dat", O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    void *data = mmap(NULL, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    int *int_ptr = (int *)data;
    float *float_ptr = (float *)(data + sizeof(int));
    char *str_ptr = (char *)(data + sizeof(int) + sizeof(float));

    *int_ptr = int_val;
    *float_ptr = float_val;
    strcpy(str_ptr, str_val);

    printf("Stored: %d %f %s\n", *int_ptr, *float_ptr, str_ptr);

    munmap(data, DATA_SIZE);
    close(fd);

    return 0;
}