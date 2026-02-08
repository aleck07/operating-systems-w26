#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define REC_COUNT (1024 * 1024) // record count, each 32 bytes
#define REC_SIZE 32 // bytes 
#define FILE_SIZE (REC_COUNT * REC_SIZE) // file size in bytes
#define MAX_COUNT 20000

void add_next_record)(int fd, void *data){
    struct flock lock = {
        .l_type = F_WRLCK,
        .l_whence = SEEK_SET,
        .l_start = 0,
        .l_len = 0,
    };
    int* count = data;
    char* record = (char*)(count + 1);
    int index = *count;
    record[index * REC_SIZE] = 'a' + (index % 26); //
    fcntl(fd, F_SETLKW, &lock); // acquire the lock
    (*count)++; // update the count
    lock.l_type = F_UNLCK; // release the lock
    fcntl(fd, F_SETLK, &lock);
}

int main(int argc, char* argv[]){
    int fd;
    int file_size = sizeof(int) * 2 * FILE_SIZE;


    for (int i =    if ((fd = open("data.dat", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        perror("open");
        return 1;
    }
}