## 1 类型系统和注册流程
&emsp;&emsp;GObject是一个基类。我们通常不使用GObject本身。因为GObject非常简单，在大多数情况下不足以单独使用。相反，我们使用GObject的子类对象，例如各种GtkWidget。可以说，这种可继承是GObject最重要的特性。本节介绍如何定义GObject的子对象。

## 2 命令惯例
&emsp;&emsp;本节的一个例子是一个对象表示一个实数。它不是很有用，因为我们已经在C语言中使用了双类型来表示实数。但是，我认为这个例子对于了解定义子对象的技术来说并不是那么糟糕。
&emsp;&emsp;首先，您需要了解命名约定。对象名称由名称空间和名称组成。例如，“GObject”由名称空间“G”和名称“Object”组成。“GtkWidget”由名称空间“Gtk”和名称“Widget”组成。设新对象的名称空间为“T”，名称为“Double”。在本教程中，我们使用“T”作为我们所创建的所有对象的名称空间。
TDouble是对象名称。它是GObject的子对象。它表示一个实数，该数的类型为double。它有一些有用的功能。
## 3 定义TDoubleClass和TDouble
&emsp;&emsp;当我们说“类型”时，它可以是类型系统中的类型或C语言类型。例如，GObject是类型系统中的类型名称。char, int或double是C语言类型。当“类型”这个词在上下文中的意思很清楚时，我们就叫它“类型”。但如果它是模棱两可的，我们称之为“C类型”或“类型系统中的类型”。
&emsp;&emsp;TDouble对象具有类和实例。类的C类型是TDoubleClass。其结构是这样的:

```c
typedef struct _TDoubleClass TDoubleClass
struct _TDoubleClass {
  GObjectClass parent_class;
}
```
&emsp;&emsp;TDoubleClass是一个C结构标签名，TDoubleClass是“struct TDoubleClass”。TDoubleClass是一个新创建的C类型。

 - 使用typedef定义类类型。
 - 结构体的第一个成员必须是父类结构。

TDoubleClass不需要自己的成员。
TDouble实例的C类型为TDouble。

```c
typedef struct _TDouble TDouble
struct _TDouble {
  GObject parent;
  double value;
}
```
这类似于类的结构。

 - 使用typedef定义实例类型。
 - 结构的第一个成员必须是父实例结构。

TDouble有自己的成员value。它是TDouble实例的值。上面的编码惯例需要一直保持。
## 4 GObject子对象的创建过程
TDouble类型的创建过程与GObject类似。

 1. 用类型系统注册TDouble类型。
 2. 类型系统为TDoubleClass和TDouble分配内存。
 3. 初始化TDoubleClass。
 4. 初始化TDouble。
 
 ## 5 注册
 &emsp;&emsp;通常注册是通过宏完成的，比如G DECLARE FINAL TYPE和G DEFINE TYPE。所以你不需要关心注册。但是，在本教程中，理解GObject类型系统很重要，所以我想首先向您展示不带宏的注册。
 &emsp;&emsp;有两种类型，静态和动态。静态类型不会销毁它的类，即使所有实例都已销毁。动态类型在销毁最后一个实例时销毁其类。GObject的类型是静态的，它的子类对象的类型也是静态的。函数g_type_register_static注册一个静态对象的类型。下面的代码是从Glib源文件中的gtype.h中提取的。
 
```c
GType
g_type_register_static (GType           parent_type,
                        const gchar     *type_name,
                        const GTypeInfo *info,
                        GTypeFlags      flags);
```
上述参数包括:
parent type:父类型。
type name:类型的名称。例如，“TDouble”。
info:类型信息。GTypeInfo结构将在下面解释。GTypeFlags:flag。如果类型是抽象类型或抽象值类型，则设置它们的标志。否则，将其设置为零。
&emsp;&emsp;因为类型系统维持类型的父子类型关系，g_type_refister_static有一个父类型参数。类型系统还保存类型信息。注册后，g_type_refister_static返回新对象的类型。GTypeInfo结构的定义如下：

