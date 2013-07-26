#!/usr/bin/python
#date:Thu Apr 11 17:58:39 CST 2013
#author:haiyfu
#e-mail:haiyangfu512@gmail.com
#description:
"""
    read_info will operate with two lines each time
    write_result will write the information contained
    in the structure data info and output the results
    to a new file with the same name and different 
    suffix.
"""
#change log:
#Sun Apr 14 10:27:25 CST 2013
#add lines 39-40 ,44-46
import os.path
from sys import argv
#pos denotes the position where to begin to read
def read_info(pos,info_file):
    fptr=open(info_file)
    result=[]
    fptr.seek(pos)
    #read into the first line of the current pos
    name=fptr.readline().strip("\n ,").split()[0]
    result.append(name)
    line=fptr.readline().strip("\n ,").split(",")
    line=[int(x.split(":")[0]) for x in line]
    line.sort()
    result.append(line)
    result.append(fptr.tell())
    fptr.close()
    return result
    
#info is a list like [["GA"],[1,3,5,10]]
def write_result(src_file,info):
    src=open(src_file)
    dst_name=info[0]+"_"+os.path.basename(src_file)
    dst=open(dst_name,"w")
    ln=len(src.readline().split(","))
    src.seek(0,0);
    for l in src:
        l=l.strip("\n").split(",")
        line=[]
        if len(l) != ln:
            print "Not equal elements line."
            break;
        for x in info[1]:
            line.append(l[x])
        line.append(l[-1])
        line=",".join(line)+"\n"
        dst.write(line)
    src.close()
    dst.close()

if __name__=="__main__":
    script,info_file,src_file=argv
    info=read_info(0,info_file)
    write_result(src_file,info)
    info=read_info(info[2],info_file)
    write_result(src_file,info)
    info=read_info(info[2],info_file)
    write_result(src_file,info)








