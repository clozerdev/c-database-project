#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"
#include "file.h"

int
create_db_file (const char *filename)
{
    int fd = open (filename, O_RDONLY);
    if (fd != -1)
    {
        close (fd);
        printf ("[!] File already exists!\n");
        return STATUS_ERROR;
    }

    fd = open (filename, O_RDWR | O_CREAT, 0644);
    if (fd == -1)
    {
        perror ("[!] Error creating file\n");
        return STATUS_ERROR;
    }

    return fd;
}

int
open_db_file (const char *filename)
{
    int fd = open (filename, O_RDWR);
    if (fd == -1)
    {
        perror ("[!] Error opening file");
        return STATUS_ERROR;
    }

    return fd;
}
