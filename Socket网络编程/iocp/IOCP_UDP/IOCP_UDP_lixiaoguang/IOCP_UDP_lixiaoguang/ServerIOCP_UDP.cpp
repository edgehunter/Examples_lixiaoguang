/*
==========================================================================

Purpose:

This is a sample code that demonstrates for the following:

* Use of the I/O Completion ports with WinSock.  The idea is to create a
simple application that will use IOCP, highlight how to use IOCP.

Notes:

* The server will create IOCP, Worker threads, all incoming client sockets
will be associated with IOCP, the server will accept the client sent
message will display it and then send the same message back as an
acknowledgement.

Author:

* Swarajya Pendharkar

Date:

* 10th March 2006

Updates:

* Implemented IOCP with Overlapped I/O - 24th March 2006
* More updates pertaining to Overlapped I/O - 19th April 2006
* Updates pertaining to IOCP implementation and proper shutdown
of application - 28th May 2006
* Minor tweaks and comments - 9th June 2006
* Updates to allow clients to send multiple messages and other
code improvements - 19th August 2006
* Minor updates - 22th August 2006
* Number of worker threads will be based on the number of processors.
New improved WriteToConsole().
- 24th September 2006
* Minor tweaks - 28th September 2006
* Fixed a defect as per input by xircon (codeproject.com) and
Visual C++ 2005 migration updates - 3rd Feb 2007
* Cut down CPU usage by updating WSAWaitForMultipleEvents()
- 31st March 2007
==========================================================================
*/

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <winsock2.h>
#include <vector>

#include "ServerIOCP_UDP.h"

int main()
{

	if (false == Initialize())
	{
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;

	struct sockaddr_in ServerAddress;

	//Overlapped I/O follows the model established in Windows and can be performed only on 
	//sockets created through the WSASocket function 
	//ListenSocket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	//ListenSocket = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a receiver socket to receive datagrams
	ListenSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == ListenSocket)
	{
		printf("\nError occurred while opening socket: %d.", WSAGetLastError());
		goto error;
	}
	else
	{
		printf("\nWSASocket() successful.");
	}

	//Set Socket RecvBuffer To Maximum
	SetSocketRecvBufferToMaximum(ListenSocket);

	//Cleanup and Init with 0 the ServerAddress
	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));

	//Port number will be supplied as a command line argument
	int nPortNo = 5005;

	//Fill up the address structure
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.S_un.S_addr = inet_addr("192.168.1.211"); //WinSock will supply address
	//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);//WinSock will supply address
	ServerAddress.sin_port = htons(nPortNo);    //comes from commandline

	//Assign local address and port number
	if (SOCKET_ERROR == bind(ListenSocket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))
	{
		closesocket(ListenSocket);
		printf("\nError occurred while binding.");
		goto error;
	}
	else
	{
		printf("\nbind() successful.");
	}


	//DWORD nThreadID;
	//g_hAcceptThread = CreateThread(0, 0, AcceptThread, (void *)ListenSocket, 0, &nThreadID);

	AcceptConnection_UDP(ListenSocket);

	printf("\nTo exit this server, hit a key at any time on this console...");

	//Hang in there till a key is hit
	while (!_kbhit())
	{
		Sleep(0);  //switch to some other thread
	}

	WriteToConsole("\nServer is shutting down...");

	//Start cleanup
	CleanUp();

	//Close open sockets
	closesocket(ListenSocket);

	DeInitialize();

	return 0; //success

error:
	closesocket(ListenSocket);
	DeInitialize();
	return 1;
}

bool Initialize()
{
	//Find out number of processors and threads
	g_nThreads = WORKER_THREADS_PER_PROCESSOR * GetNoOfProcessors();

	printf("\nNumber of processors on host: %d", GetNoOfProcessors());

	printf("\nThe following number of worker threads will be created: %d", g_nThreads);

	//Allocate memory to store thread handless
	g_phWorkerThreads = new HANDLE[g_nThreads];

	//Initialize the Console Critical Section
	InitializeCriticalSection(&g_csConsole);

	//Initialize the Client List Critical Section
	InitializeCriticalSection(&g_csClientList);

	//Initialize the Client Count 测试序号
	InitializeCriticalSection(&g_csCountMemory);
	

	for (size_t mm = 0; mm < 1565 + 1; mm++)
	{
		g_csCountArray[mm] = 0;
	}
	

	//Create shutdown event
	g_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Initialize Winsock
	WSADATA wsaData;

	int nResult;
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (NO_ERROR != nResult)
	{
		printf("\nError occurred while executing WSAStartup().");
		return false; //error
	}
	else
	{
		printf("\nWSAStartup() successful.");
	}

	if (false == InitializeIOCP())
	{
		printf("\nError occurred while initializing IOCP");
		return false;
	}
	else
	{
		printf("\nIOCP initialization successful.");
	}

	return true;
}

