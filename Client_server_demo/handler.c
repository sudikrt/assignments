#include "socket_header.h"
int handle_request (void* arg)
{
        C_Request request = *((C_Request*) arg);
        /*Calls the request handler based of the type of 
        * the request*/
        printf ("\t Inside Request %s\n", request.buf);
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
        return 0;
}