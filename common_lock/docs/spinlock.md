# 自旋锁（spinlock）

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