void SetSocketSendBufferToMaximum(SOCKET s)
{
	int soRecvBufSize = 0;

	int optLen = sizeof(soRecvBufSize);

	if (SOCKET_ERROR == ::getsockopt(s, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&soRecvBufSize), &optLen))
	{
		printf("setsockopt ERROR getsockopt(s, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&soRecvBufSize), &optLen)) \n");
	}

	printf("Send size = %d \n", soRecvBufSize);

	soRecvBufSize = 0x3FFFFFFF;

	printf("Try to set Send buf to %d \n" , soRecvBufSize);

	if (SOCKET_ERROR == ::setsockopt(s, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&soRecvBufSize), sizeof(soRecvBufSize)))
	{
		printf("setsockopt ERROR setsockopt(s, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&soRecvBufSize), sizeof(soRecvBufSize))) \n");
	}

	if (SOCKET_ERROR == ::getsockopt(s, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&soRecvBufSize), &optLen))
	{
		printf("setsockopt ERROR getsockopt(s, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char *>(&soRecvBufSize), &optLen)) \n");
	}

	printf("Send buf actually set to %d \n", soRecvBufSize);
}

void SetSocketRecvBufferToMaximum(SOCKET s)
{
	int soRecvBufSize = 0;

	int optLen = sizeof(soRecvBufSize);

	if (SOCKET_ERROR == ::getsockopt(s, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&soRecvBufSize), &optLen))
	{
		printf("setsockopt ERROR getsockopt(s, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&soRecvBufSize), &optLen)) \n");
	}

	printf("Recv size = %d \n", soRecvBufSize);

	soRecvBufSize = 2147483647;

	//   0x3FFFFFFF - possible max?;

	printf("Try to set recv buf to %d \n", soRecvBufSize);

	if (SOCKET_ERROR == ::setsockopt(s, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&soRecvBufSize), sizeof(soRecvBufSize)))
	{
		printf("setsockopt ERROR setsockopt(s, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&soRecvBufSize), sizeof(soRecvBufSize))) \n");
	}

	if (SOCKET_ERROR == ::getsockopt(s, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&soRecvBufSize), &optLen))
	{
		printf("setsockopt ERROR getsockopt(s, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char *>(&soRecvBufSize), &optLen)) \n");
	}

	printf("Recv buf actually set to %d \n", soRecvBufSize);
}

//Function to Initialize IOCP
bool InitializeIOCP()
{
	//Create I/O completion port
	g_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (NULL == g_hIOCompletionPort)
	{
		printf("\nError occurred while creating IOCP: %d.", WSAGetLastError());
		return false;
	}

	DWORD nThreadID;

	//Create worker threads
	for (int ii = 0; ii < g_nThreads; ii++)
	{
		g_phWorkerThreads[ii] = CreateThread(0, 0, WorkerThread, (void *)(ii + 1), 0, &nThreadID);
	}

	return true;
}

void CleanUp()
{
	//Ask all threads to start shutting down
	SetEvent(g_hShutdownEvent);

	//Let Accept thread go down
	//WaitForSingleObject(g_hAcceptThread, INFINITE);

	for (int i = 0; i < g_nThreads; i++)
	{
		//Help threads get out of blocking - GetQueuedCompletionStatus()
		PostQueuedCompletionStatus(g_hIOCompletionPort, 0, (DWORD)NULL, NULL);
	}

	//Let Worker Threads shutdown
	WaitForMultipleObjects(g_nThreads, g_phWorkerThreads, TRUE, INFINITE);

	//Cleanup dynamic memory allocations, if there are any.
	CleanClientList();
}

void DeInitialize()
{
	//Delete the Client Count 测试序号
	DeleteCriticalSection(&g_csCountMemory);

	//Delete the Console Critical Section.
	DeleteCriticalSection(&g_csConsole);

	//Delete the Client List Critical Section.
	DeleteCriticalSection(&g_csClientList);

	//Cleanup IOCP.
	CloseHandle(g_hIOCompletionPort);

	//Clean up the event.
	CloseHandle(g_hShutdownEvent);

	//Clean up memory allocated for the storage of thread handles
	delete[] g_phWorkerThreads;

	//Cleanup Winsock
	WSACleanup();
}

