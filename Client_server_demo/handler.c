#include "socket_header.h"
/*
 * This is the call back funnction which calls the
 * appropriate function based on the type of the request.
 * Input :
 *      void*   :       specifies the type of the request 
 *                      and its functions.
 * Output :
 *      int     :       specifies successful function invocation
 * */
int handle_request (void* arg)
{
        C_Request request = *((C_Request*) arg);
        /*Calls the request handler based of the type of 
        * the request*/
        switch (request.type)
        {
                case file_request:
                                request.operation (request.buf,
                                                        request.client_fd);
                                break;
                case open_file_request:
                                request.operation (request.buf,
                                                        request.client_fd);
                                break;
                case close_file_request:
                                request.operation (request.buf, 
                                                        request.client_fd);
                                break;
                case read_in_chunks:
                                request.operation (request.buf,
                                                        request.client_fd);
                                break;
        }
        return 0;
}