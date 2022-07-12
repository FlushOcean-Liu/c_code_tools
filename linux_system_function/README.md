# 描述Linux系统函数使用
* rename
* mkdir
* popen
* system


## rename使用注意事项

rename 达到move命令功能，要注意不同磁盘的rename会失败。

## popen说明

ÔÚCÓïÑÔÖÐÊÔÏÂshellÃüÁîÖ´ÐÐº¯Êý£¬popenÊÇ²»¶ÂÈûµÄ£¬Ö´ÐÐ½á¹û¿ÉÒÔ·µ»Øµ½bufÖÐ£»

# system说明
systemÊÇ¶ÂÈûµÄ£¬»á×Ô¶¯¶Ô½øÖÆ½ø³Ì¹ÜÀí£¬system²»»á·µ»ØÖ´ÐÐµÄ½á¹û£¬µ«ÊÇ»á·µ»ØÊÇ·ñÖ´ÐÐ³É¹¦¡£

```c
int system(const char * cmdstring) {
    pid_t pid;
    int status;
    if(cmdstring == NULL) {
        return (1); //Èç¹ûcmdstringÎª¿Õ£¬·µ»Ø·ÇÁãÖµ£¬Ò»°ãÎª1
    }
    if((pid = fork())<0) {
        status = -1; //forkÊ§°Ü£¬·µ»Ø-1
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127); // execÖ´ÐÐÊ§°Ü·µ»Ø127£¬×¢ÒâexecÖ»ÔÚÊ§°ÜÊ±²Å·µ»ØÏÖÔÚµÄ½ø³Ì£¬³É¹¦µÄ»°ÏÖÔÚµÄ½ø³Ì¾Í²»´æÔÚ
    } else { //¸¸½ø³Ì
        while(waitpid(pid, &status, 0) < 0) {
            if(errno != EINTR) {
                status = -1; //Èç¹ûwaitpid±»ÐÅºÅÖÐ¶Ï£¬Ôò·µ»Ø-1
                break;
            }
        }
    }
    return status; //Èç¹ûwaitpid³É¹¦£¬Ôò·µ»Ø×Ó½ø³ÌµÄ·µ»Ø×´Ì¬
}

```

