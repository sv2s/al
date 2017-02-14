###手机恢复出厂设置默认语言更改。
   /home/wangyao/Code/Z6U110H/frameworks/base/core/jni/AndroidRuntime.cpp文件中readLocale(char* language, char* region)方法中添加
```
   #elif CUSTOM_DEFAULT_UI_18
                    propLang[0] = 'z';
                    propLang[1] = 'h';
                    propRegn[0] = 'C';
                    propRegn[1] = 'N';
```
***

### makefile
 targetfilename ： dependonfilename （目标文件 ： 依赖文件）

 command开头用tab

 A (大写的作为变量) 

 A = a.c b.c a.h (赋值)

 $(A) 获取变量的值

 $@, $< 和 $^特殊变量：

 $@ 扩展成当前规则的目的文件名， $< 扩展成依靠列表中的第一个依靠文件，而 $^ 扩展成整个依靠的列表（除掉了里面所有重 复的文件名）
 
 如果你把 生成 foo.o 和 bar.o 的命令从它们的规则中删除， make 将会查找它的隐含规则，然后会找到一个适当的命令。它的命令会 使用一些变量，因此你可以按照你的想法来设定它：它使用变量 CC 做为编译器（象我们在前面的例子），并且传递变量 CFLAGS （给 C 编译器，C++ 编译器用 CXXFLAGS ），CPPFLAGS （ C 预 处理器旗标）， TARGET_ARCH （现在不用考虑这个），然后它加 入旗标 '-c' ，后面跟变量 $< （第一个依靠名），然后是旗 标 '-o' 跟变量 $@ （目的文件名）。一个Ｃ编译的具体命令将 会是：
 
 $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@
***
####假象目的 (Phony Targets)
 
all : exec1 exec2

veryclean :
 rm *.o
 rm a

通过使用命令make veryclean 才可以执行

***
####函数 (Functions)：

SOURCES = $(wildcard *.c)
 
这行会产生一个所有以 '.c' 结尾的文件的列表，然后存入变量 SOURCES 里。当然你不需要一定要把结果存入一个变量。

另一个有用的函数是 patsubst （ patten substitude, 匹配替 换的缩写）函数。它需要３个参数——第一个是一个需要匹配的 式样，第二个表示用什么来替换它，第三个是一个需要被处理的由空格分隔的字列。例如，处理那个经过上面定义后的变量，
 
OBJS = $(patsubst %.c,%.o,$(SOURCES))

= 是最基本的赋值
:= 是覆盖之前的值
?= 是如果没有被赋值过就赋予等号后面的值
+= 是添加等号后面的值

1、“=”

  make会将整个makefile展开后，再决定变量的值。也就是说，变量的值将会是整个makefile中最后被指定的值。看例子：

            x = foo
            y = $(x) bar
            x = xyz

  在上例中，y的值将会是 xyz bar ，而不是 foo bar 。

2、“:=”

 “:=”表示变量的值决定于它在makefile中的位置，而不是整个makefile展开后的最终值。

            x := foo
            y := $(x) bar
            x := xyz

   在上例中，y的值将会是 foo bar ，而不是 xyz bar 了。

***
### Android.mk
***

---------- cut here ------------------

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= helloworld

LOCAL_SRC_FILES := helloworld.c

include $(BUILD_SHARED_LIBRARY)

---------- cut here ------------------

好，我们来解释一下这几行代码：
***

LOCAL_PATH := $(call my-dir)

一个Android.mk file首先必须定义好LOCAL_PATH变量。它用于在开发树中查找源文件。在这个例子中，宏函数’my-dir’, 由编译系统提供，用于返回当前路径（即包含Android.mk file文件的目录）。

include $( CLEAR_VARS)

CLEAR_VARS由编译系统提供，指定让GNU MAKEFILE为你清除许多LOCAL_XXX变量（例如 LOCAL_MODULE, LOCAL_SRC_FILES, LOCAL_STATIC_LIBRARIES, 等等...),

除LOCAL_PATH 。这是必要的，因为所有的编译控制文件都在同一个GNU MAKE执行环境中，所有的变量都是全局的。

LOCAL_MODULE := helloworld

LOCAL_MODULE变量必须定义，以标识你在Android.mk文件中描述的每个模块。名称必须是唯一的，而且不包含任何空格。注意编译系统会自动产生合适的前缀和后缀，换句话说，一个被命名为'foo'的共享库模块，将会生成'libfoo.so'文件。

重要注意事项

如果你把库命名为‘libhelloworld’，编译系统将不会添加任何的lib前缀，也会生成libhelloworld.so，这是为了支持来源于Android平台的源代码的Android.mk文件，如果你确实需要这么做的话。

LOCAL_SRC_FILES := helloworld.c

LOCAL_SRC_FILES变量必须包含将要编译打包进模块中的C或C++源代码文件。注意，你不用在这里列出头文件和包含文件，因为编译系统将会自动为你找出依赖型的文件；仅仅列出直接传递给编译器的源代码文件就好。【注意，默认的C++源码文件的扩展名是’.cpp’. 指定一个不同的扩展名也是可能的，只要定义LOCAL_DEFAULT_CPP_EXTENSION变量，不要忘记开始的小圆点（也就是定义为‘.cxx’,而不是‘cxx’）（当然这一步我们一般不会去改它）】

include $(BUILD_SHARED_LIBRARY)

BUILD_SHARED_LIBRARY是编译系统提供的变量，指向一个GNU Makefile脚本（应该就是在build/core目录下的shared_library.mk），负责收集自从上次调用'include $(CLEAR_VARS)'以来，定义在LOCAL_XXX变量中的所有信息，并且决定编译什么，如何正确地去做。并根据其规则生成静态库。同理对于静态库。

