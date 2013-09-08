#!/bin/bash
#Date:Fri Sep  6 19:26:31 CST 2013
#Author:haiy

fls=`ls input_dir/`
par=`pwd`
src="/input_dir/"
dst="/fast_mic/"

mkdir output
cd fast_mic
for src_file in $fls
do
    echo "Now $src_file!"
#    echo $par$src$src_file,$par$dst
    cp $par$src$src_file $par$dst
#    ./fast_mic.sh $src_file 3 0.6 15
#    mv ${src_file::-4}.mic ../output
    rm $src_file
done 


