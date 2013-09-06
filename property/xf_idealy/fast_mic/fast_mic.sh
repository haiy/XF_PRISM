#!/bin/bash
#Update:Fri Sep  6 19:23:27 CST 2013
#Author:haiy
#usage: ./fast_mic.sh file_name expected_splits exp f


fnm=$1
afn="act_"$fnm
spn=$2
exp=$3
f=$4

#remove the blank lines
sed '/^$/d' $fnm > $afn

#generate the split positon file
python split_result.py $fnm $spn > sp_res.txt

#build the directory that store the partitions of MIC
#if the dir is exists already then remove it,else build it
if [ ! -d $fnm"_mic" ]; then
    mkdir $fnm"_mic"
else
    rm $fnm"_mic"/*.mic
fi

#read the split position from the split file
while read line;
do
    #echo $line
    out_name=$( echo $line | sed 's/[[:space:]]/_/g' )
    out_name=$out_name.mic
    ./mic_set $afn $line $exp $f > $fnm"_mic"/$out_name  &
done < sp_res.txt

echo "Here we go !!"
tn=$(sed -n $spn"p" sp_res.txt | awk '{print $2}')
echo There should have $tn lines.
cd $fnm"_mic"
tcn=$(wc -l *.mic | grep "total" | awk '{print $1}' )

#recursively check if all the tasks are done
echo "Running..."
while [ ! $tn -eq $tcn ]; do
    tcn=$(wc -l *.mic | grep "total" | awk '{print $1}' )
    #echo "Now $tcn lines generated!"
done
echo "I'm done!"

mv ../sp_res.txt .
mv ../$afn .

flst=$(ls *.mic | sort)
for i in $flst;
do
    cat $i >> ${fnm::-4}.mic
done

#merge all the partitions together
cat ${fnm::-4}.mic | awk '{ print length, $0 }' | sort -n | awk '{$1=""; print $0}' > tmp.mic

mv tmp.mic ${fnm::-4}.mic

ln=$(wc -l ${fnm::-4}.mic | awk '{print $1}')
echo "${fnm::-4}.mic have $ln lines"
mv ${fnm::-4}.mic ..
cd ..
rm -rf $fnm"_mic"





