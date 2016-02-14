#include "socket_header.h"

int
handle_file_operation (int client_fd) {
        
        int ret         =       -1;
        int len         =       0;
        char* file_name;
        char* buffer;
        
        while (1) {
                pritnf ("\t1.Open \n\t2.Read_num_bytes \n\t3.Close \n\t4.Exit\n\tEnter your choice\n");
                scanf ("%d", &choice);
                
                switch (choice) {
                        case 1:
                                printf ("\tEnter the file name :");
                                file_name = (char*) calloc (1, 1024);
                                scanf ("%s", file_name);
read_choice:                    printf ("\tEnter the Mode you want to open :\n");
                                printf ("1 for read_mode, 2 for write_mode :\n");
                                scanf ("%d", &choice);
                                if (choice != 1 || choice != 2) {
                                        goto read_choice;
                                }
                                
                                
                                break;
                        case 2:
                                break;
                        case 3:
                                break;
                        case 4:
                                ret = 0;
                                goto out;
                                break;
                }
        }
out:
        return ret;
}