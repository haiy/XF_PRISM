import glob
import subprocess
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

def prt_subset(lst,fnm):
    f=open(fnm,'a')
    for fts in lst:
        f.write("(")
        for ft in fts:
            f.write(str(ft)+"  ")
        f.write(")")
    f.write("\n")
    f.close()

if __name__=="__main__":
    flst=glob.glob("test_*.res")
    flst.sort()
    GAFS=[]
    SFFS=[]
    FCBF=[]
    for f in flst:
        info_file=f
        info=read_info(0,info_file)
        GAFS.append(info[1])
        info=read_info(info[2],info_file)
        SFFS.append(info[1])
        info=read_info(info[2],info_file)
        FCBF.append(info[1])
    fnm="final_res"
    prt_subset(GAFS,fnm)
    prt_subset(SFFS,fnm)
    prt_subset(FCBF,fnm)









