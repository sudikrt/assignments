#include "utils/list.h"
#include "utils/pool.h"
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
enum request_type {file_request = 30, write_request = 31, file_operation = 32, 
                        open_file_request = 33, quit = 34, close_file_request = 35,
                        read_in_chunks = 36};

/* Enumeration for file opening modes. */
enum mode_type {read_mode = 1, write_mode = 2};

/* Enumeration for queue_errors */
enum queue_error {queue_ful = 40, queue_nful = 41};

/* Enumeration for error list */
enum err_list {file_not_exist = 20, read_fail = 21, write_fail = 22,
                file_exist = 23, file_already_open = 24, 
                file_already_closed = 25, fail_response = 27, 
                success_response = 26, insufficient_arg = 28,
                file_open_error = 29};

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

typedef struct service_obj
{
        int client_fd;
        queue_t* obj;
} service_obj_t;

typedef struct fd_data {
        int fd;
        char* file_name;
        unsigned int mode;
        struct fd_data* next;
} fd_data_t;

fd_data_t* fd_node;


/* Function Decleration for initilization of server */
void* sftp_rpc_server_init (void* port_id);

/* Function for processing the client_request */
void* service_queue (void* arg);

//void* server_response (void* arg);

/* Function for handling the read request */ 
void read_handler (void* arg, int client_fd);

void file_handler (void* arg, int client_fd);

void open_file_handler (void* arg, int client_fd);

void close_file_handler (void* arg, int client_fd);

void read_chunks_handler (void* arg, int client_fd);

int handle_request (void *);

/* Variable declaration of the export directory
 * here exp_dir is server-side directory
 * here dest_dir is on client-side export directory
 * */
char* exp_dir;
char* dest_dir;

int file_desc_client;

/* Creating the thread pool variable */
thread_pool_t *pool;

/* Variable decleration of lock object and condition variables */
pthread_mutex_t lock;
int thread_flag;
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;

