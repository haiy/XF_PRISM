#!/bin/bash
#date:Tue Sep 10 13:54:33 CST 2013
#author:haiy

flst=`ls *.csv| sort`
for fl in $flst
do
    ./fast_mic.sh $fl 3 0.7 15
done