```c
typedef struct _GTypeInfo  GTypeInfo;

struct _GTypeInfo
{
  /* interface types, classed types, instantiated types */
  guint16                class_size;

  GBaseInitFunc          base_init;
  GBaseFinalizeFunc      base_finalize;

  /* interface types, classed types, instantiated types */
  GClassInitFunc         class_init;
  GClassFinalizeFunc     class_finalize;
  gconstpointer          class_data;

  /* instantiated types */
  guint16                instance_size;
  guint16                n_preallocs;
  GInstanceInitFunc      instance_init;

  /* value handling */
  const GTypeValueTable  *value_table;
};
```
这个结构需要在注册之前创建。

 - class size：类的大小。例如，TDouble的类大小是sizeof (TDoubleClass)。
 - base init, base finalize:这些函数initialize/finalize类的动态成员。在许多情况下，它们不是必需的，并被赋值为NULL。更多信息，请参见GObject API参考—— GObject API Reference -- BaseInitFunc and GObject API Reference -- ClassInitFunc.。
 - class init:初始化类的静态成员。将类初始化函数赋给class init。按照约定，名称为<name space>_<name>_class_init，例如：t_double_class_init。
 - value_table：这通常只对基本类型有用。如果类型是GObject的子类，则赋值NULL。
 
 &emsp;&emsp;这些信息由类型系统保存，并在对象创建或销毁时使用。Class_size和instance_size用于为类和实例分配内存。Class_init和instance_init函数在类或实例初始化时被调用。
 
 [Example3.c](./example3.c)展示了如何使用g_type_register_static。
 

```c
 1 #include <glib-object.h>
 2 
 3 #define T_TYPE_DOUBLE  (t_double_get_type ())
 4 
 5 typedef struct _TDouble TDouble;
 6 struct _TDouble {
 7   GObject parent;
 8   double value;
 9 };
10 
11 typedef struct _TDoubleClass TDoubleClass;
12 struct _TDoubleClass {
13   GObjectClass parent_class;
14 };
15 
16 static void
17 t_double_class_init (TDoubleClass *class) {
18 }
19 
20 static void
21 t_double_init (TDouble *d) {
22 }
23 
24 GType
25 t_double_get_type (void) {
26   static GType type = 0;
27   GTypeInfo info;
28 
29   if (type == 0) {
30     info.class_size = sizeof (TDoubleClass);
31     info.base_init = NULL;
32     info.base_finalize = NULL;
33     info.class_init = (GClassInitFunc)  t_double_class_init;
34     info.class_finalize = NULL;
35     info.class_data = NULL;
36     info.instance_size = sizeof (TDouble);
37     info.n_preallocs = 0;
38     info.instance_init = (GInstanceInitFunc)  t_double_init;
39     info.value_table = NULL;
40     type = g_type_register_static (G_TYPE_OBJECT, "TDouble", &info, 0);
41   }
42   return type;
43 }
44 
45 int
46 main (int argc, char **argv) {
47   GType dtype;
48   TDouble *d;
49 
50   dtype = t_double_get_type (); /* or d = T_TYPE_DOUBLE */
51   if (dtype)
52     g_print ("Registration was a success. The type is %lx.\n", dtype);
53   else
54     g_print ("Registration failed.\n");
55 
56   d = g_object_new (T_TYPE_DOUBLE, NULL);
57   if (d)
58     g_print ("Instantiation was a success. The instance address is %p.\n", d);
59   else
60     g_print ("Instantiation failed.\n");
61 
62   return 0;
63 }
64 
```

 - 16-22:类初始化函数和实例初始化函数。它们在这里什么都不做，但它们是注册所必需的。
 - 24-43: t_double_get_type函数。该函数返回TDouble对象的类型。函数的名称总是`<name space>_get_type`。宏`<NAME_SPACE>_TYPE_<NAME>`(所有字符都是大写)将被此函数替换。看第3行。`T_TYPE_DOUBLE`是一个由t_double_get_type()取代的宏。这个函数有一个静态变量类型（static GType）来保存对象的类型。在第一次调用此函数时，type为0。然后调用g_type_register_static将对象注册到类型系统。在第二次或后续调用时，函数只返回type，因为静态变量type已被g_type_register_static赋值为非零值，并且它保留该值。
 - 30-40:设置info结构并调用g_type_register_static。
 - 45-63:主要功能。获取TDouble对象的类型并显示它。G_object_new用于实例化对象。显示实例的地址。

