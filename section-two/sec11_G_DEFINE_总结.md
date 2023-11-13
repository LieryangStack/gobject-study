# 关于G_DEFINE_XXX宏相关总结

## 1 普通对象

### 1.1 G_DEFINE_TYPE

1. **声明**实例初始化函数;

2. **声明**类初始化函数;

3. **定义**t_double_get_type函数（t_double_class_intern_init、t_double_get_type_once函数都是被t_double_get_type函数调用，所有可以把他们合到一起）

4. **定义**t_double_get_instance_private函数（使用G_DEFINE_TYPE宏，_get_instance_private该函数内部的TDouble_private_offset变量并没有被赋值）
   
   只有在G_DEFINE_TYPE_WITH_PRIVATE宏时候使用。

### 1.2 G_DEFINE_TYPE_WITH_CODE

&emsp;&emsp;两个声明和两个定义都和 **G_DEFINE_TYPE** 宏一样

<span style="color: red;">&emsp;&emsp;唯一不一样的地方就是，t_double_get_type 调用的 t_double_get_type_once 内部可以插入用户自定义要执行的代码。</span>

### 1.3 G_DEFINE_TYPE_WITH_PRIVATE

&emsp;&emsp;两个声明和两个定义都和 **G_DEFINE_TYPE** 宏一样

<span style="color: red;">&emsp;&emsp;唯一不一样的地方就是，**t_double_get_type**调用的 **t_double_get_type_once** 内部插入了 **G_ADD_PRIVATE(TDouble)** 宏</span>，该宏展开代码如下：

```c
TDouble_private_offset = g_type_add_instance_private (g_define_type_id, sizeof (TDoublePrivate)); 
```

&emsp;&emsp;其实跟 **G_DEFINE_TYPE_WITH_CODE** 一样，只不过 **_WITH_CODE**宏是用户自己插入代码，而 **_WITH_PRIVATE** 宏是插入 **G_ADD_PRIVATE(TDouble)**

## 2 抽象对象

普通对象定义讲明白之后，抽象对象定义也很容易理解。

<span style="color: red;">抽象对象不能创建实例</span>，展开抽象对象定义宏后，与 G_DEFINE_TYPE 唯一不同的地方就是 t_double_get_type_once 函数中的 GTypeFlags变量的参数是 **G_TYPE_FLAG_ABSTRACT**。

```c
typedef enum    /*< skip >*/
{
  G_TYPE_FLAG_NONE GOBJECT_AVAILABLE_ENUMERATOR_IN_2_74 = 0,
  G_TYPE_FLAG_ABSTRACT = (1 << 4),
  G_TYPE_FLAG_VALUE_ABSTRACT = (1 << 5),
  G_TYPE_FLAG_FINAL GOBJECT_AVAILABLE_ENUMERATOR_IN_2_70 = (1 << 6),
  G_TYPE_FLAG_DEPRECATED GOBJECT_AVAILABLE_ENUMERATOR_IN_2_76 = (1 << 7)
} GTypeFlags;
```

例如：抽象对象一般作为一个基类（比如实现 整形数字对象、小数数学对象，可以把数字作为一个抽象对象）

**G_DEFINE_ABSTRACT_TYPE**

**G_DEFINE_ABSTRACT_TYPE_WITH_CODE**

**G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE**

## 3 最终对象

<span style="color: red;">最终对象不能被继承</span>，展开抽象对象定义宏后，与 G_DEFINE_TYPE 唯一不同的地方就是 t_double_get_type_once 函数中的 GTypeFlags变量的参数是 **G_TYPE_FLAG_FINAL**。

一个final类型对象不需要有自己的类结构体成员。

**G_DEFINE_FINAL_TYPE**

**G_DEFINE_FINAL_TYPE_WITH_CODE**

**G_DEFINE_FINAL_TYPE_WITH_PRIVATE**

## 4 接口对象

接口对象可以当做一个抽象类来看待（注册该类型的时候也就仅仅注册了类，并没有注册实例结构体），但是与抽象类不同的是，虚函数可以由任意对象实现。

