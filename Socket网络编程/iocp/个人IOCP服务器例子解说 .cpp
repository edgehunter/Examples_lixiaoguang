
这里写两方面的解说，一方面，解说iocp内部处理情况（这部分以个人查考写的iocp服务器和客户端写的）；一方面，参考libeventlibevent-1.4.4-iocp-3 大致调整给出一个比较标准的服务器和客户端例子。
新手可以通过前部分获得建立iocp的基本理解；通过后部分建立自己（基于libevent）标准的iocp代码。

 
一、//先来建立自己的iocp吧

 
1）自己的iocp服务器（MyIOCPServer.cpp）

 
#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////

// 单句柄数据
typedef struct tagPER_HANDLE_DATA
{
 SOCKET Socket;
 SOCKADDR_STORAGE ClientAddr;
 // 将和这个句柄关联的其他有用信息，尽管放在这里面吧

}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

// 但I/O 操作数据
typedef struct tagPER_IO_DATA
{
 OVERLAPPED Overlapped;
 WSABUF DataBuf;
 char buffer[1024];
 int BufferLen;
 int OperationType; // 可以作为读写的标志，为简单，我忽略了
}PER_IO_DATA, *LPPER_IO_DATA;

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////

//线程函数
DWORD WINAPI ServerWorkerThread(LPVOID lpParam);

DWORD WINAPI ServerWorkerThread(LPVOID lpParam)
{
 HANDLE CompletionPort = (HANDLE)lpParam;
 DWORD BytesTransferred;
 LPOVERLAPPED lpOverlapped;
 LPPER_HANDLE_DATA PerHandleData = NULL;
 LPPER_IO_DATA PerIoData = NULL;
 DWORD SendBytes;
 DWORD RecvBytes;
 DWORD Flags;
 BOOL bRet = FALSE;

 while (TRUE) //无限循环
 {
  bRet = GetQueuedCompletionStatus(CompletionPort,&BytesTransferred,(PULONG_PTR)&PerHandleData,(LPOVERLAPPED*)&lpOverlapped,INFINITE);

  // 检查成功的返回，这儿要注意使用这个宏CONTAINING_RECORD
  PerIoData = (LPPER_IO_DATA)CONTAINING_RECORD(lpOverlapped,PER_IO_DATA,Overlapped);
  // 先检查一下，看看是否在套接字上已有错误发生

  if (0 == BytesTransferred)
  {
   closesocket(PerHandleData->Socket);
   GlobalFree(PerHandleData);
   GlobalFree(PerIoData);
   continue;
  }

  // 数据处理
  char sendBuf[100];
  sprintf(sendBuf,"Welcome %s to  %d %d \n",PerIoData->DataBuf.buf,PerHandleData->Socket,::GetCurrentThreadId());
  send(PerHandleData->Socket,sendBuf,strlen(sendBuf)+1,0);
  //WSASend()
   /*DataBuf.len = DATA_BUFSIZE; 
  　　DataBuf.buf = buffer; 　　
    for(i=0; i < SEND_COUNT ;i++) { 　　
     WSASend(PerHandleData->Socket, &DataBuf, 1, 　　&SendBytes, 0, &SendOverlapped, NULL); */


  // 成功了！！！这儿就收到了来自客户端的数据
  cout << PerIoData->DataBuf.buf << ::GetCurrentThreadId() << endl;

  Flags = 0;
  // 为下一个重叠调用建立单I/O 操作数据
  ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));

  PerIoData->DataBuf.len = 1024;
  PerIoData->DataBuf.buf = PerIoData->buffer;
  PerIoData->OperationType = 0; // read

  WSARecv(PerHandleData->Socket,&(PerIoData->DataBuf),1,&RecvBytes,&Flags,&(PerIoData->Overlapped),NULL);
 }
 return 0;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////

