#include "utils/queue.h"
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
#include <signal.h>

#define BACK_LOG 5

/* Enumeration for request_type */
enum request_type {read_request = 30, write_request = 31, quit = 32};

/* Enumeration for queue_errors */
enum queue_error {queue_ful = 40, queue_nful = 41};

/* Enumeration for error list */
enum err_list {file_not_exist = 20, read_fail = 21, write_fail = 22,
                s_accept = 23, file_exist = 24};

/*Stat structure variable*/
struct stat st;

/* External Variable for error number */
extern int errno;

/* Structure for the client request */
typedef struct client_request
{
        int type;
        int client_fd;
        char* buf;
        void (*operation) (void*, int);
} C_Request;

queue_t* queue_obj;

/* Function Decleration for initilization of server */
void* sftp_rpc_server_init (void* port_id);

/* Function for processing the client_request */
void* service_queue (void* arg);

/* Function for handling the read request */ 
void read_handler (void* arg, int client_fd);


int handle_request (void *);

/* Variable declaration of the export directory
 * here exp_dir is server-side directory
 * here dest_dir is on client-side export directory
 * */
char* exp_dir;
char* dest_dir;

/* Variable decleration of lock object and condition variables */
pthread_mutex_t lock;
int thread_flag;
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;

