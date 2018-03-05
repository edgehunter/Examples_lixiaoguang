#include "StdAfx.h"
#include "MyIOCP.h"

#define UDP_SOCKET_IOCP_KEY  10
#define EXIT_IOCP_KEY	0

CMyIOCP::CMyIOCP(void)
{
	InitData();
}

CMyIOCP::~CMyIOCP(void)
{
}

void CMyIOCP::InitData()
{

	m_hIocp=NULL;
	m_udpSocket=INVALID_SOCKET;
	m_numOfWorkThreads=0;
	m_numOfMaxIoConcurrent=0;
	m_iocpDataVector.clear();
	m_isRun=false;
}

void CMyIOCP::Run()
{
	if (m_isRun)
		return;

	m_isRun=true;
	InitIOCP();
	StartThreads();
	InitSocket();
	IocpBindSocket();
	IoPend();
}

bool CMyIOCP::InitIOCP()
{
	m_hIocp=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	
	if(NULL==m_hIocp)
	{
		TRACE("ERROR:Run IOCP is failure;ErrorCode:%d\n\n",GetLastError());
		return false;
	}

	return true;
}

bool CMyIOCP::InitSocket()
{
	sockaddr_in localAddr={0};
	m_udpSocket=socket(AF_INET,SOCK_DGRAM,0);

	if (SOCKET_ERROR==m_udpSocket)
	{
		TRACE("ERROR:Run socket is failure;ErrorCode:%d\n\n",WSAGetLastError());
		return false;
	}
	
	localAddr.sin_family=AF_INET;
	localAddr.sin_addr.S_un.S_addr=INADDR_ANY;
	localAddr.sin_port=htons(5005);

	if (SOCKET_ERROR==bind(m_udpSocket,(sockaddr*)&localAddr,sizeof(sockaddr)))
	{
		TRACE("ERROR:socket bind info failur,Error Code:%d",WSAGetLastError());
		return false;
	}

	return true;
}

bool CMyIOCP::IocpBindSocket()
{
	if(NULL==CreateIoCompletionPort((HANDLE)m_udpSocket,m_hIocp,UDP_SOCKET_IOCP_KEY,0))
	{
		TRACE("ERROR:iocp bind socket failure;ErrorCode:%d\n\n",WSAGetLastError());
		return false;
	}

	return true;
}

DWORD WINAPI CMyIOCP::ThreadExecute()
{
	DWORD numOfBytesRecv=0;	
	ULONG iocpKey=0;
	PIOCPDATA pIocpData=NULL;
	LPOVERLAPPED lpOverlapped=NULL;
	INT res=0;

	while (true)
	{
		numOfBytesRecv=0;
		lpOverlapped=NULL;

		if (!GetQueuedCompletionStatus(m_hIocp,&numOfBytesRecv,&iocpKey,&lpOverlapped,INFINITE))
		{
			TRACE("ERROR:Get IOCP Queued Failure,ErrorCode:%d\n\n",WSAGetLastError());
		}

		pIocpData=(IOCPDATA *)lpOverlapped;

		switch (iocpKey)
		{
		case UDP_SOCKET_IOCP_KEY:
			TRACE("TeadId:%d;%s\r\n",GetCurrentThreadId(),pIocpData->data);
			break;

		case EXIT_IOCP_KEY:
			return 0;
		}

		pIocpData->initData();
		if (WSARecvFrom(m_udpSocket,
			&pIocpData->wsaBuf,
			1,
			&numOfBytesRecv,
			&pIocpData->flags,
			(sockaddr*)&pIocpData->remoteAddr,
			&pIocpData->remoteAddrLen,
			(LPWSAOVERLAPPED)pIocpData,
			NULL) == SOCKET_ERROR)
		{
			res=WSAGetLastError();
			if (res != WSA_IO_PENDING)
			{
				TRACE("ERROR:WSARecvFrom failure;ErrorCode:%d\n\n",res);
			}
		}
	} //end while(1); 线程循环工作
}

bool CMyIOCP::StartThreads()
{
	SYSTEM_INFO sysInfo={0};
	HANDLE hThread=NULL;

	GetSystemInfo(&sysInfo);	
	m_numOfMaxIoConcurrent=sysInfo.dwNumberOfProcessors;

	/*
	 * 为了更好的发挥服务器的潜力,创建的线程数大于CPU的完成端口的并发数;
	 * 在这里创建了(m_numOfMaxIoConcurrent*2+2)个线程
	 */
	m_numOfWorkThreads=m_numOfMaxIoConcurrent*2+2;
	
	for (int i=0;i<m_numOfWorkThreads;i++)
	{
		hThread=CreateThread(NULL,0,m_threadFunc.ThreadRun,this,0,NULL);		
		if (NULL==hThread)
		{
			TRACE("ERROR:Create thread failure;ErrorCode:%d\n\n",GetLastError());
			m_numOfWorkThreads--;
			continue;
		}

		m_vetHThread.push_back(hThread);
	}  // end for (int i=0;i<m_numOfWorkThreads;i++)
	
	return hThread>0 ? true : false;
}

void CMyIOCP::IoPend()
{
	PIOCPDATA pIocpData = NULL;
	DWORD numOfBytesRecv=0;
	int res=0;
	
	for (int i=0;i<m_numOfWorkThreads;i++)
	{
		pIocpData=new IOCPDATA;		
		ASSERT(NULL!=pIocpData);
		m_iocpDataVector.push_back(pIocpData);

		if (WSARecvFrom(m_udpSocket,
			&pIocpData->wsaBuf,
			1,
			&numOfBytesRecv,
			&pIocpData->flags,
			(sockaddr*)&pIocpData->remoteAddr,
			&pIocpData->remoteAddrLen,
			(LPWSAOVERLAPPED)pIocpData,
			NULL)==SOCKET_ERROR)
		{
			res=WSAGetLastError();
			if (res != WSA_IO_PENDING)
			{
				TRACE("ERROR:WSARecvFrom IO pending failure;ErrorCode:%d\n\n",res);
			}
		}
	}  // end for (int i=0;i<m_numOfMaxIoConcurrent;i++)
}

void CMyIOCP::Stop( void )
{
	if (!m_isRun)
		return;

	vector<IOCPDATA *>::iterator iocpDataItr;

	while (m_numOfWorkThreads>0)
	{
		if (PostQueuedCompletionStatus(m_hIocp,0,EXIT_IOCP_KEY,NULL)==NULL)
		{
			TRACE("ERROR:Post for stop IOCP failure;ErrorCode:%d\n\n",GetLastError());
		}
		m_numOfWorkThreads--;
	} // end while (m_numOfWorkThreads>0)

	WaitForMultipleObjects(m_vetHThread.size(),&m_vetHThread[0],TRUE,INFINITE);

	closesocket(m_udpSocket);
	CloseHandle(m_hIocp);

	for (iocpDataItr=m_iocpDataVector.begin(); iocpDataItr!=m_iocpDataVector.end(); iocpDataItr++)
	{
		delete *iocpDataItr;
	} // end for(iocpDataItr...

	InitData();
}