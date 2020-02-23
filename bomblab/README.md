# bomb lab
## 实验准备
bomb lab真的可以说是收获很大一个课程，整个实验我整整做了三天。一开始的学习曲线是非常陡峭的，也就是一开始你解决第一个问题的难度是很高的。

首先你要先学习gdb的使用方式，然后要适应追踪程序的过程。这里,附上也附上一些关于gdb的有用的指令:
- ni 下一条汇编指令(跳过过程)
- si 下一个汇编指令(进入过程)
- b *(地址) 在某一个地址处设置断点
- continue 继续执行到下一个断点
- display /i $pc 显示过程中命令的位置
- display /x $rdi 过程第一个参数
- display /x $rsi 过程第二个参数
- display /x $rdx 过程第三个参数
- display /x $rcx 过程第四个参数
- display /x $rsp 过程中的栈地址
- x/1sb 内存地址
- x/32xw 内存地址

之后，phase_1-phase_6的难度是不断增加的，到最后还潜藏着一个secrete_phase。一开始可能对机器码很不熟悉，但是随着不断地通过关卡，对机器码知识的了解也会不断增加。

所以，来拆弹吧ヾ(◍°∇°◍)ﾉﾞ。

## 实验说明
- **邪恶博士的阴险炸弹**
- 邪恶博士炸药有限公司特此授予您（受害者）获得使用该炸弹的明确许可
- 这是一个限时许可，在您死亡后失效
- 本炸药不会对受害者造成的，包括受伤，受挫，抓狂，红眼，手腕折损，睡眠不足及其他症状等，承担任何责任
- (除非本公司想为此获得荣誉>_<>)
- 同时应该没有受害者会使用调试，反向工程，运行“字符串”，反编译，解密或其他技术等，来了解和解除炸弹程序
- 本公司不会为本公司的无良的幽默而道歉
- 最后一句:这条许可是无用的，因为实际上炸弹并不合法。

## answer
- Border relations with Canada have never been better.
- 1 2 4 8 16 32
- 0 207
- 7 0 DrEvil
- ionefg
- 4 3 2 1 6 5
- 22

## 实验过程
### phase_1
首先看phase_1的函数
```
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq   
```
调用```<strings_not_equal>```后，若返回值为0，则跳转至400ef7，不会调用```<explode_bomb>```引爆炸弹:
```
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7
```

之后跟踪```<strings_not_equal>```函数，可以发现这是一个比对两个字符串是否相同的函数，传入的两个参数为字符串的首地址指针。

然后，我们去查看rdi和rsi的地址,分别为0x402400和0x603780
```
(gdb) ni
0x0000000000400ee9 in phase_1 ()
4: /x $rsp = 0x7fffffffe390
3: /x $rsi = 0x402400
2: /x $rdi = 0x603780
1: x/i $pc
```

查看rsi处内存的值，发现需要输入的文字为
```
(gdb) x/1sb 0x402400
0x402400:       "Border relations with Canada have never been better."
```

这样，第一关就通过了ヾ(◍°∇°◍)ﾉﾞ
```
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Border relations with Canada have never been better.
Phase 1 defused. How about the next one?
```

