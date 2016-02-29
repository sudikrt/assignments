#include "list.h"

int main () {
        
        int choice = 0;
        int i = 0;
        int ret = -1;
        char *ch;
        char *str;
        char *str1 ;
        str = (char*) calloc (1, 1024);
        str = "Hello";
        
        list_t *list_obj = list_new ();
        
        ch = (char*) calloc (1, 1024);
        str1 = (char*) calloc (1, 1024);
   
        if (list_obj == NULL) {
                ret = -1;
                goto out;
        }
        do {
                printf ("1. insert 2. Delete\n");
                scanf ("%d", &choice) ;
                switch (choice) {
                        case 1:
                                if (i % 2 == 0) {
                                        list_obj -> first_node = insert_into_list (list_obj, ch);
                                } else {
                                        list_obj -> first_node = insert_into_list (list_obj, str1);
                                }
                                break;
                        case 2:
                                delete_from_list (list_obj, str1);
                                break;
                        case 3:
                                printf ("No of items is %d\n", list_obj -> max);
                                disp (list_obj);
                                break;
                        case 4:
                                list_free (list_obj);
                                printf ("Success \n");
                                goto out;
                                break;
                }
                i++;
        }while (choice < 5);
        ret = 0;
out:
        return ret;
}
