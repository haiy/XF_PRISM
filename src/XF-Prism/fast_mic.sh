#!/bin/bash
fnm=$1
afn="act_"$fnm
spn=$2
exp=$3
f=$4

sed '/^$/d' $fnm > $afn
python split_result.py $fnm $spn > sp_res.txt

if [ ! -d $fnm"_mic" ]; then
    mkdir $fnm"_mic"
else
    rm $fnm"_mic"/*.mic
fi

while read line;
do
    echo $line
    out_name=$( echo $line | sed 's/[[:space:]]/_/g' )
    out_name=$out_name.mic
    ./mic_set $afn $line $exp $f > $fnm"_mic"/$out_name  &
done < sp_res.txt

echo "Here we go !!"
tn=$(sed -n $spn"p" sp_res.txt | awk '{print $2}')
echo There should have $tn lines.
cd $fnm"_mic"
tcn=$(wc -l *.mic | grep "total" | awk '{print $1}' )
while [ ! $tn -eq $tcn ]; do
    tcn=$(wc -l *.mic | grep "total" | awk '{print $1}' )
    echo "Now $tcn lines generated!"
done

mv ../sp_res.txt .
mv ../$afn .

flst=$(ls *.mic | sort)
for i in $flst;
do
    cat $i >> ../${fnm::-4}.mic
done
cd ..
ln=$(wc -l ${fnm::-4}.mic | awk '{print $1}')
echo "${fnm::-4}.mic have $ln lines"

    





