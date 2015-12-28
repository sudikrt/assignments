#include "socket_header.h"
/*
 * This check the file exists in export folder if exists return file_exists
 * Input:
 *      char* file_name :       Specifies the name of the file.
 * Output:
 *      int             :       If file exists then 0 will returned otherwise -1. 
 * */
int check_file_exist (char * file_name)
{
        int ret = -1;

        /* Change searching directory to the export directory. */
        ret = chdir(exp_dir);
        if (ret == -1)
        {
                fprintf (stderr, "\t%s\n", strerror(errno));
                ret = errno;
                goto out;   
        }

        /* Searches for the named file specified in the input. */
        ret = stat(file_name, &st);
        if (ret == -1)
        {
                ret = errno;
                fprintf (stderr, "\t%s\n", strerror(errno));
                goto out;
        }
        ret = file_exist;
        out:
            return ret;

} 