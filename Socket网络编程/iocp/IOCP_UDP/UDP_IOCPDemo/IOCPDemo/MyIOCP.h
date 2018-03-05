#pragma once

#include <WinSock2.h>
#include "IThread.h"
#include <vector>

using namespace std;

#define IOCP_BUF_LEN 512

typedef struct iocp_data
{
	WSAOVERLAPPED	 overlapped;
	WSABUF			 wsaBuf;
	char			 data[IOCP_BUF_LEN];

	sockaddr_in		 remoteAddr;
	int				 remoteAddrLen;
	DWORD flags;

	iocp_data()
	{
		flags=0;
		wsaBuf.buf=data;
		wsaBuf.len=IOCP_BUF_LEN;
		remoteAddrLen=sizeof(sockaddr_in);

		memset(data,0x00,IOCP_BUF_LEN);
		memset(&remoteAddr,0x00,sizeof(sockaddr_in));
		memset(&overlapped,0x00,sizeof(WSAOVERLAPPED));
	}

	~iocp_data()
	{
		remoteAddrLen=10;
	}

	void initData()
	{
		flags=0;
		wsaBuf.buf=data;
		wsaBuf.len=IOCP_BUF_LEN;
		remoteAddrLen=sizeof(sockaddr_in);

		memset(data,0x00,IOCP_BUF_LEN);
		memset(&remoteAddr,0x00,sizeof(sockaddr_in));
		memset(&overlapped,0x00,sizeof(WSAOVERLAPPED));
	}
}IOCPDATA,* PIOCPDATA;

class CMyIOCP : public IThread
{
public:
	CMyIOCP(void);
	~CMyIOCP(void);
	
	void Run(void);
	void Stop(void);
private:
	bool m_isRun;
	HANDLE m_hIocp;			// 完成端口句柄
	SOCKET m_udpSocket;	// udp套接字
	int m_numOfWorkThreads;	// 工作线程数
	int m_numOfMaxIoConcurrent; // 最大并发数
	vector<IOCPDATA *> m_iocpDataVector;	// 管理完成端口数据信息向量
	vector<HANDLE> m_vetHThread;

	inline void InitData();
	// 创建完成端口
	inline bool InitIOCP();

	// 启动完成端口线程
	bool StartThreads();

	// 创建套接字
	inline bool InitSocket();

	// 完成端口绑定一套接字
	inline bool IocpBindSocket();

	//套接字重叠操作,以便套接字接收远程数据
	void IoPend();

	// 数据接收线程函数;
	DWORD WINAPI ThreadExecute();
};
