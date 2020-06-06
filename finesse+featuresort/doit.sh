#!/bin/bash

#使用 . 号来引用test1.sh 文件

./main -o 1gwebfinessesorttest -a 1gwebfinessesorttestinfo web
echo "webdone!"
./main -o 1gvmfinessesorttest -a 1gvmfinessesorttestinfo vm
echo "vmdone!"
./main -o 1gmailfinessesorttest -a 1gexchangefinessesorttestinfo exchange
echo "maildone!"
./main -o 1gwikifinessesorttest -a 1gwikifinessesorttestinfo wiki
echo "wikidone!"
# 或者使用以下包含文件代码
# source ./test1.sh