example3.c在src/misc目录中。
执行结果：

```sh
$ cd misc; _build/example3
Registration was a success. The type is 557292894830.
Instantiation was a success. The instance address is 0x557292897000.
```

## 6 G_DEFINE_TYPE 宏
上面的注册总是有相同的算法完成的。因此，它可以被定义为一个宏，如G_DEFINE_TYPE。

```c
static void t_double_init (TDouble *self); 
static void t_double_class_init (TDoubleClass *klass); 
static GType t_double_get_type_once (void); 
static gpointer t_double_parent_class =  ((void *)0); 
static gint TDouble_private_offset; 

static void 
t_double_class_intern_init (gpointer klass) { 
  t_double_parent_class = g_type_class_peek_parent (klass); 
  if (TDouble_private_offset != 0) 
    g_type_class_adjust_private_offset (klass, &TDouble_private_offset); 
  t_double_class_init ((TDoubleClass*) klass); 
}

__attribute__((__unused__)) static inline gpointer 
t_double_get_instance_private (TDouble *self) {
  return (((gpointer) ((guint8*) (self) + (glong) (TDouble_private_offset)))); 
}

/**
 * 主要是展开该函数
 * */ 
GType 
t_double_get_type (void) {
  static volatile gsize g_define_type_id__volatile = 0; 
  if ((__extension__ ({ 
    _Static_assert (sizeof *(&g_define_type_id__volatile) == sizeof (gpointer), "Expression evaluates to false"); 
    (void) (0 ? (gpointer) *(&g_define_type_id__volatile) : ((void *)0)); 
    (!(__extension__ ({ 
      _Static_assert (sizeof *(&g_define_type_id__volatile) == sizeof (gpointer), "Expression evaluates to false"); 
      __typeof__(*(&g_define_type_id__volatile)) gapg_temp_newval; 
      __typeof__((&g_define_type_id__volatile)) gapg_temp_atomic = (&g_define_type_id__volatile); 
      __atomic_load (gapg_temp_atomic, &gapg_temp_newval, 5); 
      gapg_temp_newval; 
      })) && g_once_init_enter (&g_define_type_id__volatile)); 
    }))) {
    
    GType g_define_type_id = t_double_get_type_once (); 
  
    (__extension__ ({ 
      _Static_assert (sizeof *(&g_define_type_id__volatile) == sizeof (gpointer), "Expression evaluates to false"); 
      0 ? (void) (*(&g_define_type_id__volatile) = (g_define_type_id)) : (void) 0; 
      g_once_init_leave ((&g_define_type_id__volatile), (gsize) (g_define_type_id)); 
      })
    );
  }
  
  return g_define_type_id__volatile; 
}
  
__attribute__((noinline)) static GType 
t_double_get_type_once (void) { 
  GType g_define_type_id = g_type_register_static_simple (((GType) ((20) << (2))), 
                                                          g_intern_static_string ("TDouble"), 
                                                          sizeof (TDoubleClass), (
                                                          GClassInitFunc)(void (*)(void)) t_double_class_intern_init, 
                                                          sizeof (TDouble), 
                                                          (GInstanceInitFunc)(void (*)(void)) t_double_init, 
                                                          (GTypeFlags) 0); 
  
  { {{};} } 
  
  return g_define_type_id; 
}

```

G_DEFINE_TYPE的作用如下:

 - **声明**一个类初始化函数。它的名称是` <name space>_<name>_class_init`。例如，如果对象名是TDouble，它就是t_double_class_init。这是一个声明，不是定义。你需要定义它。
 - **声明**一个实例初始化函数。其名称为`<name space>_<name>_init`。例如，如果对象名称为TDouble，则它为t_double_init。这是声明，不是定义。你需要定义它。
 - **定义**一个指向父类的静态变量。它的名字是`<name space>_<name>_parent_class`。例如，如果对象名称是TDouble，它就是t_double_parent_class。
 - **定义**一个`<name space>_<name>_get_type ()`函数。例如，如果对象名称是TDouble，则它是t_double_get_type。注册是在这个函数中完成的，就像前面的小节一样。

