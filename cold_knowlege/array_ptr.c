#include <stdio.h>

/*
 * 说明：考察变量占用字节，解引用结果
 *
 * array元素大小占2个字节，ptr指向大小未4个字节
 * 所有ptr指向是 0x0011,0x0022
 *
 * 输出应该是：0x220011
 *
 * 编译： gcc array_ptr.c
 * 执行： ./a.out
 *
 */

int main()
{
    unsigned short array[10]={0x11,0x22,0x33,0x44,0x55,0x66};
    unsigned int   *ptr=(unsigned int *)&array[0];

    printf("0x%4x\n",*ptr);

    return 0;
}
