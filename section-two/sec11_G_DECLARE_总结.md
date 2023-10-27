# 关于G_DECLARE_XXX宏相关总结

## 1 G_DECLARE_FINAL_TYPE

<span style="color: red;">一个final类型对象不需要有自己的类结构体成员</span>

G_DECLARE_FINAL_TYPE执行以下主要操作:

 - **定义** `typedef struct _TDouble TDouble;`。

 - **定义** 类结构体。一个final类型对象不需要有自己的类结构体成员。

    ```c
    typedef struct { 
      GObjectClass parent_class; 
    } TDoubleClass; 
    ```
 - **定义** `T_DOUBLE` 宏。例如，如果对象是TDouble，宏就是T_DOUBLE。它将展开为一个函数，将实参转换为指向对象的指针。例如，T_Double (obj)将obj类型强制转换为TDouble *。
 - **定义** `T_IS_DOUBLE` 宏。例如，如果对象是TDouble，宏就是T_IS_DOUBLE。它将扩展为一个函数，该函数检查参数是否指向TDouble的实例。如果参数指向TDouble的后代，则返回true。


## 2 G_DECLARE_DERIVABLE_TYPE


<span style="color: red;">一个可派生类型实例对象仅有父实例</span>

G_DECLARE_DERIVABLE_TYPE执行以下主要操作:

 - **定义** `typedef struct _TDoubleClass TDoubleClass;`。

 - **定义** 实例结构体。其成员仅为其父实例。

    ```c
    typedef struct _TDouble TDouble; 
    struct _TDouble { GObject parent_instance; }; 
    ```
 - **定义** `T_DOUBLE` 宏。

 - **定义** `T_IS_DOUBLE` 宏。

 - **定义** `T_DOUBLE_CLASS` 宏。

 - **定义** `T_IS_DOUBLE_CLASS` 宏。

 - **定义** `T_DOUBLE_GET_CLASS` 宏。


## 3 G_DECLARE_INTERFACE

## 补充：宏展开代码

```c

/******************************G_DECLARE_FINAL_TYPE (TDouble, t_double, T, DOUBLE, GObject)宏展开********START********************************/

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic push
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

typedef struct _TDouble TDouble; 
typedef struct { 
  GObjectClass parent_class; 
} TDoubleClass; 

typedef TDouble *TDouble_autoptr; 
typedef GList *TDouble_listautoptr; 
typedef GSList *TDouble_slistautoptr; 
typedef GQueue *TDouble_queueautoptr;

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic push
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
static __attribute__((__unused__)) inline void 
glib_autoptr_clear_TDouble (TDouble *_ptr) { 
  if (_ptr) (glib_autoptr_clear_GObject) ((GObject *) _ptr); 
  } 

static __attribute__((__unused__)) inline void 
glib_autoptr_cleanup_TDouble (TDouble **_ptr) { 
  glib_autoptr_clear_TDouble (*_ptr); 
} 

static __attribute__((__unused__)) inline void 
glib_listautoptr_cleanup_TDouble (GList **_l) { 
  g_list_free_full (*_l, (GDestroyNotify) (void(*)(void)) glib_autoptr_clear_GObject); 
} 

static __attribute__((__unused__)) inline void 
glib_slistautoptr_cleanup_TDouble (GSList **_l) { 
  g_slist_free_full (*_l, (GDestroyNotify) (void(*)(void)) glib_autoptr_clear_GObject); 
} 

static __attribute__((__unused__)) inline void 
glib_queueautoptr_cleanup_TDouble (GQueue **_q) { 
  if (*_q) g_queue_free_full (*_q, (GDestroyNotify) (void(*)(void)) glib_autoptr_clear_GObject); 
}
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic pop
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

typedef TDoubleClass *TDoubleClass_autoptr; 
typedef GList *TDoubleClass_listautoptr; 
typedef GSList *TDoubleClass_slistautoptr; 
typedef GQueue *TDoubleClass_queueautoptr;

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic push
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
static __attribute__((__unused__)) inline void 
glib_autoptr_clear_TDoubleClass (TDoubleClass *_ptr) { 
  if (_ptr) (g_type_class_unref) ((TDoubleClass *) _ptr); 
} 

static __attribute__((__unused__)) inline void 
glib_autoptr_cleanup_TDoubleClass (TDoubleClass **_ptr) { 
  glib_autoptr_clear_TDoubleClass (*_ptr); 
} 

static __attribute__((__unused__)) inline void 
glib_listautoptr_cleanup_TDoubleClass (GList **_l) { 
  g_list_free_full (*_l, (GDestroyNotify) (void(*)(void)) g_type_class_unref); 
} 

static __attribute__((__unused__)) inline void 
glib_slistautoptr_cleanup_TDoubleClass (GSList **_l) { 
  g_slist_free_full (*_l, (GDestroyNotify) (void(*)(void)) g_type_class_unref); 
} 

static __attribute__((__unused__)) inline void 
glib_queueautoptr_cleanup_TDoubleClass (GQueue **_q) { 
  if (*_q) 
    g_queue_free_full (*_q, (GDestroyNotify) (void(*)(void)) g_type_class_unref); 
}

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic pop
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
__attribute__((__unused__)) static inline TDouble * 
T_DOUBLE (gpointer ptr) { 
  return (((TDouble*) g_type_check_instance_cast ((GTypeInstance*) (ptr), (t_double_get_type ())))); 
} 
__attribute__((__unused__)) static inline gboolean 
T_IS_DOUBLE (gpointer ptr) { 
  return ((__extension__ ({
           GTypeInstance *__inst = (GTypeInstance*) (ptr); 
           GType __t = (t_double_get_type ()); 
           gboolean __r; 
           if (!__inst) __r = (0); 
           else if (__inst->g_class && __inst->g_class->g_type == __t) __r = (!(0)); 
           else __r = g_type_check_instance_is_a (__inst, __t); __r; }))); 
}
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic pop
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

/******************************G_DECLARE_FINAL_TYPE (TDouble, t_double, T, DOUBLE, GObject)宏展开********END********************************/

```

