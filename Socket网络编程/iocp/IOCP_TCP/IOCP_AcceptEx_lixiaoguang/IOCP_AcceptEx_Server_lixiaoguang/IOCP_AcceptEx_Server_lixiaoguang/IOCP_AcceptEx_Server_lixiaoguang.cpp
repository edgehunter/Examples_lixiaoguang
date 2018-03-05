// IOCP_AcceptEx_Server_lixiaoguang.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//本程序在VS2003编译器编译运行。在6.0下可能需要稍加修改。

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>     //微软扩展的类库

#pragma comment(lib, "ws2_32.lib")
using namespace std;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

#define SEND 0
#define RECV 1
#define ACCEPT 2
#define DATA_LENGTH 1000

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

//单句柄数据定义
typedef struct _PER_HANDLE_DATA
{
	SOCKET socket;     //相关的套接字
	SOCKADDR_STORAGE clientAddr;     //客户端的地址
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

//但IO操作数据
typedef struct{
	OVERLAPPED overlapped;
	WSABUF buffer;     //一个数据缓冲区,用于WSASend/WSARecv中的第二个参数
	char dataBuffer[DATA_LENGTH];     //实际的数据缓冲区
	int dataLength;                     //实际的数据缓冲区长度
	int operatorType;                 //操作类型,可以为SEND/RECV两种
	SOCKET client;                     //分别表示发送的字节数和接收的字节数
}PER_IO_DATA, *LPPER_IO_DATA;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

DWORD WINAPI ServerThread(LPVOID lpParam);

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void main()
{
	HANDLE CompletionPort;
	WSADATA data;
	SYSTEM_INFO info;
	SOCKADDR_IN addr;
	SOCKET Listen;

	unsigned int i;
	WSAStartup(MAKEWORD(2, 2), &data);

	//创建一个IO完成端口
	CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	//确定处理器的数量
	GetSystemInfo(&info);     
	
	//创建线城
	for (i = 0; i < info.dwNumberOfProcessors * 2; i++)
	{
		//根据处理器的数量创建相应多的处理线程
		HANDLE thread = CreateThread(NULL, 0, ServerThread, CompletionPort, 0, NULL);
		CloseHandle(thread);
	}

	//创建一个监听套接字(进行重叠操作)
	Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	//将监听套接字与完成端口绑定
	LPPER_HANDLE_DATA perDandleData;
	perDandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA));
	perDandleData->socket = Listen;
	CreateIoCompletionPort((HANDLE)Listen, CompletionPort, (ULONG_PTR)perDandleData, 0);

	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.11");//sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(6001);

	bind(Listen, (PSOCKADDR)&addr, sizeof(addr));
	listen(Listen, 5);

	LPFN_ACCEPTEX lpfnAcceptEx = NULL;     //AcceptEx函数指针
	
	//Accept function GUID
	GUID guidAcceptEx = WSAID_ACCEPTEX;
	
	//get acceptex function pointer
	DWORD dwBytes = 0;
	
	if (WSAIoctl(Listen, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(guidAcceptEx), &lpfnAcceptEx, sizeof(lpfnAcceptEx),
		&dwBytes, NULL, NULL) == 0)
		cout << "WSAIoctl success..." << endl;
	else
	{
		cout << "WSAIoctl failed..." << endl;
		switch (WSAGetLastError())
		{
		case WSAENETDOWN:
			cout << "" << endl;
			break;
		case WSAEFAULT:
			cout << "WSAEFAULT" << endl;
			break;
		case WSAEINVAL:
			cout << "WSAEINVAL" << endl;
			break;
		case WSAEINPROGRESS:
			cout << "WSAEINPROGRESS" << endl;
			break;
		case WSAENOTSOCK:
			cout << "WSAENOTSOCK" << endl;
			break;
		case WSAEOPNOTSUPP:
			cout << "WSAEOPNOTSUPP" << endl;
			break;
		case WSA_IO_PENDING:
			cout << "WSA_IO_PENDING" << endl;
			break;
		case WSAEWOULDBLOCK:
			cout << "WSAEWOULDBLOCK" << endl;
			break;
		case WSAENOPROTOOPT:
			cout << "WSAENOPROTOOPT" << endl;
			break;
		}
		return;
	}


	//准备调用 AcceptEx 函数，该函数使用重叠结构并于完成端口连接
	LPPER_IO_DATA perIoData = (LPPER_IO_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_DATA));
	memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
	perIoData->operatorType = ACCEPT;

	//在使用AcceptEx前需要事先重建一个套接字用于其第二个参数。这样目的是节省时间
	//通常可以创建一个套接字库
	perIoData->client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	perIoData->dataLength = DATA_LENGTH;
	DWORD flags = 0;

	//调用AcceptEx函数，地址长度需要在原有的上面加上16个字节
	//注意这里使用了重叠模型，该函数的完成将在与完成端口关联的工作线程中处理
	cout << "Process AcceptEx function wait for client connect..." << endl;

	int rc = lpfnAcceptEx(Listen, perIoData->client, perIoData->dataBuffer,
		perIoData->dataLength - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes,
		&(perIoData->overlapped));
	if (rc == FALSE)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
			cout << "lpfnAcceptEx failed.." << endl;
	}

	cin >> i;
	closesocket(Listen);
	WSACleanup();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

