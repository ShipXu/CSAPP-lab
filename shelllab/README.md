首先，我们需要复习下CSAPP课堂中的全部代码，这样你做起shelllab才会比较顺利。

用了wait就会阻塞到前台程序结束，在这个实验中显然不可取:
```
while (pid == fg)
```

做不出来，直接抄了，呜呜呜T_T