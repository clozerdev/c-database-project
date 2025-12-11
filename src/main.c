#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void
print_usage (char *argv[])
{
    printf ("[*] Usage: %s -n -f <database file>\n", argv[0]);
    printf ("\t -n: Creates new database file\n");
    printf ("\t -f: Specify the filepath\n");

    return;
}

int
main (int argc, char *argv[])
{
    const char *filepath = NULL;

    int opt;
    int dbfd = STATUS_ERROR;
    int new_file = 0;

    struct dbheader_t *header = NULL;

    while ((opt = getopt (argc, argv, "nf:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            new_file = 1;
            break;
        case 'f':
            filepath = optarg;
            break;
        default: /* ? */
            print_usage (argv);
            exit (EXIT_FAILURE);
        }
    }

    if (filepath == NULL)
    {
        printf ("[!] Filepath is a required argument!\n");
        print_usage (argv);
        return 0;
    }

    if (new_file)
    {
        dbfd = create_db_file (filepath);
        if (dbfd == STATUS_ERROR)
            return STATUS_ERROR;

        if (create_db_header (dbfd, &header) == STATUS_ERROR)
        {
            printf ("[!] Error creating the DB header, exiting...\n");
            return STATUS_ERROR;
        }
    }
    else
    {
        dbfd = open_db_file (filepath);
        if (dbfd == STATUS_ERROR)
            return STATUS_ERROR;

        if (validate_db_header (dbfd, &header) == STATUS_ERROR)
        {
            printf ("Failed to validate header\n");
            return STATUS_ERROR;
        }
    }

    output_file (dbfd, header);

    return STATUS_SUCCESS;
}
