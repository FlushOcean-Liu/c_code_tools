
有名管道，通过/tmp/my_fifo传输

write_fifo.cpp
编译： g++ write_fifo.cpp -o write

read_fifo.cpp 
编译： g++ read_fifo.cpp -o read

分别运行write，read，如果my_fifo没有创建，先运行write