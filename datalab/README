首先，需要通过下面代码，为dlc添加运行权限:
```
chmod +x dlc
```

## 环境配置问题
一开始，根据README介绍运行环境时就出现了问题，首先显示
```
/usr/include/gnu/stubs.h:7:27: 致命错误：gnu/stubs-32.h：没有那个文件或目录
```
谷歌之后，根据[编译GCC-4.8出现的错误 —— Linux gnu/stubs-32.h: No such file or directory](https://blog.csdn.net/wang_xijue/article/details/47128423)安装了32位的C运行库，但依旧不行，显示:
```
btest.c: 在函数‘test_function’中:
btest.c:332:23: 警告：‘arg_test_range[1]’ may be used uninitialized in this function [-Wmaybe-uninitialized]
     if (arg_test_range[1] < 1)
         ~~~~~~~~~~~~~~^~~
/usr/bin/ld: 当搜索用于 /usr/local/lib/gcc/x86_64-pc-linux-gnu/7.1.0/libgcc.a 时跳过不兼容的 -lgcc 
/usr/bin/ld: 找不到 -lgcc
/usr/bin/ld: 找不到 -lgcc_s
```
之后，通过[cannot find -lgcc_s](https://www.cnblogs.com/chobits/p/4281732.html)
博客，定位到了问题可能出自32的编译过程中。

通过查询《lgcc_s 32位库》，得到了最终答案
[GCC在64位系统上编译32位程序遇到的问题](http://notes.maxwi.com/2017/12/06/compile-x32-executable-at-x64-linux-system/)


解决好环境问题之后，就可以开始A题了

## 实验规则
### INTEGER编码规则:

用一到多行代码替换每个函数中的"return"表达式，你的代码一定要复合以下的风格:
```
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }
```

每个"Expr"是**只**使用以下规则的元素的一个表达式:
1. 0-255之间的整数(不允许使用大常量，例如: 0xffffffff);
2. 函数参数和局部变量(不是一个全局变量);
3. Unary integer operations: ! ~
4. Binary integer operations: & ^ | + << >>
一些问题对操作符有更深的限制。每个"Expr"可能由多个操作符组成，你可以不必每一行只有一个操作符;

你需要注意以下的禁止项：
1. 使用控制结构，例如:if, do, while, for, switch
2. Define or use any macros. 定义和使用宏
3. Define any additional functions in this file. 在文件中定义额外的函数
4. Call any functions. 调用任何函数
5. Use any other operations, such as &&, ||, -, or ?: 使用任何其他操作，例如:&&, ||, -, or ? 
6. 不准使用任何形式的cast转换
5. 不准使用除了int之外的数据类型，包括数组、结构体和联合

你可以假设你的机器:
1. 使用2进制，使用32位表示整数int；
2. 算术上执行右移操作;
3. 当shift额度小于0或者大于31时，会产生不可预测的行为;


EXAMPLES OF ACCEPTABLE CODING STYLE:
可以接受的编码风格示例
```
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }
```
```
  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }
```

### FLOATING POINT编码规则

对于浮点数的操作，coding rules没有之前的实验那么严格: 
- 你可以使用循环和判断控制
- 你被允许使用int和unsigned类型的变量
- 你可以使用任意的整数和unsigned的常量
- 你可以使用对于int或者unsigned定义的任意算法，逻辑和比较操作。

你需要注意以下的禁止项：
1. 不准用宏
2. 不准定义任何其他函数
3. 不准调用函数
4. 不准使用任何形式的cast转换
5. 不准使用除了int和unsigned之外的数据类型，包括数组、结构体和联合
6. 不准使用任何的浮点类型相关的变量，操作和常量

提示:
1. 采用dlc (data lab checker) 编译器，来检查你的答案的合法性;
2. 每一个函数都有操作数目的最大限制，你在实现的时候需要注意。最大的操作数，dlc在编译期间会检查;
3. 使用btest test harness检查你的函数的正确性;
4. 使用BDD checker来正式验证你的函数;

## int部分实验
刚开始以为int部分应该挺简单的，做的时候发现会是要很动一下脑子，主要是在运算符方面的限制太大了
### 3分级别的
#### isAsciiDigit
```C++
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  return (!!(((x + ~0x30 + 1) >> 31) + 1)) & (!(((x + ~0x39 + 1) >> 31) + 1));
}
```
第一部分，需要对等式做下变形，我们将原始的不等式变形为:
```
x - 0x30 >= 0
x - 0x39 <= 0
```
然后，我们要思考，如何使用合法的运算符组合出我们要的减法-和比较运算符:<=、>=
##### -部分可以参考上一题 
```
int negate(int x) {
  return ~x + 1;
}
```
那么就有不等式
```
(x + ~0x30 + 1) >= 0
(x + ~0x39 + 1) <= 0
```
##### 比较符部分可以考虑利用补码的负数位
```
(x >> 31) + 1
// 如果x负数位为1，则(x >> 31) = 0xffffffff
// 换句话说 x < 0 逻辑等价于 (x >> 31) + 1
```
则不等式可以变形为
```
(!!(((x + ~0x30 + 1) >> 31) + 1)) 
& 
(!(((x + ~0x3a + 1) >> 31) + 1))
```
注意
```
// <= 0x39这个地方要绕一下，因为我们只有小于号的表示
// 另外负数位为1，则一定为小于，这点可以好好体会一下
(!(((x + ~0x3a + 1) >> 31) + 1))
```

#### conditional
```C++
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  return 2;
}
```
三元运算符部分，刚开始还是挺头疼的，但是仔细想了想其实也不难，首先一定要表达出x==0这个部分
##### x==0
```
// x != 0时
!x = 0x00000000
!!x = 0x00000001
// x == 0时
!x = 0x00000001
!!x = 0x00000000
```
##### 利用加法得到屏蔽位
```
// x != 0时
!x = 0x00000000 + 0xffffffff = 0xffffffff
!!x = 0x00000001 + 0xffffffff = 0x00000000
// x == 0时
!x = 0x00000001 + 0xffffffff = 0xffffffff
!!x = 0x00000000 + 0xffffffff = 0x00000000
```
##### 利用|来组合两个数
```C++
// m_one = -1
m_one = 0xffffffff
(((!x) + m_one) & y) | (((!!x) + m_one) & z);
```
##### 用移位操作组合出负一
```
// 在没有加法的情况组合出-1，也是需要思考的
// 1 >> 31只会产生0，这点也可以回顾一下逻辑移位部分
(1 << 31) >> 31
```

##### 最后答案
```C++
int conditional(int x, int y, int z) {
  int m_one = (1 << 31) >> 31;
  return (((!x) + m_one) & y) | (((!!x) + m_one) & z);
}
```

#### isLessOrEqual
```C++
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  return 2;
}
```
嘎 看到这部分，我觉得可能之前ASCII码部分会不会做的太复杂了
= =，利用之前部分的思考过程

```
// y - x >= 0
!!(((y + ~x + 1) >> 31) + 1))

```
这时候突然发现踩坑了，这道题的困难之处，在于如果你直接采用```y - x ```来处理，可能会导致溢出的情况。所以，我的做法是分开异号和同号的情况。

```C++
int isLessOrEqual(int x, int y) {
  int sign = (x ^ y) >> 31;
  int ret1 = (!((y + ~x + 1) >> 31)) & (!sign);
  int ret2 = (x >> 31) & !!sign;
  return ret1 | ret2;
}
```

##### 是否异号的判断
```C++
// sign为0，两者同号
// sign为1，两者异号
sign = (x ^ y) >> 31;
```

##### 异号的情况
```C++
// 异号时，通过判断x的符号位判断x是否为负数
// 如果x为负数且异号，则ret2返回True
ret2 = (x >> 31) & !!sign;
```

##### 同号的情况
```C++
// 同号时，判断y >= x是否成立
// 通过y - x进行转换 
ret1 = (!((y + ~x + 1) >> 31)) & (!sign);
```

一开始的想法是，用0xFFFFFFF加上一个值，然后移位后来想想，不如直接用0x8000000。
这道题的初始想法很简单，但是边界情况还是要考虑挺多的。

最基础的想法，就是0其实就是不是正数也不是负数的情况。0的特殊之处在于，在补码中，只有正0。
所以第一步，我们检查首位是不是0，判断它的负权值为1。
然后我们检查这个数+(-1)后，如果是0那么+(-1)后，最高位仍然为1，如果不是0，那么+(-1)后，最高位就肯定不为1。

合理运算符没有==，所以我们采用^不是来表达是的概念(二元运算的方便之处就在这)。
```C++
int logicalNeg(int x) {
  int ret1 = ((x >> 31) & 1) ^ 1;
  int ret2 = (((x + ((1 << 31) >> 31)) >> 31) & 1) ^ 0;
  return ret1 & ret2;
}
```

首先，肯定是要有判断每一位是否为1这个操作的，问题在于如何实现这个只取最大位的操作。
刚开始，我甚至认为这题出错了，因为几个例子我发现都不对。后来发现，这道题是说，用二进制补码来表示的最小位数，比如说
```C++
// 12 = 01100
howManyBits(12) = 5
// 298 = 0100110000
howManyBits(298) = 10
// -5 = 1011
howManyBits(-5) = 4
// 0  = 0
howManyBits(0)  = 1
// -1 = 1
hManyBits(-1) = 1
// 0x80000000 = 32
howManyBits(0x80000000) = 32
```
这个地方，我们很容易观察到的一个直觉是，最高位出现在第一个异号的位置附近。然后，我们怎么解释这个问题，就是利用"补码的符号扩展"的，不懂的同学请回去翻书。
只有二分查找有可能可以解决这个问题。

这个小问题，花了一天的时间才想清楚:
二分查找的变种，在5次查找之内
```
int howManyBits(int x) {
  int m_one = (1 << 31) >> 31;
  int r = 32;
  int t = x >> 15;
  int t_flag = !((!t) | !(t + 1)) + m_one;
  r = r + ~(t_flag & 16) + 1;
  x = x << (t_flag & 16);

  t = x >> 23;
  t_flag = !((!t) | !(t + 1)) + m_one;
  r = r + ~(t_flag & 8) + 1;
  x = x << (t_flag & 8);

  t = x >> 27;
  t_flag = !((!t) | !(t + 1)) + m_one;
  r = r + ~(t_flag & 4) + 1;
  x = x << (t_flag & 4);

  t = x >> 29;
  t_flag = !((!t) | !(t + 1)) + m_one;
  r = r + ~(t_flag & 2) + 1;
  x = x << (t_flag & 2);

  t = x >> 30;
  t_flag = !((!t) | !(t + 1)) + m_one;
  r = r + ~(t_flag & 1) + 1;
  x = x << (t_flag & 1);

  return r;
}
```


## 
这里犯了个错误，以为还是int补码型，所以写了这样一个判断
```C++
int e = ((uf << 1) >> 24);// & (0xff);

```
unsigned都是逻辑右移，不会补1