//This function will process the accept event
void AcceptConnection_UDP(SOCKET ListenSocket)
{
	//完成key，UDP方式如何绑定。。。？？？追随CreateIoCompletionPort((HANDLE)pClientContext->GetUDPSocket(), g_hIOCompletionPort, (DWORD)pClientContext, 0);，还是同overlap地址一致。。。
	//确定当前方式，存在错误，需要修改。

	//确定overlap地址，与CClientContext地址，保持一致（类中不采用指针方式），可以实现多次正确投递
	//overlap地址与其他buffer地址，绑定到一个类中，地址有关联性，可以用于数据读取。

	//Create a new ClientContext for this newly accepted client
	CClientContext   *pClientContext = NULL;

	if (true == AssociateWithIOCP(ListenSocket))
	{
		//Once the data is successfully received, we will print it.
		for (int ii = 0; ii < g_nThreads; ii++)
		{
			pClientContext = new CClientContext;
			pClientContext->SetUDPSocket(ListenSocket);
			pClientContext->SetOpCode(OP_WRITE);

			//Store this object
			AddToClientList(pClientContext);
			

			WSABUF *p_wbuf = pClientContext->GetWSABUFPtr();
			OVERLAPPED *p_ol = pClientContext->GetOVERLAPPEDPtr();

			//Get data.
			DWORD dwFlags = 0;
			DWORD dwBytes = 0;

			/*pIocpData = new IOCPDATA;

			int nBytesRecv = WSARecvFrom(ListenSocket,
			&pIocpData->wsaBuf,
			1,
			&dwBytes,
			&pIocpData->flags,
			(sockaddr*)&pIocpData->remoteAddr,
			&pIocpData->remoteAddrLen,
			(LPWSAOVERLAPPED)pIocpData,
			NULL);*/

			if (ii == g_nThreads-1)
			{
				//先向客户端发送取数指令
				sockaddr_in *m_ClientAddress = pClientContext->GetAddressPtr();

				//Fill up the address structure
				m_ClientAddress->sin_family = AF_INET;
				m_ClientAddress->sin_addr.S_un.S_addr = inet_addr("192.168.1.19"); //WinSock will supply address
				m_ClientAddress->sin_port = htons(8080);    //comes from commandline

				pClientContext->SetBuffer_QS();
				pClientContext->SetWSABUFLength(8);
				pClientContext->SetOpCode(2);

				//Overlapped send
				int nBytesSent = WSASendTo(ListenSocket, p_wbuf, 1,
					&dwBytes, dwFlags, (SOCKADDR *)pClientContext->GetAddressPtr(), pClientContext->GetClientAddressLen(), p_ol, NULL);

				int err = WSAGetLastError();

				if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))
				{
					WriteToConsole("\nError in Initial Post Send, Error Code %ld.", WSAGetLastError());
				}
			}

			else
			{
				//Post initial RecvFrom
				//This is a right place to post a initial RecvFrom
				//Posting a initial RecvFrom in WorkerThread will create scalability issues.
				int nBytesRecv = WSARecvFrom(ListenSocket,
					p_wbuf,
					1,
					&dwBytes,
					&dwFlags,
					(SOCKADDR *)pClientContext->GetAddressPtr(),
					pClientContext->GetClientAddressLenPtr(),
					p_ol,//(LPWSAOVERLAPPED)pClientContext,
					NULL);//(LPINT)是否存在问题，确实是这里的问题。

				int err = WSAGetLastError();

				if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
				{
					WriteToConsole("\nError in Initial Post, Error Code %ld.", WSAGetLastError());
				}
			}

			
		}
		
	}
}


bool AssociateWithIOCP(SOCKET ListenSocket)
{
	//Associate the socket with IOCP
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)ListenSocket, g_hIOCompletionPort, (DWORD)ListenSocket, 0);

	if (NULL == hTemp)
	{
		WriteToConsole("\nError occurred while executing CreateIoCompletionPort(), Error Code %ld.", WSAGetLastError());

		//Let's not work with this client，UDP模式与TCP不同
		//RemoveFromClientListAndFreeMemory(pClientContext);

		return false;
	}

	return true;
}

