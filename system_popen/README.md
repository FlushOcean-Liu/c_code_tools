# popen详解
在C语言中试下shell命令执行函数，popen是不堵塞的，执行结果可以返回到buf中；

# system说明
system是堵塞的，会自动对进制进程管理，system不会返回执行的结果，但是会返回是否执行成功。

```c
int system(const char * cmdstring) {
    pid_t pid;
    int status;
    if(cmdstring == NULL) {
        return (1); //如果cmdstring为空，返回非零值，一般为1
    }
    if((pid = fork())<0) {
        status = -1; //fork失败，返回-1
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127); // exec执行失败返回127，注意exec只在失败时才返回现在的进程，成功的话现在的进程就不存在
    } else { //父进程
        while(waitpid(pid, &status, 0) < 0) {
            if(errno != EINTR) {
                status = -1; //如果waitpid被信号中断，则返回-1
                break;
            }
        }
    }
    return status; //如果waitpid成功，则返回子进程的返回状态
}

```
