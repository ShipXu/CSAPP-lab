## Problem2
弄清楚,这两条命令所对应的装载和存储命令，对数据的分析是很关键的;
这里
```
tmp = A[i][j];
B[j][i] = tmp;
```

```
(gdb) ni
<!-- tmp = A[i][j]; -->
=> 0x400e22 <transpose_submit+140>:     mov    -0x14(%rbp),%eax
=> 0x400e25 <transpose_submit+143>:     movslq %eax,%rdx
=> 0x400e28 <transpose_submit+146>:     movslq %ebx,%rax
=> 0x400e2b <transpose_submit+149>:     imul   %rdx,%rax
=> 0x400e2f <transpose_submit+153>:     lea    0x0(,%rax,4),%rdx
=> 0x400e37 <transpose_submit+161>:     mov    -0x40(%rbp),%rax
=> 0x400e3b <transpose_submit+165>:     add    %rax,%rdx
=> 0x400e3e <transpose_submit+168>:     mov    -0x18(%rbp),%eax
=> 0x400e41 <transpose_submit+171>:     cltq   
=> 0x400e43 <transpose_submit+173>:     mov    (%rdx,%rax,4),%eax
=> 0x400e46 <transpose_submit+176>:     mov    %eax,-0x2c(%rbp)
```
```
(gdb) ni
<!-- B[j][i] = tmp; -->
=> 0x400e4c <transpose_submit+182>:     movslq %eax,%rdx
=> 0x400e4f <transpose_submit+185>:     movslq %r12d,%rax
=> 0x400e52 <transpose_submit+188>:     imul   %rdx,%rax
=> 0x400e56 <transpose_submit+192>:     lea    0x0(,%rax,4),%rdx
=> 0x400e5e <transpose_submit+200>:     mov    -0x48(%rbp),%rax
=> 0x400e62 <transpose_submit+204>:     lea    (%rdx,%rax,1),%rcx
=> 0x400e66 <transpose_submit+208>:     mov    -0x14(%rbp),%eax
=> 0x400e69 <transpose_submit+211>:     cltq   
=> 0x400e6b <transpose_submit+213>:     mov    -0x2c(%rbp),%edx
=> 0x400e6e <transpose_submit+216>:     mov    %edx,(%rcx,%rax,4)
```
通过对汇编语言的分析，我们可以发现```tmp = A[i][j]```中包含两条load指令和一条store指令;
```B[j][i] = tmp```中包含两条load指令和一条store指令;

blocking的size设置为不同的值的时候
```
// 16
Function 2 (4 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 2 (block scan transpose): hits:870, misses:1183, evictions:1151
```

```
// 8
Function 2 (4 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 2 (block scan transpose): hits:1710, misses:343, evictions:311
```

```
// 4
Function 2 (4 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 2 (block scan transpose): hits:1566, misses:487, evictions:455
```

```
// 2
Function 2 (4 total)
Step 1: Validating and generating memory traces
Step 2: Evaluating performance (s=5, E=1, b=5)
func 2 (block scan transpose): hits:1326, misses:727, evictions:695
```