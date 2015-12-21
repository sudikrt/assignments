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
#include <pthread.h>

#define BACK_LOG 5

/*Enumeration for request_type*/
enum request_type {read_request = 30, write_request = 31};

/*Enumeration for error list*/
enum err_list {file_not_exist = 20, read_fail = 21, write_fail = 22,
                s_accept = 23, file_exist = 24};

/*Stat structure variable*/
struct stat st;

/*External Variable for error number*/
extern int errno;

/*Structure for the client request*/
typedef struct client_request
{
        int type;
        int client_fd;
        char* buf;
        void (*operation) (void*, int);
} C_Request;

/*Structure for the request queue*/
struct client_queue
{
        C_Request c_req;
        struct client_queue* next;
};
typedef struct client_queue C_Queue;

/*Function Decleration*/
void* sftp_rpc_server_init (void* port_id);
void* delete_queue ();

/*Variable declaration of the queue*/
C_Queue* head;

/*Variable declaration of the export directory*/
char* exp_dir;
char* dest_dir;