DWORD WINAPI ServerThread(LPVOID lpParam)
{
	HANDLE CompletionPort = (HANDLE)lpParam;
	DWORD bytes;
	LPPER_HANDLE_DATA perHandleData = NULL;     //单句柄数据
	LPPER_IO_DATA perIoData;             //单IO数据
	DWORD Flags;
	int ret;
	DWORD RecvBytes;
	//进入循环的等待重叠操作的完成
	while (true)
	{
		bytes = -1;
		ret = GetQueuedCompletionStatus(
			CompletionPort,                 //原先的完成端口句柄
			&bytes,                         //重叠操作完成的字节数
			(LPDWORD)&perHandleData,     //原先和完成端口句柄关联起来的单句柄数据
			(LPOVERLAPPED*)&perIoData,     //用于接收已完成的IO操作的重叠结构
			INFINITE);                     //在完成端口上等待的时间 INFINITE 为无限等待

		
		//先检查在套接字上是否发生错误
		//当发生错误时关闭套接字同时释放掉所有的内存.
		int i = 0;
		if (bytes == 0 && (perIoData->operatorType == RECV ||
			perIoData->operatorType == SEND))
		{
			closesocket(perHandleData->socket);
			GlobalFree(perHandleData);
			GlobalFree(perIoData);
			cout << "closesocket and globalfree perhandledata periodata!" << endl;
			continue;
		}
		
		//这是AcceptEx函数处理完成，在下面处理
		if (perIoData->operatorType == ACCEPT)     //处理连接操作
		{
			//使用GetAcceptExSockaddrs函数 获得具体的各个地址参数.
			if (setsockopt(perIoData->client, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
				(char*)&(perHandleData->socket), sizeof(perHandleData->socket)) == SOCKET_ERROR)
				cout << "setsockopt..." << endl;

			perHandleData->socket = perIoData->client;

			//memcpy(&(perHandleData->clientAddr),raddr,sizeof(raddr));
			//将新的客户套接字与完成端口连接
			CreateIoCompletionPort((HANDLE)perHandleData->socket,
				CompletionPort, (ULONG_PTR)perHandleData, 0);

			memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
			perIoData->operatorType = RECV;         //将状态设置成接收
			//设置WSABUF结构
			perIoData->buffer.buf = perIoData->dataBuffer;
			perIoData->buffer.len = perIoData->dataLength = DATA_LENGTH;

			cout << "wait for data arrive(Accept)..." << endl;
			Flags = 0;
			if (WSARecv(perHandleData->socket, &(perIoData->buffer), 1,	&RecvBytes, &Flags, &(perIoData->overlapped), NULL) == SOCKET_ERROR)
				if (WSAGetLastError() == WSA_IO_PENDING)
					cout << "WSARecv Pending..." << endl;
			continue;
		}
		
		if (perIoData->operatorType == RECV)
			cout << perIoData->buffer.buf << endl;     //将接收到的数据显示出来
		Flags = 0;
		perIoData->operatorType = RECV;             //设置成接受数据类型

		ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
		//重新投递一个新的接收请求
		cout << " wait for data arrive..." << endl;
		WSARecv(perHandleData->socket, &(perIoData->buffer), 1,
			&RecvBytes, &Flags, &(perIoData->overlapped), NULL);
	}

	return 0;
}
//在前面有贴出了一个简单的客户端例子，可以结合起来调试，很方便的。