1. **声明** 接口初始化函数;

   `static void t_comparable_default_init (TComparableInterface *klass);`

2. **定义**t_t_comparable_get_type函数。（没有实例结构体及其实例初始化函数）
    
    
    ```c
    GType g_define_type_id = g_type_register_static_simple (((GType) ((2) << (2))), 
                                                              g_intern_static_string ("TComparable"), 
                                                              sizeof (TComparableInterface), 
                                                              (GClassInitFunc)(void (*)(void)) t_comparable_default_init, 
                                                              0,  /*没有实例结构体*/
                                                              (GInstanceInitFunc) ((void *)0), /* 没有实例初始化函数 */
                                                              (GTypeFlags) 0); 
    ```

**G_DEFINE_INTERFACE**

**G_DEFINE_INTERFACE_WITH_CODE**

## 5 封装结构体或者联合体对象

`G_DEFINE_BOXED_TYPE`` 在 GObject 系统中是一个宏，用于定义新的 “boxed type” 。GObject 的 boxed types 用于处理纯旧数据（POD）结构，这些数据类型如结构体或联合体，并使其与 GObject 类型系统兼容。

定义了 boxed type 后，你可以在 GObject 系统中，例如在 GValue 或信号中，使用这些类型。

<span style="background-color:pink">这类型对象没有信号，属性，引用计算等GObject具有的特性。<span>

<span style="background-color:pink">这类型不会自动注释到类型系统(需要调用my_struct_get_type()函数)，不能使用g_object_new。<span>

### G_DEFINE_BOXED_TYPE

### G_DEFINE_BOXED_TYPE_WITH_CODE

### G_DEFINE_BOXED_TYPE


## 补充：宏展开代码

```c
/********************************以下内容由G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)宏展开**************START*******************************/
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
                                                          sizeof (TDoubleClass), 
                                                          (GClassInitFunc)(void (*)(void)) t_double_class_intern_init, 
                                                          sizeof (TDouble), 
                                                          (GInstanceInitFunc)(void (*)(void)) t_double_init, 
                                                          (GTypeFlags) 0); 
  
  { {{};} } 
  
  return g_define_type_id; 
}


/********************************以上内容由G_DEFINE_TYPE (TDouble, t_double, G_TYPE_OBJECT)宏展开***************END********************************/
```

```c
/********************************以下内容由G_DEFINE_TYPE_WITH_CODE (TDouble, t_double, G_TYPE_OBJECT, ADD)宏展开**************START*******************************/
static void t_double_init (TDouble *self); 
static void t_double_class_init (TDoubleClass *klass); 
static GType t_double_get_type_once (void); 
static gpointer t_double_parent_class = ((void *)0); 
static gint TDouble_private_offset; 

static void 
t_double_class_intern_init (gpointer klass) { 
  t_double_parent_class = g_type_class_peek_parent (klass); 
  if (TDouble_private_offset != 0) 
    g_type_class_adjust_private_offset (klass, &TDouble_private_offset); 
    t_double_class_init ((TDoubleClass*) klass); 
} 

__attribute__ ((__unused__)) static inline gpointer 
t_double_get_instance_private (TDouble *self) { 
  return (((gpointer) ((guint8*) (self) + (glong) (TDouble_private_offset)))); 
} 

GType 
t_double_get_type (void) { 
  static gsize static_g_define_type_id = 0;
  if ((__extension__ ({ 
    _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
    (void) (0 ? (gpointer) *(&static_g_define_type_id) : ((void *)0)); 
    (!(__extension__ ({ 
      _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
      __typeof__ (*(&static_g_define_type_id)) gapg_temp_newval; 
      __typeof__ ((&static_g_define_type_id)) gapg_temp_atomic = (&static_g_define_type_id); 
      __atomic_load (gapg_temp_atomic, &gapg_temp_newval, 5); 
      gapg_temp_newval; 
      })) && g_once_init_enter (&static_g_define_type_id)); 
   }))) { 
    
    GType g_define_type_id = t_double_get_type_once (); 
    
    (__extension__ ({ 
      _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
      0 ? (void) (*(&static_g_define_type_id) = (g_define_type_id)) : (void) 0; 
      g_once_init_leave ((&static_g_define_type_id), (gsize) (g_define_type_id)); 
      })
    ); 
  } 
  return static_g_define_type_id; 
} 

