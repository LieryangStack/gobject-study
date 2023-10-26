#include "pm-dlist.h"
  
int
main (void) {
        PMDList *list;

        list = g_object_new (PM_TYPE_DLIST, NULL);

        g_object_unref (list);
        
        return 0;
}