# Linux常用的锁及基本原理

* 互斥锁(mutex)
* 自旋锁(spinlock)
* [原子锁](https://github.com/FlushOcean-Liu/linux_c_summary/blob/master/common_lock/atomic.md)
* 读写自旋锁
* 顺序锁
* 信号量


## 1.互斥锁（mutex）




## 2.自旋锁（spinlock）
自旋锁：连续循环等待，专为防止多处理器并发引入的锁，适用于快速完成的代码

特点：自旋不能抢占，可以中断；

与互斥锁区别：
    对于互斥锁，如果资源已被占用，资源申请者只能进入睡眠状态。
    但是自旋锁不会引起调用者睡眠，如果自旋锁已被别的执行单元保持，调用者就一直循环等待。

spin_lock_init(lock)  
初始化自旋锁，将自旋锁设置为1，表示有一个资源可用。

spin_is_locked(lock)  
如果自旋锁被置为1(未锁)，返回0，否则返回1。

spin_unlock_wait(lock)  
等待直到自旋锁解锁(为1)，返回0；否则返回1。

spin_trylock(lock)  
尝试锁上自旋锁(置0)，如果原来锁的值为1，返回1，否则返回0。


spin_lock(lock)  
循环等待直到自旋锁解锁(置为1)，然后，将⾃旋锁锁上(置为0)。


spin_unlock(lock)  
将自旋锁解锁(置为1)。

spin_lock_irqsave(lock, flags)  
循环等待直到自旋锁解锁(置为1)，然后，将自旋锁锁上(置为0)
。关中断，将状态寄存器值存入flags。


spin_unlock_irqrestore(lock, flags)  
将⾃旋锁解锁(置为1)。开中断，将状态寄存器值从flags存入状态寄存器。

spin_lock_irq(lock)  
循环等待直到自旋锁解锁(置为1)，然后，将自旋锁锁上(置为0)。关中断。

spin_unlock_irq(lock)  
将自旋锁解锁(置为1)。开中断。

spin_unlock_bh(lock)  
将自旋锁解锁(置为1)。开启底半部的执行。

spin_lock_bh(lock)  
循环等待直到自旋锁解锁(置为1)，然后，将自旋锁锁上(置为0)
。阻止软中断的底半部的执行。




在实际编程中，何时使用spin_lock，何时使用spin_lock_irq呢？这两者有点区别。  
(1)spin_lock  
spin_lock 的实现关系为：spin_lock -> raw_spin_lock -> _raw_spin_lock -> __raw_spin_lock ，而__raw_spin_lock 的实现为：  

```c
static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
    preempt_disable();
    spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
    LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
}
(2)spin_lock_irq
spin_lock_irq 的实现关系为：spin_lock_irq -> raw_spin_lock_irq -> _raw_spin_lock_irq -> 
__raw_spin_lock_irq，而
__raw_spin_lock_irq 的实现为：
static inline void __raw_spin_lock_irq(raw_spinlock_t *lock)
{
    local_irq_disable();
    preempt_disable();
    spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
    LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
}
```

由此可见，这两者之间只有一个差别：  
是否调用local_irq_disable()函数， 即是否禁用本地中断。  

这两者的区别可以总结为：  
在任何情况下使用spin_lock_irq都是安全的。因为它既禁用本地中断，又禁止内核抢占。  
spin_lock比spin_lock_irq速度快，但是它并不是任何情况下都是安全的。  


## 3.原子锁（atomic）

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
