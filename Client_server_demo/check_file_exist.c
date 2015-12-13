#include "socket_header.h"
#include <errno.h>

extern int errno;
/*This check the file exists in export folder if exists return file_exists*/
int check_file_exist (char * file_name, char * p_dir)
{
    int ret = -1;
    ret = chdir(p_dir);
    if (ret == -1)
    {
        fprintf (stderr, "\t%s\n", strerror(errno));
        ret = errno;
        goto out;   
    }    
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