使用这个宏可以减少程序的行数。请参阅下面的示例example4.c，其工作原理与example3.c相同。

```c
 1 #include <glib-object.h>
 2 
 3 #define T_TYPE_DOUBLE  (t_double_get_type ())
 4 
 5 typedef struct _TDouble TDouble;
 6 struct _TDouble {
 7   GObject parent;
 8   double value;
 9 };
10 
11 typedef struct _TDoubleClass TDoubleClass;
12 struct _TDoubleClass {
13   GObjectClass parent_class;
14 };
15 
16 G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)
17 
18 static void
19 t_double_class_init (TDoubleClass *class) {
20 }
21 
22 static void
23 t_double_init (TDouble *d) {
24 }
25 
26 int
27 main (int argc, char **argv) {
28   GType dtype;
29   TDouble *d;
30 
31   dtype = t_double_get_type (); /* or d = T_TYPE_DOUBLE */
32   if (dtype)
33     g_print ("Registration was a success. The type is %lx.\n", dtype);
34   else
35     g_print ("Registration failed.\n");
36 
37   d = g_object_new (T_TYPE_DOUBLE, NULL);
38   if (d)
39     g_print ("Instantiation was a success. The instance address is %p.\n", d);
40   else
41     g_print ("Instantiation failed.\n");
42 
43   return 0;
44 }
45 
```
多亏了G_DEFINE_TYPE，我们从编写GTypeInfo和g_type_register_static这样麻烦的代码中解放出来。需要注意的一件重要的事情是遵循init函数的命名约定。
执行结果：

```bash
Registration was a success. The type is 55d4a6d20830.
Instantiation was a success. The instance address is 0x55d4a6d22000.
```

## 7 G_DECLARE_FINAL_TYPE 宏

&emsp;&emsp;另一个有用的宏是G_DECLARE_FINAL_TYPE宏。这个宏可以用于最后一种类型（这个类不会再被继承）。最后一种类型是没有任何子类。如果一个类型有子类型，它就是可派生类型。如果你想定义一个可派生类型对象，请使用g_declare_derivatives able_type。然而，在大多数情况下，您可能会编写final类型对象（不会被继承的对象）。

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

G_DECLARE_FINAL_TYPE执行以下操作:

 - **声明**`<name space>_<name>_get_type ()`函数。这只是声明。你需要定义它。但是你可以使用`G_DEFINE_TYPE`宏，它的扩展包含了函数的定义。实际上，你不需要自己写定义。
 - **定义**`typedef struct <_NAME SPACE><NAME>`。例如，如果对象名称为TDouble，则`typedef struct _TDouble TDouble`将包含在展开中。但是你需要在G_DEFINE_TYPE之前**自己定义**struct _TDouble结构体。
 - **定义**了`<NAME SPACE>_<NAME> `宏。例如，如果对象是TDouble，宏就是T_DOUBLE。它将展开为一个函数，将实参转换为指向对象的指针。例如，T_Double (obj)将obj类型强制转换为TDouble *。
 - **定义**了`<NAME SPACE>_IS_<NAME>`宏。例如，如果对象是TDouble，宏就是T_IS_DOUBLE。它将扩展为一个函数，该函数检查参数是否指向TDouble的实例。如果参数指向TDouble的后代，则返回true。
 - **定义**了类结构体。一个final类型对象不需要有自己的类结构体成员。定义类似于example4.c中的第11至14行。

你需要在`G_DECLARE_FINAL_TYPE`之前编写该对象类型的宏定义。例如，如果对象是TDouble，那么

```c
#define T_TYPE_DOUBLE  (t_double_get_type ())
```
需要在`G_DECLARE_FINAL_TYPE`之前定义。

example5.c使用这个宏。它像example3.c或example4.c一样工作。

