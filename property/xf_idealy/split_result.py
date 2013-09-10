#-*- coding: utf-8 -*-
import sys
from sys import argv

#共两个参数，参数1为总行数，参数2为要分割的区间数目
f=open(sys.argv[1])
n=int( len(f.readline().split(",")) )
f.close()

t=int(sys.argv[2])

s=int(round((n**2-n)/2.0))
#此处的ave为第一次的期望值
ave=s/t					
num=1
#l列表为每一行所能容纳的点数值
l=range(0,n)				
p=0
result=[]
#range中为行号
for i in range(1,n+1):			
	if abs(l[i]) > 2*abs(ave-p):
		#此处m为，要用于分割的行号(即：区间内的最后一行)
		m=i
		result.append(m)
		for ls in range(1,m+1):
			if (ls-1)%2 == 0:
				N=ls*(ls-1)/2
			else:
				N=ls*(ls-2)/2+ls/2
		#此处的ave为除第一个区间以外的其它区间的期望值
		ave=int(round((s-N)/(float(t)-num)))
		num += 1
		#在此处将新的开始区间初始化为0.
		p=0			
	 	if num == t:
			break
	p=l[i]+p
result.append(n)
result=[x-1 for x in result]
#for k in result:
#	print k,
#print
#print the split section
#the index begin from 0 but the valid output from 1
print 1,result[0]
for k in range(0,t-1):
        print result[k]+1,result[k+1]
        


