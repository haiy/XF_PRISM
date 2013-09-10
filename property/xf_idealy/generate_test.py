

#!/usr/bin/python
#Authot:haiy
#Mon Sep  9 18:30:46 CST 2013

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



func_list=[linear_fun,non_linear,fourior_fun,sin_fun,cube_fun,norm_fun]
func_nls=["Linear","Non-linear","Fourier","Sin","Cube","Normal"]
func_pair=zip(func_list,func_nls)

if __name__=="__main__":
    rand=random.randint
    for p in range(0,10):
        f=open("test_"+str(p)+".csv",'w')
        ly=[rand(0,100) for x in range(0,500)]
        n1=int(float(p)/10*500)
        for line in range(0,n1):
            lx=[]
            lx=[rand(0,100) for x in range(0,13) ]
            lx=[str(x) for x in lx]
            f.write(",".join(lx)+"\n")
        for line in range(n1,500):
            lx=[] 
            for i in range(0,len(func_list)):
                lx.append(func_list[i](ly[line]))
            for x in range(0,6):
                lx.append(rand(0,100))
            lx=[str(x) for x in lx]
            lx.append(str(ly[line]))
            f.write(",".join(lx)+"\n")
        f.close()




