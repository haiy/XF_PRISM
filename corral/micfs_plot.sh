#!/bin/bash

#author:haiyfu
#date:Wed Apr 17 09:47:22 CST 2013
#contact:haiyangfu512@gmail.com

#description:
#   This bash script will run the all experiments except FCBF
#   it includes "weka-GA-SFFS-DT-NB and micfs-GA-SFFS-FCFS-DT-NB
#   The directory should have files below:
#  
#extract_sf.py
#fast_mic.sh
#fcbf_data.sh
#fcbf_extract_sf.py
#fcbf_plot.sh
#FCBFselector
#lung-cancer.csv
#lung-cancer.data
#main
#micfs_plot.sh
#mic_set
#parameters.file
#rc_generator.py
#README
#split_result.py
#weka.jar
#测试文件夹
            ###########################################################
            #How to run:bash micfs_plot.sh 3 0.6 15 first-last        #
            ###########################################################
#time stamp
flg=`date +%m%d%H%M`
#################################################
#Begin parameters check
src_fnm=`ls *.csv`
if [ ! -f $src_fnm ]; then
    echo "Source CSV File Not Found!"
    echo "Usage: bash script_name spn exp f"
    exit
fi
echo .....
echo Input file is :$src_fnm
echo .....

#numbers of splits 
spn=$1
#mic parameters
exp=$2
f=$3
#set the nominal attributes
nominal=$4
#set the fcbf delta
DELTA=$5
if [ $# -lt 5 ]; then
    echo "Parameters Error!"
    echo "Usage:bash script_name spn exp f nominal_attributes FCBF_DELTA"
    exit
fi
#GA,SFFS,FCFS parameters
config="parameters.file"
#################################################
#check if mic were calculated already and if 
#parameters file is ready.
echo "Check MIC file."
mic_file=${src_fnm::-4}"_"$exp"_"$f".mic"
echo Found $mic_file !
if [ ! -f $mic_file ]; then
    echo "${src_fnm::-4}.mic not found,now calculate it..."
    bash fast_mic.sh $src_fnm $spn $exp $f
    mv ${src_fnm::-4}.mic $mic_file
fi

tpmic=tp$mic_file
if [  -f "$config" ]; then
    echo "$config check"
    cat $mic_file >> $tpmic
    cat $config  >> $tpmic
#    last_line=( $( sed -n "$(wc -l ${src_fnm::-4}.mic | awk '{print $1}')p" ${src_fnm::-4}.mic ) )
#    echo "last line length ${#last_line[@]} ."
#    if [ ${#last_line[@]} -gt 1 ]; then
#        echo "add parameters file to the mic."
#        cat parameters.file >> ${src_fnm::-4}.mic
#    fi
else
    echo "parameters.file not found.exit now"
    exit
fi

#################################################
#the final result file and function variables
fnrst=$flg"_"${src_fnm::-4}_result
if [ -f "$fnrst" ]; then
    rm $fnrst
fi
touch $fnrst

micfs=main
fnm1=$flg"_"${src_fnm::-4 }.micfs
Program="java -cp weka.jar weka.filters.unsupervised.attribute.NumericToNominal -R $nominal"
Program_DT="java -cp weka.jar weka.classifiers.trees.J48 -t"
Program_NB="java -cp weka.jar weka.classifiers.bayes.NaiveBayes -t"
Program_SVM="java -cp $CLASSPATH:weka.jar:libsvm.jar weka.classifiers.functions.LibSVM -S 0 -K 2 -D 3 -G 0.0 -R 0.0 -N 0.5 -M 40.0 -C 1.0 -E 0.001 -P 0.1 -seed 1 -c last -t"

#################################################
#micfs procedure
echo "1->micfs feature selection."
./$micfs $src_fnm < $tpmic > $fnm1
rm $tpmic
#micfs features
echo "2->extract traget features"
python extract_sf.py $fnm1 $src_fnm
lst="GA SFFS FCFS"
for fn in $lst;
do
    org=`ls $fn*.csv`
    echo $org
    chg=$flg"_"$fn"_"$src_fnm
    echo $chg
    mv $org $chg
    tg=${chg::-4}.arff
    echo "2.1->class to  norminal $chg to $tg.."
    $Program -i $chg -o $tg &>/dev/null

    echo "classifiers NB DT SVM..."
    for i in NB DT SVM;
    do
        Nnm=$flg"_"$i"_"$fn"_"$src_fnm
        Nnm=${Nnm::-4}.res
        echo "2.2->NB to : $Nnm ."
        Program_NM=Program_$i
        ${!Program_NM} $tg > $Nnm
        echo mifs_${Nnm::-4} $(cat $Nnm | grep "Classified" | sed 's/[[:alpha:]%]//g') >> $fnrst
    done
done

target_dir=$flg"_mifs"
mkdir $target_dir
mv $flg*.* $target_dir
#################################################


echo "WEKA BEGIN...."
#weka
arf_file=${src_fnm::-4}.arff
$Program -i $src_fnm -o $arf_file &> /dev/null
echo "weka 1->:$src_fnm to $arf_file !"

gafs_out=$flg"_GA_"$arf_file
echo "weka 2->:from $arf_file to $gafs_out."

GA_para=( `sed -n "1p" $config` )
PZ=${GA_para[0]}
PG=${GA_para[1]}
PC=${GA_para[2]}
PM=${GA_para[3]}
echo PZ:$PZ PG:$PG PC:$PC PM:$PM 
if [[ $PZ = "" || $PG = "" || $PC = "" || $PG = "" ]]; then
	echo "Error read parameters.file.Check it."
	exit
fi
java -cp weka.jar weka.filters.supervised.attribute.AttributeSelection -E "weka.attributeSelection.CfsSubsetEval " -S "weka.attributeSelection.GeneticSearch -Z $PZ -G $PG -C $PC -M $PM -R $PG -S 1"  -i $arf_file -c last -o $gafs_out

sffs_out=$flg"_SFFS_"$arf_file
echo "weka 3->:from $arf_file to $sffs_out"
sffs_para=( `sed -n "2p" $config `)
Dn=${sffs_para[0]}

echo $Dn 
java -cp weka.jar weka.filters.supervised.attribute.AttributeSelection -E "weka.attributeSelection.CfsSubsetEval " -S "weka.attributeSelection.LinearForwardSelection -D 1 -N $Dn -I -K 50 -T 0" -i $arf_file -c last -o $sffs_out

for file in $gafs_out $sffs_out;
do
    echo now  $file
    for itm in DT NB SVM;
    do
        nm=$flg"_"$itm"_"${file:9:-6}.res;
        prg=Program_$itm
        echo Program Name  $prg
        ${!prg} $file > $nm;
        echo "weka 4->$prg $file > $nm"
        echo weka_${nm::-4}$( cat $nm | grep "Classified" | sed 's/[[:alpha:]%]//g' ) >> $fnrst
#        python collect.py $nm ${nm::-4}.data
    done
done

tdir=$flg"_"weka
mkdir $tdir
mv $flg*.* $tdir

#################################################
#Begin FCBF 
if [ ! -f "fcbf_data.sh" ]; then
    echo "Shell Script fcbf_data.sh not found!"
    exit
fi
#generate the .rc file
bash fcbf_data.sh $DELTA

src_fnm=`ls FCFS*.csv`
if [ "$src_fnm" = "" ]; then
    echo "ERROR!!FCFS*.csv not found!"
    exit
fi
echo .....
echo Input file is :$src_fnm

arf_file=${src_fnm::-4}.arff
$Program -i $src_fnm -o $arf_file &> /dev/null
echo "FCFS-weka 1->:$src_fnm to $arf_file !"

for itm in DT NB SVM; 
do
    prg=Program_$itm
    echo Program Name  $prg
    nm=$flg"_"$itm"_FCFS.res"
    ${!prg} $arf_file > $nm
    echo "FCFS-weka 4->$prg $file > $nm"
    echo weka_${nm::-4}$( cat $nm | grep "Classified" | sed 's/[[:alpha:]%]//g' ) >> $fnrst
done

mv $flg*.* $tdir
mv *.rc  *.fcbf *.arff FCFS*.csv $tdir

#################################################
#store the parameters
#
echo $1,$2,$3,$4 >> $fnrst
cat $config >> $fnrst

for a in DT NB SVM; do
	for b in GA SFFS FCFS; do
		echo now $a"_"$b----------------------------
		cat $fnrst | grep $a"_"$b | awk '{print $1, $7}' 
		cat $fnrst | grep $a"_"$b | awk '{print $1, $7}' >> $flg"_cmp_result" 
	done
done
tp=`date +%S`
mkdir $flg$tp
mv "$flg"_* $flg$tp

function CHK {
    read -p "Good or Not? [y/n]" label
    if [ "$label" = "y" ]; then
        mv $flg$tp $flg$tp"_Good"
    fi
}