```c
/******************************G_DECLARE_DERIVABLE_TYPE (TDouble, t_double, T, DOUBLE, GObject)宏展开********START********************************/

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic push
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

typedef struct _TDouble TDouble; 
typedef struct _TDoubleClass TDoubleClass; 
struct _TDouble { GObject parent_instance; }; 
typedef TDouble *TDouble_autoptr; 
typedef GList *TDouble_listautoptr; 
typedef GSList *TDouble_slistautoptr; 
typedef GQueue *TDouble_queueautoptr;

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic push
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

static __attribute__ ((__unused__)) inline void 
glib_autoptr_clear_TDouble (TDouble *_ptr) { 
  if (_ptr) (glib_autoptr_clear_GObject) ((GObject *) _ptr); 
} 

static __attribute__ ((__unused__)) inline void 
glib_autoptr_cleanup_TDouble (TDouble **_ptr) { 
  glib_autoptr_clear_TDouble (*_ptr); 
} 

static __attribute__ ((__unused__)) inline void 
glib_listautoptr_cleanup_TDouble (GList **_l) { 
  g_list_free_full (*_l, (GDestroyNotify) (void(*)(void)) glib_autoptr_clear_GObject); 
}

static __attribute__ ((__unused__)) inline void 
glib_slistautoptr_cleanup_TDouble (GSList **_l) { 
  g_slist_free_full (*_l, (GDestroyNotify) (void(*)(void)) glib_autoptr_clear_GObject); 
} 

static __attribute__ ((__unused__)) inline void 
glib_queueautoptr_cleanup_TDouble (GQueue **_q) { 
  if (*_q) g_queue_free_full (*_q, (GDestroyNotify) (void(*)(void)) glib_autoptr_clear_GObject); 
}

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic pop
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

typedef TDoubleClass *TDoubleClass_autoptr; 
typedef GList *TDoubleClass_listautoptr; 
typedef GSList *TDoubleClass_slistautoptr; 
typedef GQueue *TDoubleClass_queueautoptr;

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic push
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

static __attribute__ ((__unused__)) inline void 
glib_autoptr_clear_TDoubleClass (TDoubleClass *_ptr) { 
  if (_ptr) (g_type_class_unref) ((TDoubleClass *) _ptr); 
}

static __attribute__ ((__unused__)) inline void 
glib_autoptr_cleanup_TDoubleClass (TDoubleClass **_ptr) { 
  glib_autoptr_clear_TDoubleClass (*_ptr); 
}

static __attribute__ ((__unused__)) inline void 
glib_listautoptr_cleanup_TDoubleClass (GList **_l) { 
  g_list_free_full (*_l, (GDestroyNotify) (void(*)(void)) g_type_class_unref); 
}

static __attribute__ ((__unused__)) inline void 
glib_slistautoptr_cleanup_TDoubleClass (GSList **_l) { 
  g_slist_free_full (*_l, (GDestroyNotify) (void(*)(void)) g_type_class_unref);
} 

static __attribute__ ((__unused__)) inline void 
glib_queueautoptr_cleanup_TDoubleClass (GQueue **_q) { 
  if (*_q) g_queue_free_full (*_q, (GDestroyNotify) (void(*)(void)) g_type_class_unref); 
}

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic pop
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

__attribute__ ((__unused__)) static inline TDouble * 
T_DOUBLE (gpointer ptr) { 
  return (((TDouble*) (void *) g_type_check_instance_cast ((GTypeInstance*) (ptr), (t_double_get_type ())))); 
} 

__attribute__ ((__unused__)) static inline TDoubleClass *
T_DOUBLE_CLASS (gpointer ptr) { 
  return (((TDoubleClass*) (void *) g_type_check_class_cast ((GTypeClass*) (ptr), (t_double_get_type ())))); 
} 

__attribute__ ((__unused__)) static inline gboolean 
T_IS_DOUBLE (gpointer ptr) {
  return ((__extension__ ({ GTypeInstance *__inst = (GTypeInstance*) (ptr); 
                            GType __t = (t_double_get_type ()); 
                            gboolean __r; 
                            if (!__inst) __r = (0); 
                            else if (__inst->g_class && __inst->g_class->g_type == __t) __r = (!(0)); 
                            else __r = g_type_check_instance_is_a (__inst, __t); __r; }))); 
} 

__attribute__ ((__unused__)) static inline gboolean 
T_IS_DOUBLE_CLASS (gpointer ptr) { 
  return ((__extension__ ({ GTypeClass *__class = (GTypeClass*) (ptr); 
                            GType __t = (t_double_get_type ()); 
                            gboolean __r; 
                            if (!__class) __r = (0);
                            else if (__class->g_type == __t) __r = (!(0)); 
                            else __r = g_type_check_class_is_a (__class, __t); __r; })));
} 

__attribute__ ((__unused__)) static inline TDoubleClass * 
T_DOUBLE_GET_CLASS (gpointer ptr) { 
  return (((TDoubleClass*) (((GTypeInstance*) (ptr))->g_class))); 
}

# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"
#pragma GCC diagnostic pop
# 4 "/home/lieryang/Desktop/gobject-study/section-two/sec3/example5.c"

/******************************G_DECLARE_DERIVABLE_TYPE (TDouble, t_double, T, DOUBLE, GObject)宏展开********END********************************/
```