/*
Author:haiyfu
date:Wed Apr 24 19:14:52 CST 2013
contact:haiyangfu512@gmail.com
*/
#include "main.h"

int main(int argc,char *argv[])
{
    /* 
       参数说明：数据文件名 起始列 终止列 exp f
       注：exp和f为mic参数
     */
    cal_mic_section(argv[1],atoi(argv[2]),atoi(argv[3]),atof(argv[4]),atof(argv[5]));
    return 0;
}
