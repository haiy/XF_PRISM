#!/usr/bin/python
#Wed Sep  4 00:58:46 CST 2013
#Authot:haiy

import math
import random
import subprocess
import numpy as np


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
def init_x(func,lx):
    yx=[]
    yx=[func(x) for x in lx]
    pairs=zip(lx,yx)
    fp=open("tmp.txt",'w')
    for x in pairs:
        fp.write(str(x[0])+str(',')+str(x[1])+str('\n'))
    fp.close()
    mic=subprocess.check_output(['./mic_set','tmp.txt','0','1','0.7','15'])
    subprocess.check_output(['rm','tmp.txt'])
    mic=mic.strip("\n")
    return mic

"""
    Use various function to generate the f(lx) pairs ,and store
    the MIC of each pair to  list "new".
"""
def funcs_mic(lx,func_list):
    new=[]
    for func in func_list:
        new.append(float(init_x(func,lx)))
    return new


func_list=[linear_fun,non_linear,fourior_fun,sin_fun,cube_fun,norm_fun]

"""
    The main function will calculate the average and standard
    devitation of mic for each function.Every column represents
    the correspond mic values.
"""
if __name__=="__main__":
    sum_mic_int=[]
    sum_mic_float=[]
    """
        Simulate each function  N times
    """
    N=10
    for x in range(0,N):
        lx_int=[random.randint(0,100) for x in range(0,500)]
        lx_float=[random.uniform(0,100) for x in range(0,500)]
        sum_mic_int.append(funcs_mic(lx_int,func_list))
        sum_mic_float.append(funcs_mic(lx_float,func_list))
    mic_int=np.array(sum_mic_int)
    print "INT\n",mic_int
    mic_float=np.array(sum_mic_float)
    int_mean=np.mean(mic_int,axis=0)
    int_stdv=np.std(sum_mic_int,axis=0)
    print "X_INT\n",int_mean,"\n",int_stdv

    print "FLT\n",mic_float
    float_mean=np.mean(mic_float,axis=0)
    float_stdv=np.std(sum_mic_float,axis=0)
    print "X_FLT\n",float_mean,"\n",float_stdv


