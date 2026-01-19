#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

// Creates full file path for file
void buildPath(char *dest, char *dirPath, struct dirent *dir){
    snprintf(dest, 256, "%s/%s", dirPath, dir->d_name);
}

ssize_t readDirectory(char *dirPath){
    struct dirent *dir;
    DIR *fd;

    ssize_t byteTotal = 0;
    fd = opendir(dirPath);

    // Will result in error if file can't open
    if(fd == NULL) {
        perror("opendir");
        return 0;
    }

    while((dir = readdir(fd)) != NULL){
        char fullPath[256];
        buildPath(fullPath, dirPath, dir);
        struct stat statbuf;
        
        // Get the file information of the path, and error check it.
        if(stat(fullPath, &statbuf) == -1) {
            perror("stat");
            continue;
        }
        
        // Ignores . and .. but also hidden files :( 
        if(dir->d_name[0] == '.'){
            continue;
        }

        // Checks to see if the file is a directory or file, then adds the byte total of the file or dives deeper
        if(S_ISREG(statbuf.st_mode)){
            byteTotal += statbuf.st_size;
        } else if(S_ISDIR(statbuf.st_mode)){
            byteTotal += readDirectory(fullPath);
        }
    }
    printf("%s: %ld\n", dirPath, byteTotal);
    closedir(fd);
    return byteTotal;
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "usage: filescanner [directory]\n");
        return 1;
    }
    readDirectory(argv[1]);
    return 0;
}