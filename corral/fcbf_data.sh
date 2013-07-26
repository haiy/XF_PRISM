#!/bin/bash
#author:haiy
#date:Sun Apr 14 16:23:16 CST 2013
#contact:haiyangfu512@gmail.com

#fcbf data file should be in such format:
#1.every attribute is nominal
#2.there is none characters
#3.a *.data file should be companied with a
#  *.rc file.

#The first argument is the *.data file name
DELTA=$1
nm=`ls *.data`
echo $nm
if [ "$nm" = "" ]; then
    echo "File found Error. *.data file needed."
#    echo "Usage:bash script_name input_file.data"
    exit
fi

fnsrc=${nm::-5}


#How to del a certain line sed "1d"
#Generate the *.rc file
echo Begin...

#if the .rc file not exits,then generate it from 
#.data file
if [ ! -f $fnsrc.rc ]; then
    python rc_generator.py $fnsrc.data $fnsrc.rc
fi

#FCBF feature selection
./FCBFselector $fnsrc $DELTA| grep "[[:digit:]]" > $fnsrc.fcbf

#extract the selected features from the source file.
python fcbf_extract_sf.py $fnsrc.fcbf $fnsrc.csv
echo Finished Successfully!