__attribute__ ((__noinline__)) static GType 
t_double_get_type_once (void) { 
  GType g_define_type_id = g_type_register_static_simple (((GType) ((20) << (2))), 
                                                          g_intern_static_string ("TDouble"), 
                                                          sizeof (TDoubleClass), 
                                                          (GClassInitFunc)(void (*)(void)) t_double_class_intern_init, 
                                                          sizeof (TDouble), 
                                                          (GInstanceInitFunc)(void (*)(void)) t_double_init, 
                                                          (GTypeFlags) 0); 
  
  { {add();} } /* 与G_DEFINE_TYPE区别地方 */
  
  return g_define_type_id; 
}

/********************************以下内容由G_DEFINE_TYPE_WITH_CODE (TDouble, t_double, G_TYPE_OBJECT, ADD)宏展开***************END********************************/
```

```c
/********************************以下内容由G_DEFINE_TYPE_WITH_PRIVATE (TDouble, t_double, G_TYPE_OBJECT)宏展开**************START*******************************/
static void t_double_init (TDouble *self); 
static void t_double_class_init (TDoubleClass *klass); 
static GType t_double_get_type_once (void); 
static gpointer t_double_parent_class = ((void *)0);
static gint TDouble_private_offset; 

static void 
t_double_class_intern_init (gpointer klass) { 
  t_double_parent_class = g_type_class_peek_parent (klass);
   if (TDouble_private_offset != 0) 
    g_type_class_adjust_private_offset (klass, &TDouble_private_offset); 
  t_double_class_init ((TDoubleClass*) klass); 
} 

__attribute__ ((__unused__)) static inline gpointer 
t_double_get_instance_private (TDouble *self) { 
  return (((gpointer) ((guint8*) (self) + (glong) (TDouble_private_offset)))); 
} 

GType t_double_get_type (void) { 
  static gsize static_g_define_type_id = 0; 
  if ((__extension__ ({ 
    _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false");
    (void) (0 ? (gpointer) *(&static_g_define_type_id) : ((void *)0));
     (!(__extension__ ({ 
      _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false");
       __typeof__ (*(&static_g_define_type_id)) gapg_temp_newval; 
       __typeof__ ((&static_g_define_type_id)) gapg_temp_atomic = (&static_g_define_type_id); 
       __atomic_load (gapg_temp_atomic, &gapg_temp_newval, 5); gapg_temp_newval; 
       })) && g_once_init_enter (&static_g_define_type_id)); 
    }))) { 

      GType g_define_type_id = t_double_get_type_once (); 
    
    (__extension__ ({ 
      _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false");
      0 ? (void) (*(&static_g_define_type_id) = (g_define_type_id)) : (void) 0;
      g_once_init_leave ((&static_g_define_type_id), (gsize) (g_define_type_id)); 
      })
    );
  } 
  
  return static_g_define_type_id; 
} 

__attribute__ ((__noinline__)) static GType 
t_double_get_type_once (void) { 
  GType g_define_type_id = g_type_register_static_simple (((GType) ((20) << (2))),
   g_intern_static_string ("TDouble"), 
   sizeof (TDoubleClass), (GClassInitFunc)(void (*)(void)) 
   t_double_class_intern_init, 
   sizeof (TDouble), 
   (GInstanceInitFunc)(void (*)(void)) t_double_init, 
   (GTypeFlags) 0); { {{ 
    
    /*  与G_DEFINE_TYPE区别地方 */
    TDouble_private_offset = g_type_add_instance_private (g_define_type_id, sizeof (TDoublePrivate)); 
    
    };} } 
    
    return g_define_type_id; 
    
}

/********************************以上内容由G_DEFINE_TYPE_WITH_PRIVATE (TDouble, t_double, G_TYPE_OBJECT)宏展开***************END********************************/

