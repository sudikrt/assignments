#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#define BACK_LOG 5

/*Enumeration for error list*/
enum err_list {file_not_exist = 20, read_fail = 21, write_fail = 22,
                s_accept = 23, file_exist = 24};

/*Stat structure variable*/
struct stat st;

extern int errno;



  
