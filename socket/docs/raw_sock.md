# raw socket 和 socket

原始套接字创建方式跟TCP/UDP创建方式类似

最大区别：  
    1） raw socket可以拿到整个报文数据，发送也是整个报文，从以太层开始到应用数据；  
    2） socket 只能操作传输层之上的数据；
