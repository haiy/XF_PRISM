#!/usr/bin/python
#Wed Sep  4 00:58:46 CST 2013
#Authot:haiy

import math
import random
import subprocess

#random.randint(start,stop)
#random.uniform(start,stop)


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
def init_x(func,lx):
    yx=[]
    yx=[func(x) for x in lx]
    pairs=zip(lx,yx)
    fp=open("tmp.txt",'w')
    for x in pairs:
        fp.write(str(x[0])+str(',')+str(x[1])+str('\n'))
    mic=subprocess.check_output(['./mic_set','tmp.txt','0','1','0.7','15'])
    fp.close()
    subprocess.check_output(['rm','tmp.txt'])
    mic=mic.strip("\n")
    return mic
if __name__=="__main__":
    lx_int=[random.randint(0,200) for x in range(0,500)]
    lx_float=[random.uniform(0,150) for x in range(0,500)]
    print "Now" ,lx_int[0:10]
    fun=sin_fun
    res_float=init_x(fun,lx_float)
    res_int=init_x(fun,lx_int)
    print res_int,res_float