//Worker thread will service IOCP requests
DWORD WINAPI WorkerThread(LPVOID lpParam)
{
	int nThreadNo = (int)lpParam;

	void *lpContext = NULL;
	OVERLAPPED       *pOverlapped = NULL;
	CClientContext   *pClientContext = NULL;
	DWORD            dwBytesTransfered = 0;
	int nBytesRecv = 0;
	int nBytesSent = 0;
	DWORD             dwBytes = 0, dwFlags = 0;


	//Worker thread will be around to process requests, until a Shutdown event is not Signaled.
	while (WAIT_OBJECT_0 != WaitForSingleObject(g_hShutdownEvent, 0))
	{
		BOOL bReturn = GetQueuedCompletionStatus(
			g_hIOCompletionPort,
			&dwBytesTransfered,
			(LPDWORD)&lpContext,
			&pOverlapped,
			INFINITE);

		if (NULL == lpContext)
		{
			//We are shutting down
			break;
		}

		//Get the client context
		//pClientContext = (CClientContext *)lpContext;
		pClientContext = (CClientContext *)pOverlapped;

		//pIocpData = (IOCPDATA *)pOverlapped;

		if ((FALSE == bReturn) || ((TRUE == bReturn) && (0 == dwBytesTransfered)))
		{
			//shutdown(pClientContext->GetSocket(), SD_BOTH);
			//closesocket(pClientContext->GetSocket());

			//Display Client's IP close
			WriteToConsole("\nClient close: %s, socket: %d", inet_ntoa(pClientContext->GetAddressPtr()->sin_addr), pClientContext->GetUDPSocket());

			//Client connection gone, remove it.
			RemoveFromClientListAndFreeMemory(pClientContext);

			continue;
		}

		WSABUF *p_wbuf = pClientContext->GetWSABUFPtr();
		OVERLAPPED *p_ol = pClientContext->GetOVERLAPPEDPtr();
		
		//序号检测
		Header *m_Header_UDP = pClientContext->GetHeader_UDP();


		switch (pClientContext->GetOpCode())
		{
		case OP_READ:

			pClientContext->IncrSentBytes(dwBytesTransfered);

			//Write operation was finished, see if all the data was sent.
			//Else post another write.
			if (pClientContext->GetSentBytes() < pClientContext->GetTotalBytes())
			{
				pClientContext->SetOpCode(OP_READ);

				p_wbuf->buf += pClientContext->GetSentBytes();
				p_wbuf->len = pClientContext->GetTotalBytes() - pClientContext->GetSentBytes();

				dwFlags = 0;

				//Overlapped send
				nBytesSent = WSASendTo(pClientContext->GetUDPSocket(), p_wbuf, 1,
					&dwBytes, dwFlags, (SOCKADDR *)pClientContext->GetAddressPtr(), pClientContext->GetClientAddressLen(), p_ol, NULL);

				if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))
				{
					WriteToConsole("\nThread %d: Error occurred while executing WSASend(), address: %s, Socket: %d, Error Code %ld.", nThreadNo, inet_ntoa(pClientContext->GetAddressPtr()->sin_addr), pClientContext->GetUDPSocket(), WSAGetLastError());

					//shutdown(pClientContext->GetSocket(), SD_BOTH);
					//closesocket(pClientContext->GetSocket());

					//Let's not work with this client
					RemoveFromClientListAndFreeMemory(pClientContext);
				}
			}
			else
			{
				//Once the data is successfully received, we will print it.
				pClientContext->SetOpCode(OP_WRITE);
				pClientContext->ResetWSABUF();

				dwFlags = 0;

				//Get the data.
				nBytesRecv = WSARecvFrom(pClientContext->GetUDPSocket(), p_wbuf, 1,
					&dwBytes, &dwFlags, (SOCKADDR *)pClientContext->GetAddressPtr(), pClientContext->GetClientAddressLenPtr(), p_ol, NULL);

				if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
				{
					WriteToConsole("\nThread %d: Error occurred while executing WSARecv(), address: %s, Socket: %d, Error Code %ld.", nThreadNo, inet_ntoa(pClientContext->GetAddressPtr()->sin_addr), pClientContext->GetUDPSocket(), WSAGetLastError());

					//shutdown(pClientContext->GetSocket(), SD_BOTH);
					//closesocket(pClientContext->GetSocket());

					//Let's not work with this client
					RemoveFromClientListAndFreeMemory(pClientContext);
				}
			}

			break;

		case OP_WRITE:

			char szBuffer[MAX_BUFFER_LEN];

			//Display the message we recevied
			pClientContext->GetBuffer(szBuffer);

			
			//判断序号
			if (dwBytesTransfered > 10)
			{
				memcpy(m_Header_UDP, p_wbuf->buf, sizeof(Header));
				pClientContext->CheckCount();
			}


			//WriteToConsole("\nThread %d: The following message was received: %s", nThreadNo, szBuffer);

			/*

			//Send the message back to the client.
			//pClientContext->SetOpCode(OP_READ);

			pClientContext->SetTotalBytes(dwBytesTransfered);
			pClientContext->SetSentBytes(0);

			p_wbuf->len = dwBytesTransfered;

			dwFlags = 0;

			//Overlapped send
			nBytesSent = WSASendTo(pClientContext->GetUDPSocket(), p_wbuf, 1,
				&dwBytes, dwFlags, (SOCKADDR *)pClientContext->GetAddressPtr(), pClientContext->GetClientAddressLen(), p_ol, NULL);
			

			if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))
			{
				WriteToConsole("\nThread %d: Error occurred while executing WSASend(), address: %s, Socket: %d, Error Code %ld.", nThreadNo, inet_ntoa(pClientContext->GetAddressPtr()->sin_addr), pClientContext->GetUDPSocket(), WSAGetLastError());

				//shutdown(pClientContext->GetSocket(), SD_BOTH);
				//closesocket(pClientContext->GetSocket());

				//Let's not work with this client
				RemoveFromClientListAndFreeMemory(pClientContext);
			}

			*/

			//Once the data is successfully received, we will print it.
			pClientContext->SetOpCode(OP_WRITE);
			pClientContext->ResetWSABUF();

			dwFlags = 0;

			//Get the data.
			nBytesRecv = WSARecvFrom(pClientContext->GetUDPSocket(), p_wbuf, 1,
				&dwBytes, &dwFlags, (SOCKADDR *)pClientContext->GetAddressPtr(), pClientContext->GetClientAddressLenPtr(), p_ol, NULL);

			if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
			{
				WriteToConsole("\nThread %d: Error occurred while executing WSARecv(), address: %s, Socket: %d, Error Code %ld.", nThreadNo, inet_ntoa(pClientContext->GetAddressPtr()->sin_addr), pClientContext->GetUDPSocket(), WSAGetLastError());

				//shutdown(pClientContext->GetSocket(), SD_BOTH);
				//closesocket(pClientContext->GetSocket());

				//Let's not work with this client
				RemoveFromClientListAndFreeMemory(pClientContext);
			}


			break;

		default:
			//We should never be reaching here, under normal circumstances.
			break;
		} // switch

		

	} // while

	return 0;
}

