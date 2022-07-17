# 描述Linux系统函数使用
* rename
* mkdir
* popen
* system


## rename使用注意事项

在linux上c编程使用的rename函数，不能跨文件分区移动文件。

## popen说明

popen不阻塞，不会等等子进程的结束，能够返回执行的结果；

## system说明

system是阻塞的，会自动对进程进行管理，不会返回执行结果，但是会返回是否执行成功；


```c
int system(const char * cmdstring) 
{
    pid_t pid;
    int status;
    if(cmdstring == NULL) {
        return (1);         /* 如何cmdstring为空，返回非零值，一般为1 */
    }
    if((pid = fork())<0) {
        status = -1;        /* fork失败，返回-1 */
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);         /* exec 执行失败返回127，注意exec只在失败时
                             * 才返回现在的进程，成功的话现在进程就不存在 
                             */
    } else {                /* 父进程 */
        while(waitpid(pid, &status, 0) < 0) {
            if(errno != EINTR) {
                status = -1; /* 如果waitpid被信号中断，则返回-1 */
                break;
            }
        }
    }
    return status;           /* 如果waitpid成功，则返回子进程的返回状态  */
}

```

## popen和system简单区别
popen可以返回脚本命令执行的结果，但是不知道命令执行成功与否；  
system可以获取脚本执行成功与否，不能获取脚本执行结果。