int main(int argc, _TCHAR* argv[])
{
 //头部申明
 HANDLE CompletionPort;
 WSADATA wsd;
 SYSTEM_INFO SystemInfo;
 SOCKADDR_IN InternetAddr;
 SOCKET Listen;

 // 加载WinSock2.2
 WSAStartup(MAKEWORD(2, 2), &wsd);

 // 1.创建一个I/O 完成端口
 CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);

 // 2.确定系统中有多少个处理器
 GetSystemInfo(&SystemInfo);

 // 3.基于系统中可用的处理器数量创建工作器线程
 for (int i = 0; i < int(SystemInfo.dwNumberOfProcessors * 2); ++i)
 {
  HANDLE ThreadHandle;

  // 创建一个服务器的工作器线程，并将完成端口传递到该线程
  ThreadHandle = CreateThread(NULL,0,ServerWorkerThread,CompletionPort,0,NULL);

  CloseHandle(ThreadHandle);
 }

 // 4.创建一个监听套接字，以下的套路都是固定的。
 Listen = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

 //绑定和监听
 InternetAddr.sin_family = PF_INET;
 InternetAddr.sin_port = htons(6001);
 InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 bind(Listen, (SOCKADDR*)&InternetAddr, sizeof(InternetAddr));
 listen(Listen, 5);

 //无限循环
 BOOL b = TRUE;
 while (b)
 {
  PER_HANDLE_DATA * PerHandleData = NULL;
  SOCKADDR_IN saRemote;
  SOCKET Accept;
  int RemoteLen;

  // 5.接收连接，并分配完成端口，这儿可以用AcceptEx 来代替，以创
  // 建可伸缩的Winsock 应用程序。
  RemoteLen = sizeof(saRemote);
  Accept = accept(Listen, (SOCKADDR*)&saRemote, &RemoteLen);

  // 6.创建用来和套接字关联的单句柄数据信息结构
  PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR,sizeof(PER_HANDLE_DATA));

  //cout << "Socket number " << Accept << " connected" << endl;

  PerHandleData->Socket = Accept;
  memcpy(&PerHandleData->ClientAddr, &saRemote, RemoteLen);

  // 7.将接受套接字和完成端口关联起来
  CreateIoCompletionPort((HANDLE)Accept,CompletionPort,(DWORD)PerHandleData,0);

  // 开始在接受套接字上处理I/O
  // 使用重叠I/O 机制，在新建的套接字上投递一个或多个异步
  // WSARecv 或 WSASend 请求。这些I/O 请求完成后，工作者线程
  // 会为I/O 请求提供服务，之后就可以坐享其成了


  static int const DATA_BUFSIZE = 4096; 
  DWORD RecvBytes = 0;
  DWORD Flags = 0;

  // 单I/O 操作数据
  LPPER_IO_DATA PerIoData = NULL;
  PerIoData = (LPPER_IO_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_DATA));
  ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
  PerIoData->DataBuf.len = 1024;
  PerIoData->DataBuf.buf = PerIoData->buffer;
  PerIoData->OperationType = 0; // read
  WSARecv(PerHandleData->Socket,&(PerIoData->DataBuf),1,&RecvBytes,&Flags,&(PerIoData->Overlapped),NULL);
 }

 return 0;
}

 

 

2）自己的iocp客户端（MyIOCPClient.cpp）

 

#include "stdafx.h"
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>

void main()
{
 WORD wVersionRequested;
 WSADATA wsaData;
 int err;

 wVersionRequested = MAKEWORD( 1, 1 );

 err = WSAStartup( wVersionRequested, &wsaData );
 if ( err != 0 ) {
  return;
 }


 if ( LOBYTE( wsaData.wVersion ) != 1 ||
  HIBYTE( wsaData.wVersion ) != 1 ) {
   WSACleanup( );
   return; 
 }

 while (true)
 {
  SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);

  SOCKADDR_IN addrSrv;
  addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
  addrSrv.sin_family=AF_INET;
  addrSrv.sin_port=htons(6001);
  connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

  send(sockClient,"This is lisi",strlen("This is lisi")+1,0);

  char recvBuf[100];
  recv(sockClient,recvBuf,100,0);
  printf("%s\n",recvBuf);

  closesocket(sockClient);
 }

 WSACleanup();
}
