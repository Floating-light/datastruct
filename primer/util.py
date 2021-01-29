
import os
import sys
import re
re_digits = re.compile(r'(\d+)')
def embedded_numbers(s):
    pieces = re_digits.split(s)  
    # print(pieces)               # 切成数字和非数字
    pieces[1::2] = map(int, pieces[1::2])       # 将数字部分转成整数
    print(pieces)
    return pieces[1::2]
def rename():
    path=input("请输入路径(例如D:\\\\picture)：")
    name=input("请输入开头名:")
    startNumber= 0
    fileType=".png"
    # print("正在生成以" + name + startNumber + fileType+"迭代的文件名")
    count=0
    filelist=os.listdir(path)
    # 按数字大小排序
    filelist = sorted(filelist, key=embedded_numbers)
    print(filelist)

    for files in filelist:
        if files == "Thumbs.db":
            continue
        Olddir=os.path.join(path,files)
        if os.path.isdir(Olddir):
            continue
        Newdir=os.path.join(path,name+str(count+int(startNumber)).zfill(4)+fileType)
        os.rename(Olddir,Newdir)
        count+=1
    print("一共修改了"+str(count)+"个文件")

rename() 