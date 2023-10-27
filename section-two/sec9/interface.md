# 接口

接口（Interface）类似于抽象类（Abstract Class）。接口定义了虚拟函数，期望另一个可实例化对象中的函数来覆盖。

本节提供了一个简单的示例，TComparable。TComparable 是一个接口。它定义了比较函数，包括以下内容：

- t_comparable_cmp(self, other)：它比较 self 和 other。第一个参数 self 是 t_comparable_cmp 执行的实例。第二个参数 other 是另一个实例。这个函数需要在实现接口的对象中被覆盖。

    - 如果 self 等于 other，t_comparable_cmp 返回 0。

    - 如果 self 大于 other，t_comparable_cmp 返回 1。

    - 如果 self 小于 other，t_comparable_cmp 返回 -1。

    - 如果发生错误，t_comparable_cmp 返回 -2。

- t_comparable_eq(self, other)：如果 self 等于 other，则返回 TRUE。否则返回 FALSE。需要注意的是，即使发生错误，也会返回 FALSE。

- t_comparable_gt(self, other)：如果 self 大于 other，则返回 TRUE。否则返回 FALSE。

- t_comparable_lt(self, other)：如果 self 小于 other，则返回 TRUE。否则返回 FALSE。

- t_comparable_ge(self, other)：如果 self 大于等于 other，则返回 TRUE。否则返回 FALSE。

- t_comparable_le(self, other)：如果 self 小于等于 other，则返回 TRUE。否则返回 FALSE。

数字和字符串是可比较的。TInt、TDouble 和 TStr 实现了 TComparable 接口，以便可以使用上述函数。此外，TNumStr 也可以使用这些函数，因为它是 TStr 的子类。

例如，

```c
TInt *i1 = t_int_new_with_value (10);
TInt *i2 = t_int_new_with_value (20);
t_comparable_eq (T_COMPARABLE (i1), T_COMPARABLE (i2)); /* => FALSE */
t_comparable_lt (T_COMPARABLE (i1), T_COMPARABLE (i2)); /* => TRUE */
```

<span style="color: red;">接口和抽象类之间有什么区别？抽象类中的虚拟函数由其 **派生类** 中的函数覆盖。接口中的虚拟函数由 **任何类** 中的函数覆盖。比较 TNumber 和 TComparable。</span>

在 TIntClass 和 TDoubleClass 中重写了函数 t_number_add。但它不能在 TStrClass 中被重写，因为 TStr 不是 TNumber 的派生类。

在 TIntClass、TDoubleClass 和 TStrClass 中重写了函数 t_comparable_cmp。

## 1 TComparable interface

定义接口与定义对象类似。

- 使用 G_DECLARE_INTERFACE 代替 G_DECLARE_FINAL_TYPE。
- 使用 G_DEFINE_INTERFACE 代替 G_DEFINE_TYPE。

现在让我们看看头文件。

```c
 1 #pragma once
 2 
 3 #include <glib-object.h>
 4 
 5 #define T_TYPE_COMPARABLE  (t_comparable_get_type ())
 6 G_DECLARE_INTERFACE (TComparable, t_comparable, T, COMPARABLE, GObject)
 7 
 8 struct _TComparableInterface {
 9   GTypeInterface parent;
10   /* signal */
11   void (*arg_error) (TComparable *self);
12   /* virtual function */
13   int (*cmp) (TComparable *self, TComparable *other);
14 };
15 
16 /* t_comparable_cmp */
17 /* if self > other, then returns 1 */
18 /* if self = other, then returns 0 */
19 /* if self < other, then returns -1 */
20 /* if error happens, then returns -2 */
21 
22 int
23 t_comparable_cmp (TComparable *self, TComparable *other);
24 
25 gboolean
26 t_comparable_eq (TComparable *self, TComparable *other);
27 
28 gboolean
29 t_comparable_gt (TComparable *self, TComparable *other);
30 
31 gboolean
32 t_comparable_lt (TComparable *self, TComparable *other);
33 
34 gboolean
35 t_comparable_ge (TComparable *self, TComparable *other);
36 
37 gboolean
38 t_comparable_le (TComparable *self, TComparable *other);
```

- 6: G_DECLARE_INTERFACE宏。最后一个参数是接口的先决条件。TComparable的先决条件是GObject。因此，除了GObject的派生类之外，例如GVariant，不能实现TComparable。先决条件是接口或类的GType。此宏扩展为：

  - t_comparable_get_type()的声明。
  - typedef struct _TComparableInterface TComparableInterface

  - T_COMPARABLE()宏。它将一个实例转换为TComparable类型。

  - T_IS_COMPARABLE()宏。它检查实例的类型是否为T_TYPE_COMPARABLE。

  - T_COMPARABLE_GET_IFACE()宏。它获取给定作为参数的实例的接口。

