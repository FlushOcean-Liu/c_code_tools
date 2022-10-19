import email
import os
import sys


#获取eml附件信息

def add(a, b, c):
    print("in python function add")
    print("a=",str(a))
    print("b=",str(b))
    print("ret=",str(a+b))
    print("arg3 is :",c);
    return c 


def get_mail_message(file_path, annex_path, mail_data):
    attach_set=""
    try:
        #fp = open(file_path, 'rb')        #打开任意格式文件，通过email库来判断是否为eml文件
        #msg = email.message_from_binary_file(fp)
        msg = email.message_from_string(mail_data)

        for part in msg.walk():            #循环信件中的每一个mime的数据块
            if part.get_content_maintype() == 'multipart':
                continue

            annex_name = part.get_filename()

            if annex_name:        #如果附件存在名字
                attach_set = attach_set + ","+ annex_name
                fp = open(os.path.join(annex_path, annex_name), 'wb')
                fp.write(part.get_payload(decode=True))
        return attach_set    
    except Exception as e:
        print(e)      
        return "error"


def Get_Annex_Message(FilePath, Annex_Path):
    #global sum
    attach_set=""
    try:
        fp = open(FilePath, 'rb')        #打开任意格式文件，通过email库来判断是否为eml文件
        msg = email.message_from_binary_file(fp)

        for part in msg.walk():            #循环信件中的每一个mime的数据块
            if part.get_content_maintype() == 'multipart':
                continue

            Annex_name = part.get_filename()

            if Annex_name:        #如果附件存在名字
                #Annex_name = str(sum) + '.' + Annex_name
                attach_set = attach_set + ","+ Annex_name
                fp = open(os.path.join(Annex_Path, Annex_name), 'wb')
                fp.write(part.get_payload(decode=True))
                #sum += 1
        return attach_set    
    except Exception as e:
        print(e)      
        return

     

#递归文件夹下所有文件

def List_Filepath(Eml_Path, Annex_Path):
    for parent,dirnames,filenames in os.walk(Eml_Path): #遍历文件夹
        for dirname in dirnames:        #对文件夹进行递归
            List_Filepath(dirname, Annex_Path)
            
        for filename in filenames:        #r对文件进行判断
            FilePath = os.path.join(parent,filename)
            Get_Annex_Message(FilePath, Annex_Path)

     

#创建目的文件夹

def Create_Dir(Annex_Path):
    if os.path.exists(Annex_Path):
        print("dir exists, Annex file will create in %s" % Annex_Path)
    else:
        os.mkdir(Annex_Path)

     

#主函数

def main():
    global sum 
    sum = int(1)

    Eml_Path = sys.argv[1]        #第一个参数为eml所在文件夹
    Annex_Path = sys.argv[2]    #第二个参数为eml附件输出的路径

    Create_Dir(Annex_Path)        #创建保存附加的文件夹

    List_Filepath(Eml_Path, Annex_Path)        


if __name__ == "__main__":
    main()
    #add(3,4)