### phase_2
首先，在phase_2设置断点后，观察phase_2的源代码:
```asm
0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp
  400efd:	53                   	push   %rbx
  400efe:	48 83 ec 28          	sub    $0x28,%rsp
  400f02:	48 89 e6             	mov    %rsp,%rsi
  400f05:	e8 52 05 00 00       	callq  40145c <read_six_numbers>
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)
  400f0e:	74 20                	je     400f30 <phase_2+0x34>
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax
  400f1a:	01 c0                	add    %eax,%eax
  400f1c:	39 03                	cmp    %eax,(%rbx)
  400f1e:	74 05                	je     400f25 <phase_2+0x29>
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb>
  400f25:	48 83 c3 04          	add    $0x4,%rbx
  400f29:	48 39 eb             	cmp    %rbp,%rbx
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40>
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp
  400f3a:	eb db                	jmp    400f17 <phase_2+0x1b>
  400f3c:	48 83 c4 28          	add    $0x28,%rsp
  400f40:	5b                   	pop    %rbx
  400f41:	5d                   	pop    %rbp
  400f42:	c3                   	retq   
```
经过```read_six_numbers```函数后，输入字符串中的6位数字被输入到rsp中。
首先检查，运算```read_six_numbers```函数前，运行```read_six_numbers```函数后，rsp处数值变为输入字符串中数字:
```
(gdb) ni
// 运行前堆栈
0x0000000000400f05 in phase_2 ()
4: /x $rsp = 0x7fffffffe360
3: /x $rsi = 0x7fffffffe360
2: /x $rdi = 0x6037d0
1: x/i $pc
=> 0x400f05 <phase_2+9>:        callq  0x40145c <read_six_numbers>
(gdb) x/32xw 0x7fffffffe360
0x7fffffffe360: 0x00000000      0x00000000      0x006037d0      0x00000000
0x7fffffffe370: 0x00000000      0x00000000      0x00401431      0x00000000
0x7fffffffe380: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe390: 0x00000000      0x00000000      0x00400e5b      0x00000000
0x7fffffffe3a0: 0x00000000      0x00000000      0xf7a2f505      0x00007fff
0x7fffffffe3b0: 0x00000000      0x00000020      0xffffe488      0x00007fff
0x7fffffffe3c0: 0x00000000      0x00000001      0x00400da0      0x00000000
0x7fffffffe3d0: 0x00000000      0x00000000      0xf76cb715      0x8caa9c59

// 运行后堆栈
0x0000000000400f0a in phase_2 ()
4: /x $rsp = 0x7fffffffe360
3: /x $rsi = 0x0
2: /x $rdi = 0x7fffffffdd40
1: x/i $pc
=> 0x400f0a <phase_2+14>:       cmpl   $0x1,(%rsp)
(gdb) x/32xw 0x7fffffffe360
0x7fffffffe360: 0x00000001      0x00000002      0x00000003      0x00000004
0x7fffffffe370: 0x00000005      0x00000006      0x00401431      0x00000000
0x7fffffffe380: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe390: 0x00000000      0x00000000      0x00400e5b      0x00000000
0x7fffffffe3a0: 0x00000000      0x00000000      0xf7a2f505      0x00007fff
0x7fffffffe3b0: 0x00000000      0x00000020      0xffffe488      0x00007fff
0x7fffffffe3c0: 0x00000000      0x00000001      0x00400da0      0x00000000
0x7fffffffe3d0: 0x00000000      0x00000000      0xf76cb715      0x8caa9c59
```

现在rsp处代码就是我们输入字符处中的六位数字，之后我们继续进行实验过程。

首先，会先比较前面第一个数值的值，比较第一个代码是否为1，不为1则引爆炸弹:
```
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)
  400f0e:	74 20                	je     400f30 <phase_2+0x34>
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
```
这里可以将代码翻译一下，会发现关键点在于下面这个小循环
```
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax
  400f1a:	01 c0                	add    %eax,%eax
  400f1c:	39 03                	cmp    %eax,(%rbx)
  400f1e:	74 05                	je     400f25 <phase_2+0x29>
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb>
  400f25:	48 83 c3 04          	add    $0x4,%rbx
  400f29:	48 39 eb             	cmp    %rbp,%rbx
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>
```
其中rbx和rbp的初始值，分别被赋值为
```
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp
```
也就是，每次这个循环会去input_string中下个数和前一个数进行对比，下一个数字需要是前一个数字的两倍。通过第一个数字为1，我们可以推断phase_2的数字序列为

```
1 2 4 8 16 32
```

这样，第二关就通过了ヾ(◍°∇°◍)ﾉﾞ
```
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Border relations with Canada have never been better.
Phase 1 defused. How about the next one?
1 2 4 8 16 32
That's number 2.  Keep going!
```

