#include <stdio.h>
#include <setjmp.h>

jmp_buf my_buff;

int call_longjmp()
{
    /* 返回到setjmp并且设置它的返回值为1 */
    longjmp(my_buff, 1);

    return 0;
}


int main()
{
    if(setjmp(my_buff)==1){
        printf("longjmp here\n");
    }else{
        printf("start to call_longjmp\n");
        call_longjmp();
    }

    return 0;
}

