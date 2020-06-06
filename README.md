# migrationcompression

五个文件夹分别为研究MC算法过程中使用到的五个实验版本

编译指令：gcc -mcmodel=medium rabin_polynomial_main.c -o main

rabin指纹计算部份使用了https://github.com/joeltucci/rabin-fingerprint-c ，基于rabin指纹完成了MC算法的复现。

使用时指令格式如下：./main -o out -a info in，in为输入文件，info为预处理过程中时间信息，out为输出文件，输出文件为预处理之后的文件，需要使用压缩算法进行压缩后再计算压缩比。