### phase_3
phase_3的代码如下
```
0000000000400f43 <phase_3>:
  400f43:	48 83 ec 18          	sub    $0x18,%rsp
  400f47:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  400f4c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  400f51:	be cf 25 40 00       	mov    $0x4025cf,%esi
  400f56:	b8 00 00 00 00       	mov    $0x0,%eax
  400f5b:	e8 90 fc ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  400f60:	83 f8 01             	cmp    $0x1,%eax
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>
  400f65:	e8 d0 04 00 00       	callq  40143a <explode_bomb>
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a>
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8)
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
  400f83:	b8 c3 02 00 00       	mov    $0x2c3,%eax
  400f88:	eb 34                	jmp    400fbe <phase_3+0x7b>
  400f8a:	b8 00 01 00 00       	mov    $0x100,%eax
  400f8f:	eb 2d                	jmp    400fbe <phase_3+0x7b>
  400f91:	b8 85 01 00 00       	mov    $0x185,%eax
  400f96:	eb 26                	jmp    400fbe <phase_3+0x7b>
  400f98:	b8 ce 00 00 00       	mov    $0xce,%eax
  400f9d:	eb 1f                	jmp    400fbe <phase_3+0x7b>
  400f9f:	b8 aa 02 00 00       	mov    $0x2aa,%eax
  400fa4:	eb 18                	jmp    400fbe <phase_3+0x7b>
  400fa6:	b8 47 01 00 00       	mov    $0x147,%eax
  400fab:	eb 11                	jmp    400fbe <phase_3+0x7b>
  400fad:	e8 88 04 00 00       	callq  40143a <explode_bomb>
  400fb2:	b8 00 00 00 00       	mov    $0x0,%eax
  400fb7:	eb 05                	jmp    400fbe <phase_3+0x7b>
  400fb9:	b8 37 01 00 00       	mov    $0x137,%eax
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>
  400fc4:	e8 71 04 00 00       	callq  40143a <explode_bomb>
  400fc9:	48 83 c4 18          	add    $0x18,%rsp
  400fcd:	c3                   	retq    
```
在调用sscanf函数前我们先检查下rdi,rsi的值,rdi处的值为我们输入的字符串，rsi处的值sscanf中的格式化字符串"%d %d";
```
(gdb) ni
0x0000000000400f5b in phase_3 ()
4: /x $rsp = 0x7fffffffe380
3: /x $rsi = 0x4025cf
2: /x $rdi = 0x603820
1: x/i $pc
=> 0x400f5b <phase_3+24>:       callq  0x400bf0 <__isoc99_sscanf@plt>
(gdb) x/1sb 0x4025cf
0x4025cf:       "%d %d"
(gdb) x/1sb 0x603820
0x603820 <input_strings+160>:   "1 2"
```
调用后，发现sscanf获得的值分别被存储在rsp + 8和rsp + c处
```
(gdb) ni
0x0000000000400f60 in phase_3 ()
4: /x $rsp = 0x7fffffffe380
3: /x $rsi = 0x0
2: /x $rdi = 0x7fffffffdd80
1: x/i $pc
=> 0x400f60 <phase_3+29>:       cmp    $0x1,%eax

(gdb) x/32xw 0x7fffffffe380
0x7fffffffe380: 0x00000000      0x00000000      0x00000001      0x00000002
0x7fffffffe390: 0xffffe480      0x00007fff      0x00400e77      0x00000000
0x7fffffffe3a0: 0x00000000      0x00000000      0xf7a2f505      0x00007fff
0x7fffffffe3b0: 0x00000000      0x00000020      0xffffe488      0x00007fff
0x7fffffffe3c0: 0x00000000      0x00000001      0x00400da0      0x00000000
0x7fffffffe3d0: 0x00000000      0x00000000      0x5741ef47      0x355949b9
0x7fffffffe3e0: 0x00400c90      0x00000000      0xffffe480      0x00007fff
0x7fffffffe3f0: 0x00000000      0x00000000      0x00000000      0x00000000
(gdb) x/xw 0x7fffffffe388
0x7fffffffe388: 0x00000001
(gdb) x/xw 0x7fffffffe38c
0x7fffffffe38c: 0x00000002
```
然后会进行一些比较，输入的数字需要大于1个;第一个数字的值需要小于7，否则都会导致炸弹被引爆。
```
  400f60:	83 f8 01             	cmp    $0x1,%eax
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>
  400f65:	e8 d0 04 00 00       	callq  40143a <explode_bomb>
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a>
  ...
  400fad:	e8 88 04 00 00       	callq  40143a <explode_bomb>
```
然后第一个输入的数字会作为，这条跳转指令中的参数:
```
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8)
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
```
这条跳转指令，其实很有迷惑性，不是往0x402470处跳转，而是取出0x402370处的值和rax的值进行地址运算。
所以我们先检查0x402370处的值，如下:
```
(gdb) x/xw 0x402470
0x402470:       0x00400f7c
```
然后，这里笔者取了一个巧，令第一个数字为0的话，我们就可以直接跳转到0x00400f7c处,也就是这条指令的下一条
```
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
```
eax寄存器会被置为0，并跳转到0x400fbe处。这里需要设置(rsp + 0xc)中的值和eax的值一样才不会引爆炸弹，之前的操作中我们已经将eax的值置换为0xcf,所以这里我们需要把第二个数输入为0xcf(也就是207)。
```
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>
  400fc4:	e8 71 04 00 00       	callq  40143a <explode_bomb>
  400fc9:	48 83 c4 18          	add    $0x18,%rsp
  400fcd:	c3                   	retq   
```

总的来说，输入的第一个数决定偏移，输入的第二个数决定内容
(这里可以看出，可以取多个选择，这里我们取最简单的第一个数为0，第二个数选取0x18的情况;别的情况，大家也可以自己试试看)

phase_3的答案就是
```
0 207
```
这样，第三关也通过了ヾ(◍°∇°◍)ﾉﾞ
```
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Border relations with Canada have never been better.
Phase 1 defused. How about the next one?
1 2 4 8 16 32
That's number 2.  Keep going!
0 207
Halfway there!
```