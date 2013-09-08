#!/bin/bash
#Author:haiy
#Date:Sun Sep  8 16:13:54 CST 2013
#这个程序是从计算好的MIC矩阵中提取第11行。
flst=`ls *.mic | sort`
rm mic_rank
for f in $flst
do
    echo now $f !
    echo $f >> mic_rank
    sed -n -e 11p $f >> mic_rank
done