```c
 1 #include <glib-object.h>
 2 
 3 #define T_TYPE_DOUBLE  (t_double_get_type ())
 4 G_DECLARE_FINAL_TYPE (TDouble, t_double, T, DOUBLE, GObject)
 5 
 6 struct _TDouble {
 7   GObject parent;
 8   double value;
 9 };
10 
11 G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)
12 
13 static void
14 t_double_class_init (TDoubleClass *class) {
15 }
16 
17 static void
18 t_double_init (TDouble *d) {
19 }
20 
21 int
22 main (int argc, char **argv) {
23   GType dtype;
24   TDouble *d;
25 
26   dtype = t_double_get_type (); /* or d = T_TYPE_DOUBLE */
27   if (dtype)
28     g_print ("Registration was a success. The type is %lx.\n", dtype);
29   else
30     g_print ("Registration failed.\n");
31 
32   d = g_object_new (T_TYPE_DOUBLE, NULL);
33   if (d)
34     g_print ("Instantiation was a success. The instance address is %p.\n", d);
35   else
36     g_print ("Instantiation failed.\n");
37 
38   if (T_IS_DOUBLE (d))
39     g_print ("d is TDouble instance.\n");
40   else
41     g_print ("d is not TDouble instance.\n");
42 
43   if (G_IS_OBJECT (d))
44     g_print ("d is GObject instance.\n");
45   else
46     g_print ("d is not GObject instance.\n");
47 
48   return 0;
49 }
50 
```

> $ cd misc; _build/example5
Registration was a success. The type is 558415d17830.
Instantiation was a success. The instance address is 0x558415d19000.
d is TDouble instance.
d is GObject instance.

##8 分文件编写main.c, tdouble.h和tdouble.c
现在是时候将内容分离为三个文件:main.c、tdouble.h和tdouble.c。对象由两个文件定义，头文件和C源文件。

```c
/* filename: tdouble.h */
 1 #ifndef __T_DOUBLE_H__
 2 #define __T_DOUBLE_H__
 3 
 4 #include <glib-object.h>
 5 
 6 #define T_TYPE_DOUBLE  (t_double_get_type ())
 7 G_DECLARE_FINAL_TYPE (TDouble, t_double, T, DOUBLE, GObject)
 8 
 9 gboolean
10 t_double_get_value (TDouble *d, double *value);
11 
12 void
13 t_double_set_value (TDouble *d, double value);
14 
15 TDouble *
16 t_double_new (double value);
17 
18 #endif /* __T_DOUBLE_H__ */
19 
```

 - 头文件的内容是公共的，即它对任何文件都是开放的。头文件包括宏，宏提供类型信息、强制转换和类型检查以及公共函数。
 - 1,2,18:这些指令阻止编译器读取头文件两次或更多次。如果你的编译器支持#pragma once指令，你可以用它代替。它没有正式定义，但在许多编译器中得到广泛支持。
 - 6,7: `T_TYPE_DOUBLE`是公共的。`G_DECLARE_FINAL_TYPE`也扩展为公共定义。
 - 9-13:函数声明。它们是对象值的getter和setter方法。
 - 15-16:对象实例化函数。