```

```c
/************************************G_DEFINE_INTERFACE宏展开*********************START**********************/
static void t_comparable_default_init (TComparableInterface *klass);
GType t_comparable_get_type (void) { 
  static gsize static_g_define_type_id = 0; 
  if ((__extension__ ({ _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
    (void) (0 ? (gpointer) *(&static_g_define_type_id) : ((void *)0)); 
    (!(__extension__ ({ _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
    __typeof__ (*(&static_g_define_type_id)) gapg_temp_newval; 
    __typeof__ ((&static_g_define_type_id)) gapg_temp_atomic = (&static_g_define_type_id); 
    __atomic_load (gapg_temp_atomic, &gapg_temp_newval, 5); 
    gapg_temp_newval; 
    })) && g_once_init_enter (&static_g_define_type_id)); 
  }))) {
  
    GType g_define_type_id = g_type_register_static_simple (((GType) ((2) << (2))), 
                                                             g_intern_static_string ("TComparable"), 
                                                             sizeof (TComparableInterface), 
                                                             (GClassInitFunc)(void (*)(void)) t_comparable_default_init, 
                                                             0, 
                                                             (GInstanceInitFunc) ((void *)0), 
                                                             (GTypeFlags) 0); 
    
    if (((GType) ((20) << (2))) != ((GType) ((0) << (2)))) 
      g_type_interface_add_prerequisite (g_define_type_id, ((GType) ((20) << (2)))); 
      
      { {;;} } 
      
      (__extension__ ({ 
        _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
        0 ? (void) (*(&static_g_define_type_id) = (g_define_type_id)) : (void) 0; g_once_init_leave ((&static_g_define_type_id), (gsize) (g_define_type_id)); 
      })); 
  } 
  return static_g_define_type_id; 
}


/************************************G_DEFINE_INTERFACE宏展开*********************END**********************/
```

```c
/******************************************G_DEFINE_BOXED_TYPE (MyStruct, my_struct, my_struct_copy, my_struct_free)展开***************START*************************/

static GType my_struct_get_type_once (void);

GType my_struct_get_type (void) { 
  static gsize static_g_define_type_id = 0; 
  if ((__extension__ ({ _
    Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
    (void) (0 ? (gpointer) *(&static_g_define_type_id) : ((void *)0)); 
    (!(__extension__ ({ 
      _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
      __typeof__ (*(&static_g_define_type_id)) gapg_temp_newval; 
      __typeof__ ((&static_g_define_type_id)) gapg_temp_atomic = (&static_g_define_type_id); 
      __atomic_load (gapg_temp_atomic, &gapg_temp_newval, 5); 
      gapg_temp_newval; 
      })) && g_once_init_enter (&static_g_define_type_id)); 
    }))) { 
      GType g_define_type_id = my_struct_get_type_once (); 
      
    (__extension__ ({ 
      _Static_assert (sizeof *(&static_g_define_type_id) == sizeof (gpointer), "Expression evaluates to false"); 
      0 ? (void) (*(&static_g_define_type_id) = (g_define_type_id)) : (void) 0; 
      g_once_init_leave ((&static_g_define_type_id), (gsize) (g_define_type_id)); 
      })
    ); 
  } 
  
  return static_g_define_type_id; 
} 

__attribute__ ((__noinline__)) static GType 
my_struct_get_type_once (void) { 
  GType (* _g_register_boxed) (const gchar *, 
                               union { MyStruct * (*do_copy_type) (MyStruct *); 
                               MyStruct * (*do_const_copy_type) (const MyStruct *);  
                               GBoxedCopyFunc do_copy_boxed; } 
                              __attribute__((__transparent_union__)), 
                              union { void (* do_free_type) (MyStruct *); 
                                      GBoxedFreeFunc do_free_boxed; } 
                              __attribute__((__transparent_union__)) ) = g_boxed_type_register_static; 
                              
  GType g_define_type_id = _g_register_boxed (g_intern_static_string ("MyStruct"), my_struct_copy, my_struct_free); 

  { {{};} } 

  return g_define_type_id; 
}

/******************************************G_DEFINE_BOXED_TYPE (MyStruct, my_struct, my_struct_copy, my_struct_free)展开******************END**********************/
```