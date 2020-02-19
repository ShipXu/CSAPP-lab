# CSAPP-lab
本项目是一个笔者对自己自学CSAPP过程的一个记录，主要包括集合CSAPP学习的相关资源，分享课程的记录，以及分享实验过程。

# Readme

### 课程资源链接
- [课程地址](https://www.bilibili.com/video/av31289365/?p=3&t=4)
- [实验资源](http://csapp.cs.cmu.edu/3e/labs.html)
- [十分钟计算机](http://open.163.com/newview/movie/free?pid=MDGEPAQ4K&mid=MDGET74MS)：可以帮助理解计算机结构，更好地学习CSAPP课程

### 课程记录分享【持续更新】
#### Lecture01 观看记录📝
- 05:23 课程主题Abstraction is Good But Don’t Forget The Reality
- 07:22 课程目标 1.让你更有效率；2.给你十分有用的工具；
- 07:35 课程内容和课程意义

##### 课程内容，几个关于系统的例子
- 09:13 #1 Ints和Floats (gdb)
- 17:03 #2 Assembly汇编语言
- 18:37 #3 内存的重要性
- 25:23 #4 底层优化
- 28:59 #5 I/O的重要性和网络

##### 课程意义
- 29:07 课程意义部分

##### 课程观点
- 30:31 builder角度和程序员角度
- 34:44 关于北大旁书店的笑话 还蛮好笑的
- 38:49 学术诚信:高层次可以，低层次不行
- 46:14 学习策略，每次阅读每章三次，然后做练习题
- 51:16 课程邮箱，FAQ

##### - 1:02:01 六个实验的介绍
- 1:02:08 数据实验
- 1:03:24 炸弹实验
- 1:06:09 攻击实验
- 1:07:11 缓存实验
- 1:09:27 Shell实验
- 1:09:55 malloc实验
- 1:12:20 proxy实验

#### Lecture02-Lecture03 观看总结
- 位表示
- 位级别的操作
- 整数
- - 表示：无符号和有符号
- - 约定和转换
- - 扩展和截断
- - 加法，减法，乘法，移位
- - 内存，指针，字符串的表示
- 总结

#### Lecture02 观看记录📝
- 02:13 bit比特

##### - 03:33 二进制的数字表示
- 04:49 编码一个字节以及16进制表示
- 06:39 C中数据类型的长度
- 08:50 布尔代数和操作符
- 11:10 对向量操作的布尔代数
- 12:07 使用布尔代数表示和操作集合
- 14:30 逻辑操作和位操作的对比
- 18:44 移位操作
- 21:09 数字表示
- 26:37 数字表示的范围

##### - 有符号数和无符号数
- 35:58 位表示到两者的转换
- 37:16 有符号数和无符号数的相互转换
- 41:46 C中有符号数和无符号数
- 55:57 有符号数和无符号数的转换
- 57:53 关于无符号数的两个问题
- 1:01:03 有符号数的扩展
- 1:06:04 扩展和截断的总结


#### Lecture03 观看记录📝
- 02:32 无符号数的加法
- 06:19 无符号运算加法的可视化
- 06:42 补码的加法
- 13:42 补码加法的可视化
- 15:02 无符号数的乘法
- 17:20 C中有符号数的乘法
- 24:33 2的指数幂乘法和左移等价
- 26:32 2的指数幂除法和右移等价
- 28:48 逻辑移位和算术移位应用
- 36:03 不要使用无符号数的原因
- 39:03 如果使用无符号数的规范
- 44:22 使用无符号数的原因
- 44:56 内存，指针，字符串的表示
- 48:00 内存的结构梳理
- 49:39 机器的字长
- 53:14 面向字节的内存结构
- 【这里关于内存，需要推荐一下另一门课《十分钟计算机科学-内存部分》，看完对这部分会有更好的理解】
- 57:05 小端序和大端序
- 1:01:19 数据表示的例子
- 1:08:38 字符串部分「很简单地略过」
- 1:09:58 整数问题的小测验

#### Lecture04 浮点数 观看记录📝
- 2:44 二进制分数的表示，浮点数“浮点”的来源
- 08:07 IEEE floating point
- 08:25 浮点数的表示
- 10:56 规格化的值
- 20:20 解释exp阶码部分，这个地方其实解释有误；如果看书的话，可以发现很明显exp是不等于255的
- 23:53 非规格化的值
- 26:14 特殊值
- 28:29 浮点数编码的可视化
- 29:49 浮点数编码的简单例子
- 39:56 浮点数编码的属性
- 41:34 浮点数运算的基本思想(运算后舍入)
- 44:25 舍入的方法
- 51:33 二进制的舍入
- 54:56 浮点数的乘法
- 56:30 浮点数的加法
- 56:54 浮点数加法和乘法的数学性质
- 62:59 C中的浮点数
- 67:20 浮点数的练习题

#### Lecture05 机器级编程(基础) 观看记录📝
- **01:49 课程学习方法**
- **03:54 intel处理器的历史和架构**
    - 06:25 RISC和CISC的历史
    - 08:12 英特尔架构的里程碑
    - 12:51 Corei7的架构
    - 14:28 竞争对手AMD的历史
    - 15:35 64位的转变
    - 17:49 ARM处理器
- **19:35 C,assembly,machine code**
    - 22:02 Machine/Assembly code的视角
    - 25:19 将C代码翻译为目标代码的过程
    - 30:12 C代码到汇编代码的例子，-O优化
    - 34:36 汇编代码的特性
    - 34:48 数据类型
    - 35:56 操作
    - 36:54 sumstore的例子
    - 39:14 Disassembling反汇编器：objdump和gdb
- **47:50 Assembly基础:寄存器，操作数，move指令**
    - 48:12 X86-64寄存器
    - 53:38 move指令
    - 56:59 Simple Memory Addressing Modes
    - 58:09 简单寻址的例子
    - 56:59 Complex Memory Addressing Modes
    - 65:12 寻址的例子
    - 66:15 算术和逻辑操作
    - 66:40 lea指令
- **73:02 总结**

#### Lecture06 机器级编程(控制) 观看记录📝
- 04:00 处理器的状态（关于当前运行程序的信息，rax,rsp,rip）
- 05:51 条件码
- 07:55 比较指令(cmp,test)
- 11:46 读状态码(setx,)
- **20:14 状态分支**
    - 20:36 跳转指令(这里还提及了+=的来源)
    - 26:54 C中goto语句(尽量还是不要用，不清晰)
    - 29:05 通用条件表达的转换(使用分支)
    - 31:41 条件移动
    - 35:48 条件移动的例子
    - 36:49 条件移动不适合的情况
- **38:27 循环**
- (总的来说就是把各种形式的循环归并成一种形式)
    - 38:29 "Do while"的例子(C语言)
    - 40:44 "Do while"的例子(编译结果)
    - 41:05 通用的"Do while"的Goto版本
    - 41:21 通用的"While"的Goto版本(1)
    - 44:07 "While"的例子
    - 45:34 通用的"While"的Goto版本(2)
    - 46:26 "For Loop"的形式
    - 47:38 "For" -> "While Loop"的转换
- **50:10 Switch语句**
    - 51:01 Switch语句的例子
    - 53:44 Switch的解释(不是if-else的组合！！！超酷的)
- **1:12:31 总结**

#### Lecture07 机器级编程(过程) 观看记录📝
- Procedures
    - Stack Structure
    - Calling Conventions
        - Passing control
        - Passing data
        - Managing local data
    - Illustration of Recursion

- **2:45 ABI**
    - 3:50 Procedures的机制
    - 7:34 X86-64Stack
- **ABI中调用的约定**
    - **14:38 过程中的控制**
        - 15:32 过程中的控制(例子)
    - **21:43 过程中的数据(参数传递)**
        - 23:06 过程中的数据(例子)
    - **26:19 过程中的局部变量管理**
        - 26:22 栈帧
        - 31:15 一个具体的例子
        - 34:42 X86-64/Linux栈帧的结构
        - 39:06 Calling incr的例子
    - **过程调用中寄存器的使用**
        - 46:29 Register保存的约定
        - 49:38 不同寄存器的用途
        - 51:17 被调用者保存寄存器的使用示例及其工作原理
    - **递归部分的解释**
        - 54:31 递归的例子
        - 1:03:40 递归部分的总结

#### Lecture08 机器级编程(数据) 观看记录📝
- **数组**
    - 04:16 数组声明
    - 05:14 数组访问
    - 14:38 数组的例子
    - 21:49 数组和指针的不同(C语言部分，可看书)
    - 39:47 嵌套数组
    - 45:32 嵌套数组的访问
    - 51:04 多层数组的元素访问
    - 54:58 N*N的矩阵
    - 56:41 16*16数组访问的例子
- **结构**
    - 59:24 结构体的表示
    - 1:00:16 指向结构体成员的指针
    - 1:05:34 结构体和对齐的概念
    - 1:07:03 对齐的原则
    - 1:11:15 节省内存
- **浮点数**
    - 1:13:00 浮点数的历史
    - 1:14:44 SSE3编程
    - 1:16:01 Scalar和SIMD(真的好感动，实习时用过)
    - 1:17:13 FP运算的知识
- 其实这里应该提及多个浮点数一起运算的加速效果