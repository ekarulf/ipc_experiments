#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "string_compat.h"

#define PATH_LEN 1024
#define DATA_LEN 2048

int main (int argc, char const *argv[])
{
    const char* template = argv[1]; // eg. /tmp/bench.XXXX
    int iterations = atoi(argv[2]); // eg. 10000

    char filename[PATH_LEN];
    int fd;
    void* data;
    
    strlcpy(filename, template, PATH_LEN);
    while(iterations--)
    {
        // Run variables
        fd = -1;
        data = NULL;
        // strlcpy(filename, template, PATH_LEN);
        
#ifdef USE_SHM
        if ((fd = shm_open(filename, O_RDWR | O_CREAT | O_EXCL)) < 0) {
            perror("Unable to create shared memory");
            break;
        }
        
        if (shm_unlink(filename) == -1)
        {
            perror("Unable to unlink shared memory");
            goto error;
        }
#else
        if ((fd = open(filename, O_RDWR | O_CREAT | O_EXCL, 0600)) < 0) {
            perror("Unable to create temporary file");
            break;
        }

        if (unlink(filename) == -1)
        {
            perror("Unable to unlink temporary file");
            goto error;
        }
#endif
        if (ftruncate(fd, DATA_LEN) == -1)
        {
            perror("Error priming virtual memory file");
            goto error;
        }

        data = mmap(0, DATA_LEN, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED)
        {
            perror("Error mapping virtual memory");
            goto error;
        }
        
        //
        // Do stuff with data
        //
        
        if (munmap(data, DATA_LEN) == -1)
        {
            perror("Error unmapping virtual memory");
            goto error;
        }
        
        if (close(fd) == -1)
        {
            perror("Error closing temporary file");
            goto error;
        }
    }
    return 0;

error:
    if (data != NULL)
        munmap(data, DATA_LEN);
    
    if (fd >= 0)
        close(fd);
    
    return 1;
}