//Function to synchronize console output
//Threads need to be synchronized while they write to console.
//WriteConsole() API can be used, it is thread-safe, I think.
//I have created my own function.
void WriteToConsole(char *szFormat, ...)
{
	EnterCriticalSection(&g_csConsole);

	va_list args;
	va_start(args, szFormat);

	vprintf(szFormat, args);

	va_end(args);

	LeaveCriticalSection(&g_csConsole);
}

//Store client related information in a vector
void AddToClientList(CClientContext   *pClientContext)
{
	EnterCriticalSection(&g_csClientList);

	//Store these structures in vectors
	g_ClientContext.push_back(pClientContext);

	LeaveCriticalSection(&g_csClientList);
}

//This function will allow to remove one single client out of the list
void RemoveFromClientListAndFreeMemory(CClientContext   *pClientContext)
{
	EnterCriticalSection(&g_csClientList);

	std::vector <CClientContext *>::iterator IterClientContext;

	//Remove the supplied ClientContext from the list and release the memory
	for (IterClientContext = g_ClientContext.begin(); IterClientContext != g_ClientContext.end(); IterClientContext++)
	{
		if (pClientContext == *IterClientContext)
		{
			g_ClientContext.erase(IterClientContext);

			//i/o will be cancelled and socket will be closed by destructor.
			delete pClientContext;
			break;
		}
	}

	LeaveCriticalSection(&g_csClientList);
}

//Clean up the list, this function will be executed at the time of shutdown
void CleanClientList()
{
	EnterCriticalSection(&g_csClientList);

	std::vector <CClientContext *>::iterator IterClientContext;

	for (IterClientContext = g_ClientContext.begin(); IterClientContext != g_ClientContext.end(); IterClientContext++)
	{
		//i/o will be cancelled and socket will be closed by destructor.
		delete *IterClientContext;
	}

	g_ClientContext.clear();

	LeaveCriticalSection(&g_csClientList);
}

//The use of static variable will ensure that 
//we will make a call to GetSystemInfo() 
//to find out number of processors, 
//only if we don't have the information already.
//Repeated use of this function will be efficient.
int GetNoOfProcessors()
{
	static int nProcessors = 0;

	if (0 == nProcessors)
	{
		SYSTEM_INFO si;

		GetSystemInfo(&si);

		nProcessors = si.dwNumberOfProcessors;
	}

	return nProcessors;
}
