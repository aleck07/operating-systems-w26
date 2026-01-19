# File Scanner Utility

`usage: filescanner [directory]`

## Description

In this file scanner program, it starts at the `main`, where we expect a single argument. Error checking is included to make sure user specified 1 argument. Then the `readDirectory(mainArg)` function is called. In this function, we use a while loop that calls `buildPath(fullPath, dirPath, dir)` and checks to see if the output is a file or directory. If it is a directory, then we call `readDirectory()` again on that directory. Once our `dir` variable is `NULL`, then the while loop stops. After the while loop we print out the `dirPath` and the `byteTotal` to the terminal. Then return `byteTotal`.

## Function call diagram:
```
main
--- readDirectory(dirPath)
    --- opendir(dirPath)
    --- readdir(fd) (Part of the base case)
    --- buildPath(fullPath, dirPath, dir)
    ------- sprintf(...)              
    --- stat(fullPath, &statbuf)
    --- readDirectory(fullPath) (*Recursive*)
    --- printf(...)
    --- closedir(fd)
    --- return byteTotal
```