```c
/* filename: tdouble.c */
 1 #include "tdouble.h"
 2 
 3 struct _TDouble {
 4   GObject parent;
 5   double value;
 6 };
 7 
 8 G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)
 9 
10 static void
11 t_double_class_init (TDoubleClass *class) {
12 }
13 
14 static void
15 t_double_init (TDouble *d) {
16 }
17 
18 gboolean
19 t_double_get_value (TDouble *d, double *value) {
20   g_return_val_if_fail (T_IS_DOUBLE (d), FALSE);
21 
22   *value = d->value;
23   return TRUE;
24 }
25 
26 void
27 t_double_set_value (TDouble *d, double value) {
28   g_return_if_fail (T_IS_DOUBLE (d));
29 
30   d->value = value;
31 }
32 
33 TDouble *
34 t_double_new (double value) {
35   TDouble *d;
36 
37   d = g_object_new (T_TYPE_DOUBLE, NULL);
38   d->value = value;
39   return d;
40 }
41 
```

 - 3-6:实例结构的声明。由于G_DECLARE_FINAL_TYPE宏发出类型器结构体_TDouble TDouble，结构的标记名必须为_TDouble。
 - 8: G_DEFINE_TYPE宏。
 - 10-16:类和实例初始化函数。目前，他们什么都不做。
 - 18 - 24: Getter。实参值是指向double类型变量的指针。将对象值(d->值)赋给变量。如果成功，则返回TRUE。G_return_val_if_fail用于检查参数类型。如果参数d不是TDouble类型，它将错误输出到日志并立即返回FALSE。此函数用于报告程序员的错误。您不应该将它用于运行时错误。更多信息请参见Glib API参考—错误报告。G_return_val_if_fail不在静态类函数中使用，它们是私有的，因为静态函数只能从同一文件中的函数调用。这样的函数很清楚参数类型。g_return_val_if_fail用于公共函数。
 - 26-31: Setter。G_return_if_fail函数用于检查参数类型。这个函数不返回任何值。因为t_double_set_value的类型是void，所以不会返回任何值。因此，我们使用g_return_if_fail而不是g_return_val_if_fail。
 - 33-40:对象实例化函数。它有一个参数值来设置对象的值。
 - 37:该函数使用g_object_new实例化对象。参数T_TYPE_DOUBLE扩展为函数t_double_get_type()。如果这是对t_double_get_type的第一次调用，则将执行类型注册。

```c
/* filename: main.c */
 1 #include <glib-object.h>
 2 #include "tdouble.h"
 3 
 4 int
 5 main (int argc, char **argv) {
 6   TDouble *d;
 7   double value;
 8 
 9   d = t_double_new (10.0);
10   if (t_double_get_value (d, &value))
11     g_print ("t_double_get_value succesfully assigned %lf to value.\n", value);
12   else
13     g_print ("t_double_get_value failed.\n");
14 
15   t_double_set_value (d, -20.0);
16   g_print ("Now, set d (tDouble object) with %lf.\n", -20.0);
17   if (t_double_get_value (d, &value))
18     g_print ("t_double_get_value succesfully assigned %lf to value.\n", value);
19   else
20     g_print ("t_double_get_value failed.\n");
21 
22   return 0;
23 }
24 
```
 - 2:包含tdouble.h。这对于访问TDouble对象是必要的。
- 9:实例化TDouble对象，并设置d指向对象。
- 10-13:测试对象的getter。
- 15-20:测试对象的setter。
源文件位于src/ tdoube1中。将当前目录更改为上面的目录并输入以下内容。

```bash
meson _build
ninja -C _build
```

> $ cd tdouble1; _build/example6
t_double_get_value succesfully assigned 10.000000 to value.
Now, set d (tDouble object) with -20.000000.
t_double_get_value succesfully assigned -20.000000 to value.

这个例子非常简单。但是任何对象都有头文件和C源文件，就像上面的例子一样。他们遵循惯例。你可能知道会议的重要性。欲了解更多信息，请参阅GObject API参考—约定。
##9 函数
对象的函数对其他对象开放。它们就像面向对象语言中的公共方法。

向TDouble对象添加计算操作符是很自然的，因为它们表示实数。例如，t_double_add将实例的值与另一个实例相加。然后它创建一个新的TDouble实例，该实例的值是这些值的和。
```c
TDouble *
t_double_add (TDouble *self, TDouble *other) {
  g_return_val_if_fail (T_IS_DOUBLE (self), NULL);
  g_return_val_if_fail (T_IS_DOUBLE (other), NULL);
  double value;

  if (! t_double_get_value (other, &value))
    return NULL;
  return t_double_new (self->value + value);
}
```
self是函数所属的实例。other是另一个TDouble实例。

self的值可以通过self->值访问，但不能使用other->值获取other的值。使用函数t_double_get_value代替。因为self是来自于other的一个实例。一般来说，对象的结构对其他对象是不开放的。当一个对象A访问另一个对象B时，A必须使用B提供的公共函数。
##10 练习
编写TDouble对象的减、乘、除和符号改变(一元减)函数。比较你的程序与src/tdouble2目录下的tdouble.c。

 [翻译自GObject tutorial](https://github.com/ToshioCP/Gobject-tutorial/blob/main/gfm/sec3.md)
