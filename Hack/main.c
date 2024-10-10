#include<stdio.h>
#include<fstream>
#include<iostream>
#include<string.h>
int getCount(std::string path){
    std::ifstream file(path.c_str());
    if(file.is_open()){
        int n=0;
        file.seekg(0,std::ios::end);
        n=file.tellg();
        file.close();
        return n;
    }

    return -1;
}
char *getData(std::string path){
    
    int n=getCount(path);
    std::ifstream file(path.c_str());
    if(file.is_open()){
        int unnes=10;
        int length=unnes+n;
        char *data=(char*)malloc(length);
        memset(data,length,0);
        file.getline(data,n);
        file.close();
        return data;
    }
}
void writeData(std::string path,char* data,int count){
    std::ofstream file(path.c_str(),std::ios_base::trunc);
    if(file.is_open()){
        file.write(data,count);
        file.close();
       printf("succeed!\n");
        return ;
    }
    return ;
}

int main()
{
    //std::string path="/Home/Desktop/knowledge/technology/Hack/test.out";
    std::string path="test.out";
    int count=getCount(path);
    char* data=getData(path);
    for(int i=4437;i<4441;i++){
        printf("data:%x\n",data[i]);
    }
    data[4440]=0;
    
    writeData(path,data,count);
    return 0;
}
