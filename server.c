/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  main process server
 *
 *        Version:  1.0
 *        Created:  2013Äê05ÔÂ15ÈÕ 21Ê±58·Ö14Ãë
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangxy
 *   Organization:  
 *
 * =====================================================================================
 */



#include    <stdlib.h>
#include    <stdio.h>
#include    <sys/types.h>
#include    <sys/socket.h>
#include    <netinet/in.h> //for sockaddr_in
#include    <sys/stat.h>
#include    <unistd.h>
#include    <time.h>
#include    <string.h>

#include    "send.h"
#include    "process.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main function
 * =====================================================================================
 */
    int 
main ( int argc, char *argv[] )
{
    int sock, pid;
    FILE* conf;
    char buf_conf[4096];
    char* port_str;
    char* path;
    char* pos;
    int PORT;

/* ----------------------- start parse the http.conf ------------------------------- */
conf = fopen("http.conf", "rb");
if(conf == NULL)
{
    printf("Can't open the http.conf file\r\n");
    exit(1);
}

if(!fgets(buf_conf, sizeof(buf_conf), conf))
    return -1;

port_str     =  strtok(buf_conf, " ");
path         =  strtok(NULL, " ");
pos          =  strchr(path, '\n');

if(pos)
    path[pos-path] = 0;

PORT = atoi(port_str);
fclose(conf);

/* ------------------------ end -------------------------------------------------- */

/* ----------------------use socket to implement the http protocol---------------- */

//定义一个socket地址结构
struct sockaddr_in server_addr;
//清理该内容区域的垃圾数据
bzero(&server_addr, sizeof(server_addr));
//创建一个用于tcp的一个socket
sock = socket(AF_INET, SOCK_STREAM, 0);

//设置该地址结构的各种结构体变量的属性
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
server_addr.sin_port = htons(PORT);

//把socket和socket地址联系起来
bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
//listen仅仅被tcp协议服务器调用
//将sock创建的主动套接字转化为被动套接字，并等待客户断浏览器的主动请求
listen(sock, 50);
printf("HTTP server listening on port %d at %s \n", PORT, inet_ntoa(server_addr.sin_addr));

//循环等待队列
while (1){
    int s;
    FILE* f;
    //accept也仅仅tcp服务器调用
    //从已完成连接队列中取出一个连接
    s = accept(sock, NULL, NULL);
    //创建多进程来是实现多进程连接
    //fork返回两次，副进程返回创建的子进程id
    //子进程返回0,如果返回-1表示创建失败
    if ((pid = fork()) == 0)
    {
        close(sock);
        //网络或者管道方式取得文件描述符
        f = fdopen(s, "r+");
        //将处理这个网络文件
        process(f, s, path);

        fclose(f);
        close(s);
        exit(0);
    }
    close(s);
}
close(sock);
return 0;

/* ---------------------------end------------------------------------------------- */


    return EXIT_SUCCESS;
}               /* ----------  end of function main  ---------- */




