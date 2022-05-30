# 原子锁(atomic)

Linux2.6.18之后，系统便删除了<asm/atomic.h>和<asm/bitops.h>，<alsa/iatomic.h>，  在Linux操作系统下GCC提供了内置的原子操作函数__sync_*，更方便程序员调用。    
现在atomic.h在Linux的内核头文件中，即便能搜索到，但依旧不在gcc默认搜索路径下  (/usr/include，/usr/local/include，/usr/lib/gcc-lib/i386-linux/x.xx.x/include)，    
即使像下面这样强行指定路径，还是会出现编译错误。  
```
#include</usr/src/linux-headers-4.4.0-98/include/asm-generic/atomic.h>   
或在编译时提供编译路径  
 -I /usr/src/linux-headers-4.4.0-98/include/asm-generic依旧会出现问题  
```


gcc从4.1.2提供了__sync_*系列的built-in函数，用于提供加减和逻辑运算的原子操作。  
可以对1,2,4或8字节长度的数值类型或指针进行原子操作，其声明如下：  
```
type __sync_fetch_and_add (type *ptr, type value, ...)
type __sync_fetch_and_sub (type *ptr, type value, ...)
type __sync_fetch_and_or (type *ptr, type value, ...)
type __sync_fetch_and_and (type *ptr, type value, ...)
type __sync_fetch_and_xor (type *ptr, type value, ...)
type __sync_fetch_and_nand (type *ptr, type value, ...)

type __sync_add_and_fetch (type *ptr, type value, ...)
type __sync_sub_and_fetch (type *ptr, type value, ...)
type __sync_or_and_fetch (type *ptr, type value, ...)
type __sync_and_and_fetch (type *ptr, type value, ...)
type __sync_xor_and_fetch (type *ptr, type value, ...)
type __sync_nand_and_fetch (type *ptr, type value, ...)
```
可以直接使用以上函数即可。



如果依然想保持atomic_*函数方式，可以定义如下宏进行替代：  
```
#define atomic_inc(x) __sync_fetch_and_add((x),1)  
#define atomic_dec(x) __sync_fetch_and_sub((x),1)  
#define atomic_add(x,y) __sync_fetch_and_add((x),(y))  
#define atomic_sub(x,y) __sync_fetch_and_sub((x),(y))  
```

