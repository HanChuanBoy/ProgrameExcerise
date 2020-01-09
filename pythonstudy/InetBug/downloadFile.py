#!/usr/bin/env python3
#-*- coding:utf-8 -*-
import requests
import os
from pyquery import PyQuery as pq
import pandas as pd
from multiprocessing import Pool
 
 
def getHTMLText(url, code="utf-8"):   #得到转换后的每条url对应的HTML代码
    while(1):                         #加入重试机制，若某次超时或未请求到HTML代码；则重复请求，直到拿到HTML代码为止
        try:
            headers={
                    'User-Agent':'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36'

                    }
            r = requests.get(url,headers=headers)
            r.raise_for_status()
            r.encoding = code
            print(r.status_code)
            if r.status_code==200:
                return r.text
        except:
            print ("fail")
            continue
            
       
 
def parseHTMLText(html):   #解析网页源码，提取指定下载文件的文件名
    doc=pq(html)           #用pyquery提取a标签
    a=doc('a')
    filename='RNG'         #默认文件名为RNG
    for item in a.items(): #遍历所有提取的a标签，根据href属性值，找到指定的下载文件的文件名
        if 'protein.gpff.gz' in item.attr('href'):
            filename=item.attr('href')
            print(filename)
    return filename
 
 
def writeFile(basicUrl,filename,filepath,count):
        filepath1='{0}/{1}'.format(filepath,str(count)+'_'+filename)#本地写入路径
        print("filepath1 \n",filepath1)
        if not os.path.exists(filepath1):
            with open(filepath1,'wb') as f:
                if filename!='RNG':                #存在目标文件
                    while(1):                      #加入重试机制，直到拿到目标文件并写入本地成功为止
                        try:
                            file=requests.get(basicUrl+filename) #指定文件的下载链接
                            if file.status_code==200:
                                f.write(file.content)
                                print("write sussess\n")
                                return
                        except:
                            continue
                else:                          #文件名为RNG说明不存在目标文件，此时会生成一个空文件，文件名为:序号_RNG
                    print("write fail\n")
 
 
 
def changePre(data):  #更换url前缀为https
    hurlsGen=[]       #存放所有转换完成的Gen FTP链接
    hurlsRef=[]       #存放所有转换完成的Ref FTP链接
    i=0
    while i<len(data):  #循环遍历所有数据进行转换
        data=data.astype(str)
        furlGen = data['GenBank FTP'][i].strip()[3:]
        furlRef = data['RefSeq FTP'][i].strip()[3:]
        hurlGen = 'https' + furlGen+'/'
        hurlRef = 'https' + furlRef+'/'
        hurlsGen.append(hurlGen)
        hurlsRef.append(hurlRef)
        i+=1
    print("ChangePre Down")
    return hurlsGen,hurlsRef
 
def Write(urls,title):
    count=1
    for url in urls:
        print(url)
        if not os.path.exists(title):
            os.mkdir(title)
        html = getHTMLText(url)
        #print(html)
        filename = parseHTMLText(html)
        filepath = title
        writeFile(url, filename, filepath,count)
        count+=1
'''
def delete_Null(data):
    print(len(data))
    print(data['GenBank FTP'].isnull().value_counts())
    data['GenBank FTP']=data['GenBank FTP'].fillna("-1")
    row1=data[data['GenBank FTP']=="-1"].index.tolist()
    data=data.drop(row1)
    print(len(data))
    print(data['RefSeq FTP'].isnull().value_counts())
    data['RefSeq FTP']=data['RefSeq FTP'].fillna("-1")
    row2=data[data['RefSeq FTP']=="-1"].index.tolist()
    data=data.drop(row2)
    print(len(data))
    return data
def Write(data):
    data.to_csv('prokaryotes_new.csv',index=False,encoding='utf-8')
'''
 
hurlsGen=[]
hurlsReq=[]
 
def main():
    data=pd.DataFrame(pd.read_csv('prokaryotes_new.csv'))
    #data=data.head()
    hurlsGen,hurlsReq=changePre(data)
    #hurlsGen=["https://ftp.ncbi.nlm.nih.gov/genomes/all/GCA/000/504/085/GCA_000504085.1_ASM50408v1"]
    print("hurlsGen \n",hurlsGen)
    print("hurlsReq \n",hurlsReq)
    Write(hurlsGen,'Gen')
    Write(hurlsReq,'Req')
 
if __name__=='__main__':
    main()