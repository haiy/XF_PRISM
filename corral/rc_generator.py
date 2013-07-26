#author :haiyfu
#date:April 14
#description:
#contact:haiyangfu512@gmail.com

"""
    This little part is to check how many different values in
    a column and store the unqiue values in a list.
    For FCBF initially.
    The last column is the class .
"""

from sys import argv

#only count the target file and return
#a list structure which contains the detail
#information,like [23, [[1,23],[11,233]], 34 ]
#Here is the correspond meanings
#[attribure_number,[ [first-column-different-values] [2nd-df-val] ],line_num]
def rc_gn(sn):
    fin=open(sn)
    atrn=len(fin.readline().split(","))

    #Initialize the result list
    fin.seek(0,0)
    rc=[]
    rc.append(atrn)
    rc.append([])
    l=fin.readline().strip("\r \n ").split(",")
    for x in l:
        rc[1].append([x])
    count=0
    for l in fin:
        l=l.strip("\n \r").split(",")
        idx=0
        if(len(l)<rc[0]):
            break
        for x in l:
            if x not in rc[1][idx]:
                rc[1][idx].append(x)
                rc[1][idx].sort()
            idx=idx+1
        count=count+1
    #print rc
    rc.append(count+1)
    fin.close()
    return rc

def  wrt_rc(rc,tn):
    #print rc
    ft=open(tn,"w")
    #class info
    ft.write(str(len(rc[1][-1]))+","+",".join(rc[1][-1])+".\n" )
    #attribure number
    ft.write(str( rc[0]-1 )+"\n")
    #every attribure info
    for x in range(rc[0]-1):
        sl="A"+str(x+1)+" - "+"|".join(rc[1][x])+".\n"
        ft.write(sl)
    ft.close()

if __name__=="__main__":
    script_nm,src_file,out_file=argv
    wrt_rc(rc_gn(src_file),out_file)
    
