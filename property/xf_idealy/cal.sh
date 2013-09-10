#!/bin/bash
flst=`ls *.csv`
rm res
for f in $flst
do
    cat ${f::-4}.mic > tp
    cat par >> tp
    ./main $f < tp > ${f::-4}.res  
    echo $f is finished!
    rm tp
done

