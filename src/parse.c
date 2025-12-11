#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

int
output_file (int fd, struct dbheader_t *dbheader)
{
    if (fd < 0)
    {
        printf ("Received invalid file descriptor\n");
        return STATUS_ERROR;
    }

    dbheader->version = htons (dbheader->version);
    dbheader->count = htons (dbheader->count);
    dbheader->magic = htonl (dbheader->magic);
    dbheader->filesize = htonl (dbheader->filesize);

    if (lseek (fd, 0, SEEK_SET) == -1)
    {
        perror ("LSEEK");
        return STATUS_ERROR;
    }

    if (write (fd, dbheader, sizeof (struct dbheader_t)) == -1)
    {
        perror ("Writing file");
        return STATUS_ERROR;
    }

    return STATUS_SUCCESS;
}

int
validate_db_header (int fd, struct dbheader_t **header_out)
{
    if (fd < 0)
    {
        printf ("Received invalid file descriptor\n");
        return STATUS_ERROR;
    }

    struct dbheader_t *header = calloc (1, sizeof (struct dbheader_t));
    if (header == NULL)
    {

        perror ("[!] Error allocating memory for validating DB header");
        return STATUS_ERROR;
    }

    if (read (fd, header, sizeof (struct dbheader_t))
        != sizeof (struct dbheader_t))
    {
        perror ("Unable to read header");
        free (header);
        return STATUS_ERROR;
    }

    header->version = ntohs (header->version);
    header->count = ntohs (header->count);
    header->magic = ntohl (header->magic);
    header->filesize = ntohl (header->filesize);

    if (header->version != 0x1)
    {
        printf ("Invalid header version!\n");
        free (header);
        return STATUS_ERROR;
    }

    if (header->magic != MAGIC_NUMBER)
    {
        printf ("Invalid magic number!\n");
        free (header);
        return STATUS_ERROR;
    }

    struct stat dbstat = { 0 };
    if (fstat (fd, &dbstat) == -1)
    {
        perror ("Error on getting stat");
        free (header);
        return STATUS_ERROR;
    }

    if (header->filesize != dbstat.st_size)
    {
        printf ("File size doesn't match!\n");
        free (header);
        return STATUS_ERROR;
    }

    *header_out = header;

    return STATUS_SUCCESS;
}

int
create_db_header (struct dbheader_t **header_out)
{
    struct dbheader_t *header = calloc (1, sizeof (struct dbheader_t));
    if (header == NULL)
    {
        perror ("[!] Error allocating memory for DB header");
        return STATUS_ERROR;
    }

    header->version = 0x1;
    header->count = 0;
    header->magic = MAGIC_NUMBER;
    header->filesize = sizeof (struct dbheader_t);

    *header_out = header;

    return STATUS_SUCCESS;
}
