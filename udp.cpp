#include <string.h>
#include <stdio.h>
#include "udp.h"
#include "math.h"

myudp::myudp()
{
#ifndef MYLINUX
    //初始化socket
    DWORD ver;
    WSADATA WSAData;
    ver = MAKEWORD(1, 1);//sock的版本

    WSAStartup(ver , &WSAData);//调用socket第一步不可忽略
    //初始化完成
     st = socket(AF_INET , SOCK_DGRAM, 0);//建立一个socket
#endif

}

myudp::~myudp()
{
#ifdef MYLINUX
close(st);
#else
closesocket(st);//使用完socket要关闭
WSACleanup();//释放socket内部资源
#endif

}


int myudp::socket_send(const char *IP,const char *buf, int len)
{


    //参数1指定协议，AF_INET代表要用TCP/IP
    //参数2SOCK_DGRAM表示用UDP协议
    //参数3一般默认0
    st = socket(AF_INET , SOCK_DGRAM, 0);//建立一个socket
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));//初始化结构addr
 //   addr.sin_family = AF_INET;//代表使用一个TCP/IP协议
 //   addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr(IP);


    size_t rc=0;


    rc = sendto(st,buf,len,0,(struct sockaddr *)&addr,sizeof(addr));


    return rc;
}

int myudp::socket_bind(short int port)
{

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));//初始化结构addr
    addr.sin_family = AF_INET;//代表使用一个TCP/IP协议
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);//作为接收方不需要指定具体的ip地址
    int rc=0;
    return bind(st,(struct sockaddr *)&addr, sizeof(addr));//绑定端口

}

int myudp::socket_recv(char *buf, int len,char *srcIP)
{

    //参数1指定协议，AF_INET代表要用TCP/IP
    //参数2SOCK_DGRAM表示用UDP协议
    //参数3一般默认0

        struct sockaddr_in sendaddr;
        memset(&sendaddr,0,sizeof(sendaddr));
#ifdef MYLINUX
        socket_t len;
#else
        int addrlen;
#endif
        addrlen = sizeof(sendaddr);

            memset(buf , 0, len);
            size_t rc = recvfrom(st, buf,len, 0,(struct sockaddr *)&sendaddr,&addrlen);
           if(srcIP)
                   strcpy(srcIP,inet_ntoa(sendaddr.sin_addr));


    return rc;

}


