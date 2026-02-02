#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define DATA_SIZE 1024

int main(void)
{
    int int_val;
    float float_val;
    const char *str_val;

    int fd = open("./data.dat", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    void *data = mmap(NULL, DATA_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    int *int_ptr = (int *)data;
    float *float_ptr = (float *)(data + sizeof(int));
    char *str_ptr = (char *)(data + sizeof(int) + sizeof(float));

    int_val = *int_ptr;
    float_val = *float_ptr;
    str_val = str_ptr;
    printf("Read: %d %f %s\n", int_val, float_val, str_val);
    munmap(data, DATA_SIZE);
    close(fd);
    return 0;
}