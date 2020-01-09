#!/usr/bin/env python3
#-*- coding:utf-8 -*-
import requests
import os
from pyquery import PyQuery as pq
import pandas as pd
from multiprocessing import Pool
 
 
def getHTMLText(url, code="utf-8"):   #�õ�ת�����ÿ��url��Ӧ��HTML����
    while(1):                         #�������Ի��ƣ���ĳ�γ�ʱ��δ����HTML���룻���ظ�����ֱ���õ�HTML����Ϊֹ
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
            
       
 
def parseHTMLText(html):   #������ҳԴ�룬��ȡָ�������ļ����ļ���
    doc=pq(html)           #��pyquery��ȡa��ǩ
    a=doc('a')
    filename='RNG'         #Ĭ���ļ���ΪRNG
    for item in a.items(): #����������ȡ��a��ǩ������href����ֵ���ҵ�ָ���������ļ����ļ���
        if 'protein.gpff.gz' in item.attr('href'):
            filename=item.attr('href')
            print(filename)
    return filename
 
 
def writeFile(basicUrl,filename,filepath,count):
        filepath1='{0}/{1}'.format(filepath,str(count)+'_'+filename)#����д��·��
        print("filepath1 \n",filepath1)
        if not os.path.exists(filepath1):
            with open(filepath1,'wb') as f:
                if filename!='RNG':                #����Ŀ���ļ�
                    while(1):                      #�������Ի��ƣ�ֱ���õ�Ŀ���ļ���д�뱾�سɹ�Ϊֹ
                        try:
                            file=requests.get(basicUrl+filename) #ָ���ļ�����������
                            if file.status_code==200:
                                f.write(file.content)
                                print("write sussess\n")
                                return
                        except:
                            continue
                else:                          #�ļ���ΪRNG˵��������Ŀ���ļ�����ʱ������һ�����ļ����ļ���Ϊ:���_RNG
                    print("write fail\n")
 
 
 
def changePre(data):  #����urlǰ׺Ϊhttps
    hurlsGen=[]       #�������ת����ɵ�Gen FTP����
    hurlsRef=[]       #�������ת����ɵ�Ref FTP����
    i=0
    while i<len(data):  #ѭ�������������ݽ���ת��
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