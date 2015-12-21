#include "socket_header.h"

void read_handler (void* arg, int client_fd)
{
        int ret         =       0;
        int len         =       0;
        char* buffer;
        char* data;
        char* req_buf;

        data = (char*) calloc (1,100);
        req_buf = (char*) calloc (1, 1024);
        
        req_buf = (char*) arg;
        /*Clear the contents of data*/
        memset (data, 0, 100);

        /*Read the length for the file_name*/
        memcpy (data, &req_buf [2], 2);
        len = atoi (data);

        /*Clear the content of data and read the file_name*/
        memset (data, 0, 100);
        memcpy (data, &req_buf [4], len);

        /*Check the file exist in the exp_dir*/
        ret = check_file_exist(data);

        /*If file exists*/
        if (ret == file_exist)
        {
                memset (req_buf, 0, 1024);
                sprintf (req_buf, "%d", file_exist);
                ret = write (client_fd, req_buf, 1024);
                if (ret < 0)
                {
                        printf ("\tCommunication error\n");
                        return;
                }
                
                ret = send_data_to_client (client_fd, data);
                if(ret < 0)
                {
                        printf ("\tError in sending file \n");
                        return;
                }
                printf("\tFinished sending\n");
        }
        /*File not exists send the error message to client*/
        else
        {
                /*Set the the erro code*/
                memset (req_buf, 0, sizeof(buffer));
                sprintf (req_buf, "%d", ENOENT);
                ret = write (client_fd, req_buf, 1024);
                if (ret < 0)
                {
                        printf ("\tCommunication error\n");
                        return;
                }
        }
}

void write_handler (void* arg, int client_fd)
{
        
}

/*Function to add the client request to the queue*/
void add_queue (void* arg)
{
        C_Queue *current;
        C_Request data;
        current = head;
        pthread_mutex_t lock;

        data = *((C_Request*) arg) ;

        while (current -> next != head) 
        {
                current = current -> next;
        }
        /*Allocate the memory the for the new data
         * and store the data in in*/
        //pthread_mutex_lock(&lock);
        current -> next = (C_Queue*) calloc (1, sizeof(C_Queue));
        current = current -> next;
        current -> c_req = data;
        current -> next = head;
        //pthread_mutex_unlock(&lock);
        printf ("\tRequest is inserted\n");
        return;
}

/*Function to remove the request from the queue and process its request*/
void* delete_queue ()
{
        C_Queue *previous ;
        C_Request request;
        while (1){
                if(head -> next != head)
                {
                        /*Removes the request on the top of the queue*/
                        previous = head -> next;
                        head -> next = previous -> next;
                        request = previous -> c_req;
                        
                        bzero (previous, sizeof(previous));
                        /*Calls the request handler based of the type of 
                         * the request*/
                        switch (request.type)
                        {
                                case read_request:
                                                request.operation (request.buf, 
                                                                request.client_fd);
                                                break;
                                case write_request:
                                                request.operation (request.buf, 
                                                                request.client_fd);
                                                break;
                        }
                }
        }
}

/*Recieves the client request and addes it to the queue*/
void server_response (int client_fd)
{
        char* buffer;
        char* data;
        int ret         =       0;
        int res         =       0;
        C_Request c_request;
        buffer = (char*) calloc (1,1024);
        data = (char*) calloc (1,100);
        pthread_mutex_t lock;
        while (1)
        {
                ret = read (client_fd, buffer, 1024);
                if(ret != 0)
                {
                        /*Read the type of the request from the client request*/
                        memcpy (data, buffer, 2);
                        res = atoi (data);
                        
                        switch (res)
                        {
                                case read_request:
                                                c_request.type = read_request;
                                                c_request.buf = buffer;
                                                c_request.client_fd = client_fd;
                                                c_request.operation = (void*) read_handler;
                                                break;
                                case write_request:
                                                c_request.type = write_request;
                                                c_request.buf = buffer;
                                                c_request.client_fd = client_fd;
                                                c_request.operation = (void*) write_handler;
                                                break;
                        }
                        pthread_mutex_lock(&lock);
                        add_queue ((void*) &c_request);
                        pthread_mutex_unlock(&lock);

                }
        }

        return ;
}