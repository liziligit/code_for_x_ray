#ifndef LOADER_FILE_H
#define LOADER_FILE_H

#include <fstream>
#include <sys/types.h>  //fstat()
#include <sys/stat.h> //fstat()
#include <unistd.h> 
#include <iostream>
using namespace std;

char header[1024];  //预留的1024 byte的数据头
char *p;  //数据指针
int memz;   //缓冲区大侠
int nByte;  //数据大小

//写文件的函数，mode=“wb”为写二进制文件
//fn为文件名 wb为数据缓冲区，wbz为写入的字节数
void write_file(char*fn,char* wb,int wbz,char *mode){
		FILE *p;
		p=fopen(fn,mode);
		fwrite(wb,wbz,1,p);	
        fclose(p);		
}

//加载文件的类
class file_loder{
public:
file_loder(){//构造函数 初始化变量
	fz=0;
	buf=NULL;
}
~file_loder(){
if(buf!=NULL) delete []buf; //析造函数  清空缓冲区
buf=NULL;
}
//获得文件大小 以byte计数
static int file_size(char* fn)
{
    FILE *p =fopen(fn,"rb"); 
    if(p==NULL) {printf("FILE %s Not exist",fn);return -1;} 
    fseek(p,0L,SEEK_END);
    int fz=ftell(p);fclose(p);	
    return fz;
}
//将文件存入缓冲区 
//fn为文件名 pd为缓冲区指针 length为大小 以byte计数
static int file_load(char* fn,char* pd, int length)
{
    FILE *fp=fopen(fn,"rb");
    if(fp==NULL){
        printf("FILE %s Is Not Open",fn);
        return 0;}
    fread(pd,1,length,fp);
    fclose(fp);	
    return 1;
}

//获得文件创建时间
static long file_time(char* fn){
    FILE * fp;
    int fd;
    struct stat buf;
    fp=fopen(fn,"rb"); //C.zip in current directory, I use it as a test
	if(fp==NULL) return 0;
    fd=fileno(fp);
    fstat(fd, &buf);
    long modify_time=buf.st_mtime; //latest modification time (seconds passed from 01/01/00:00:00 1970 UTC)
    fclose(fp);
    return modify_time;
}


int fz;
char* buf;
void load(char *fn){
	fz=file_size(fn);
	if(buf!=NULL) delete []buf;
	buf=new char[fz];
	file_load(fn,buf,fz);
	
}

//写文件
void write(char*fn,char *mode)
{
write_file(fn,buf,fz,mode);
}	

//写文件  将string里的字符串写进文件
void write(char *fn,string s,char *mode)
{
write_file(fn,(char*)s.data(),s.length(),mode);
}
	
};//end of class

//产生缓冲区
int createMem(int s){
	if(memz<s){
		delete []p;
		p=new char[s];
		memz=s;
	}	
    return s;		
}
//写文件
void write(char *fn){
	write_file(fn,header,1024,(char*)"wb");
	write_file(fn,p,nByte,(char*)"ab");	
};
//读文件
void read(char *fn){
		int fz=file_loder::file_size(fn);
		fz-=1024;
		if(fz<=0){cout<<fz<<" is negtive"<<endl; exit(0);}
        createMem(fz);
		nByte=fz;
		FILE *fp;
		fp=fopen(fn,"rb");
		if(fp==NULL){cout<<fn<<" is not opened"<<endl; exit(0);}
		
		fread(header,1024,1,fp);
		fread(p,nByte,1,fp);
		fclose(fp);
		
	}

#endif