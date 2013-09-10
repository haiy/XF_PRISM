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



func_list=[linear_fun,non_linear,fourior_fun,sin_fun,cube_fun,norm_fun]
func_nls=["Linear","Non-linear","Fourier","Sin","Cube","Normal"]
func_pair=zip(func_list,func_nls)

def pure_func(filename,rand,p=0):
    f=open(filename,'w')
    cn=0
    for line in range(0,500):
        lx=[rand(0,100) for x in range(0,12)]
        y=0
        if cn<int(500*p):
            y=rand(0,100)
            cn=cn+1
        else:
            for x in range(0,len(func_list)):
                y=y+func_list[x](lx[x])
        lx=[str(x) for x in lx ]
        f.write(",".join(lx)+','+str(y)+'\n')
    f.close()


def mix_func(filename,p=0):
    f=open(filename,'w')
    rand=[random.choice([random.randint,random.uniform]) for x in range(0,12)]
    cn=0
    for line in range(0,500):
        lx=[rand[x](0,100) for x in range(0,12)]
        y=0
        if cn<int(500*p):
            y=random.uniform(0,100)
            cn=cn+1
        else:
            for x in range(0,len(func_list)):
                y=y+func_list[x](lx[x])
        lx=[str(x) for x in lx ]
        f.write(",".join(lx)+','+str(y)+'\n')
    f.close()

if __name__=="__main__":
    rand_pair=[[random.randint,"Dis"],[random.uniform,"Con"]]
    for rand in rand_pair:
        p=0
        for x in range(0,10):
            p=float(x)/10
            pure_func(rand[1]+'_'+str(p)+".csv",rand[0],p)
    p=0
    for x in range(0,10):
        p=float(x)/10
        mix_func("Mix_"+str(p)+".csv",p)


