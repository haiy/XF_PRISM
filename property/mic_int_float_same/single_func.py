#!/usr/bin/python
#Wed Sep  4 00:58:46 CST 2013
#Authot:haiy

import math
import random
import subprocess
import numpy as np

"""
 Below are the test functions to generate
 (x,y)pairs.
"""

def linear_fun(x):
    return x
def sin_fun(x):
    return math.sin(x)
def cube_fun(x):
    return math.pow(x,3)
def non_linear(x):
    return x*math.sin(x)
def fourior_fun(x):
    return math.sin(16*math.pi*x)+x
def norm_fun(x,mu=0,siga=1):
    a=1/(siga*np.sqrt(2*np.pi)) 
    b=-(x-mu)**2/(2*siga**2)
    return a*np.exp(b)


"""
    init_x will firstly invoke the "func" function to generate
    the relevant f(lx) list,and the use mic_set to calculate 
    the MIC of the  pair
"""
def init_x(ly,lx):
    pairs=zip(lx,ly)
    fp=open("tmp.txt",'w')
    for x in pairs:
        fp.write(str(x[0])+str(',')+str(x[1])+str('\n'))
    fp.close()
    mic=subprocess.check_output(['./mic_set','tmp.txt','0','1','0.7','15'])
    subprocess.check_output(['rm','tmp.txt'])
    mic=mic.strip("\n")
    return mic

func_list=[linear_fun,non_linear,fourior_fun,sin_fun,cube_fun,norm_fun]
func_nls=["Linear","Non-linear","Fourier","Sin","Cube","Normal"]
func_pair=zip(func_list,func_nls)

"""
    A sinlge function's various operations.
"""
def single_func(func):
    res_single=[]
    for rand in [[random.randint,"Discrete"],[random.uniform,"Continuous"]]:
        all_mic=[]
        lx=[rand[0](0,100) for x in range(0,500)]
        for p in range(0,100,10):
            p=float(p)/100
            ly=[func[0](x) for x in lx]
            for m in range(0,int(len(lx)*p)):
                ly[m]=rand[0](0,100)
            all_mic.append(str(init_x(lx,ly)))
        all_mic.insert(0,func[1]+rand[1])
        all_mic=",".join(all_mic)
        res_single.append(all_mic)
    return res_single

if __name__=="__main__":
    res=[]
    for func in func_pair:
        res.append(single_func(func))
        print func[1]," finished!"

    f=open("fn_res.csv",'w')
    for x in res:
        for n in x:
            f.write(n+"\n")
    f.close()
