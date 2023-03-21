
// 核心API
/*
rcu_read_lock()
rcu_read_unlock()
synchronize_rcu()/call_rcu()
rcu_assign_pointer()
rcu_dereference()
*/

/*
 适用多读少写
 写先复制副本，在副本更改，更改完成替换原指针到副本上
*/
