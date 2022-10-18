#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <python3.8/Python.h>


#define BUFF_SIZE  1024


static int get_file_size(FILE *fp)
{
	if(!fp){
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
    int file_size=ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	

	return file_size;
}

static int get_file_data(FILE *fp, char *buffer, int size)
{
	if(!buffer)
	    return -1;
    char tmp[BUFF_SIZE]={0};
    int index=0;
	int len=0;
	while(fgets(tmp,BUFF_SIZE-1,fp)){
        len=strlen(tmp);
		memcpy(&buffer[index], tmp, len);
		index+=len;
		if(index>size){
			break;
		}
	}
	buffer[index]='\0';

	return 0;
}


int main()
{
    Py_Initialize();
    // 检查初始化是否成功
    if (!Py_IsInitialized()) {
        return -1;
    }
    // 添加当前路径，可以避免导入模块输入全路径
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("print ('---import sys---')");
    PyRun_SimpleString("sys.path.append('./')");
    // 载入名为pytest的脚本
    PyObject* pModule = PyImport_ImportModule("pytest");
    if (!pModule) // 加载模块失败
    {
        printf("[ERROR] Python get module failed.\n");
        //system("pause");
        return 0;
    }
    printf("[INFO] Python get module succeed.\n");
    
    PyObject* func = PyObject_GetAttrString(pModule, "get_mail_message");
    if (!PyCallable_Check(func))
    {
        printf("not find add function.\n");
        //system("pause");
        return -1;
    }
    char eml_path[256]={0};
	char result_path[256]={0};
    snprintf(eml_path, 256,"%s", "./email/YV-S2000_mail_20221012150930_192.168.105.42_21612_1024.eml");
    snprintf(result_path, 256, "%s", "./result");

#if 0
	FILE *fp=NULL;
	fp=fopen(eml_path, "r");
	if(!fp){
        printf("fopen %s failed !\n",eml_path);
		return -1;
	}
	int file_size=get_file_size(fp);
	char *mail_data=(char *)malloc((file_size+1)*sizeof(char));
	if(!mail_data){
        printf("mail data malloc failed\n");
		return -1;
	}
	get_file_data(fp, mail_data, file_size);
	if(fp){
		fclose(fp);
		fp=NULL;
	}
	//printf("datasize:%d:%s\n",file_size,mail_data);
#endif

    PyObject* args = PyTuple_New(2);
    PyObject* arg1 = PyUnicode_FromString(eml_path);
    PyObject* arg2 = PyUnicode_FromString(result_path);

    PyTuple_SetItem(args, 0, arg1);
    PyTuple_SetItem(args, 1, arg2);
    PyObject* pValue = PyObject_CallObject(func, args);
    if (pValue != NULL)
    {
        //printf("Result of call: %ld\n", PyLong_AsLong(pValue));
        //printf("Result of call: %s\n", PyUnicode_AsUTF8String(pValue));
        //printf("Result of call: %s\n", PyBytes_AsString(PyUnicode_AsUTF8String(pValue)));
        printf("Result of call: %s\n", PyUnicode_AsUTF8(pValue));
        Py_DECREF(pValue);
    }
    else
    {
        Py_DECREF(func);
        Py_DECREF(pModule);
        PyErr_Print();
        fprintf(stderr, "Call failed\n");
        return 1;
    }
    Py_Finalize();
    //system("pause");
    return 0;
}
