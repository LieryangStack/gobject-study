#include "pm-dlist.h"
  
int
main (void)
{
        /* GObject 库的类型管理系统的初始化 */
        g_type_init ();
     
        PMDList *list;
 
        list = g_object_new (PM_TYPE_DLIST, NULL);

        g_object_unref (list);
          
        return 0;
}