- 8-14: TComparableInterface结构。这类似于类结构。第一个成员是父接口。TComparableInterface的父接口是GTypeInterface。GTypeInterface是所有接口类型的基类。它类似于是GTypeClass，它是所有类类型的基类。GTypeClass是结构GObjectClass的第一个成员。 (请参阅gobject.h。请注意，GObjectClass与struct _GObjectClass相同)。下一个成员是指向“arg-error”信号的默认信号处理程序arg_error的指针。当比较函数的第二个参数不合适时，将触发此信号。例如，如果self是TInt，other是TStr，它们都是Comparable实例。但它们不能进行比较。这是因为other不是TNumber。最后一个成员cmp是指向比较方法的指针。这是一个虚拟函数，预计将由实现接口的对象中的函数覆盖。

- 22-38: 公共函数。

C文件tcomparable.c如下所示：

```c
 1 #include "tcomparable.h"
 2 
 3 static guint t_comparable_signal;
 4 
 5 G_DEFINE_INTERFACE (TComparable, t_comparable, G_TYPE_OBJECT)
 6 
 7 static void
 8 arg_error_default_cb (TComparable *self) {
 9   g_printerr ("\nTComparable: argument error.\n");
10 }
11 
12 static void
13 t_comparable_default_init (TComparableInterface *iface) {
14   /* virtual function */
15   iface->cmp = NULL;
16   /* argument error signal */
17   iface->arg_error = arg_error_default_cb;
18   t_comparable_signal =
19   g_signal_new ("arg-error",
20                 T_TYPE_COMPARABLE,
21                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
22                 G_STRUCT_OFFSET (TComparableInterface, arg_error),
23                 NULL /* accumulator */,
24                 NULL /* accumulator data */,
25                 NULL /* C marshaller */,
26                 G_TYPE_NONE /* return_type */,
27                 0     /* n_params */
28                 );
29 }
30 
31 int
32 t_comparable_cmp (TComparable *self, TComparable *other) {
33   g_return_val_if_fail (T_IS_COMPARABLE (self), -2);
34 
35   TComparableInterface *iface = T_COMPARABLE_GET_IFACE (self);
36   
37   return (iface->cmp == NULL ? -2 : iface->cmp (self, other));
38 }
39 
40 gboolean
41 t_comparable_eq (TComparable *self, TComparable *other) {
42   return (t_comparable_cmp (self, other) == 0);
43 }
44 
45 gboolean
46 t_comparable_gt (TComparable *self, TComparable *other) {
47   return (t_comparable_cmp (self, other) == 1);
48 }
49 
50 gboolean
51 t_comparable_lt (TComparable *self, TComparable *other) {
52   return (t_comparable_cmp (self, other) == -1);
53 }
54 
55 gboolean
56 t_comparable_ge (TComparable *self, TComparable *other) {
57   int result = t_comparable_cmp (self, other);
58   return (result == 1 || result == 0);
59 }
60 
61 gboolean
62 t_comparable_le (TComparable *self, TComparable *other) {
63   int result = t_comparable_cmp (self, other);
64   return (result == -1 || result == 0);
65 }
```

- 5： G_DEFINE_INTERFACE宏。第三个参数是前提条件的类型。此宏扩展为：

  - 声明t_comparable_default_init()。

  - 定义t_comparable_get_type()。

- 7-10: arg_error_default_cb是"arg-error"信号的默认信号处理程序。

- 12-29: t_comparable_default_init函数。此函数类似于类初始化函数。它初始化TComparableInterface结构。

- 15: 将cmp赋值为NULL。因此，在实现类覆盖它之前，比较方法不起作用。

- 17: 设置"arg-error"信号的默认信号处理程序。

- 18-28: 创建"arg-error"信号。

- 31-38: 函数t_comparable_cmp。它首先检查self的类型。如果它不可比较，它会记录错误消息并返回-2（错误）。如果iface->cmp为NULL（这意味着类方法还没有被覆盖），则返回-2。否则，它调用类方法并返回类方法返回的值。

- 40-65: 公共函数。这五个函数都基于t_comparable_cmp。因此，它们不需要被覆盖。例如，t_comparable_eq只是调用t_comparable_cmp。如果t_comparable_cmp返回零，它返回TRUE。否则，它返回FALSE。

这个程序使用信号向用户提供参数类型错误信息。这种错误通常是程序错误，而不是运行时错误。使用信号报告程序错误并不是一个好方法。最好的方法是使用g_return_if_fail。我之所以使用这个信号，只是想演示如何在接口中实现信号。

## 2 Implementing interface
