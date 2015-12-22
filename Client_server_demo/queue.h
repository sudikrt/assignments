#include "socket_header.h"

/*Structure for the request queue*/
struct client_queue
{
        C_Request c_req;
        struct client_queue* next;
};

typedef struct client_queue C_Queue;

/*Variable declaration of the queue*/
C_Queue* head;