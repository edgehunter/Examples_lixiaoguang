#ifndef _SERVER_IOCP_UDP_H_
#define _SERVER_IOCP_UDP_H_

//Disable deprecation warnings
#pragma warning(disable: 4996)

//Op codes for IOCP
#define OP_READ     0
#define OP_WRITE    1

#define WORKER_THREADS_PER_PROCESSOR 4

//Buffer Length 
#define MAX_BUFFER_LEN 3072

//Time out interval for wait calls
#define WAIT_TIMEOUT_INTERVAL 100

//Graceful shutdown Event
//For this simple implementation,
//We can use global variable as well.
//Wanted to demonstrate use of event
//for shutdown
HANDLE g_hShutdownEvent = NULL;

//Number of threads to be created.
int g_nThreads = 0;

//To store handle of worker threads
HANDLE *g_phWorkerThreads = NULL;

CRITICAL_SECTION g_csConsole; //When threads write to console we need mutual exclusion
CRITICAL_SECTION g_csClientList; //Need to protect the client list

//Global I/O completion port handle
HANDLE g_hIOCompletionPort = NULL;

//测试序号
CRITICAL_SECTION g_csCountMemory; //When threads write to console we need mutual exclusion
unsigned short g_csCountArray[1565 + 1];
unsigned int ErrorCount = 0;
unsigned int AllFrameCount = 0;

unsigned int RealErrorCount = 0;

unsigned int ErrorFlag = 0;


typedef struct Head_UDP
{
	unsigned short x = 0;
	unsigned short y = 0;
}Header, *PHeader;

//解决大小端不匹配问题
//////////////////////////////////////////////////////////////////////////
void changeBigLittle(unsigned char* BigLittle, Header* m_UDP_Header)
{

	BigLittle[0] = (unsigned char)(m_UDP_Header->x & 0xff);
	BigLittle[1] = (unsigned char)(m_UDP_Header->x >> 8 & 0xff);
	BigLittle[2] = (unsigned char)(m_UDP_Header->y >> 8 & 0xff);
	BigLittle[3] = (unsigned char)(m_UDP_Header->y & 0xff);

	memcpy(m_UDP_Header, BigLittle, sizeof(Header));
}


//测试序号
void csCheckCount(int idx)
{
	EnterCriticalSection(&g_csCountMemory);

	g_csCountArray[idx]++;
	g_csCountArray[1565]++;

	//printf("\n idx = %d:", idx);

	
	if (idx == 1565 - 1)
	{
		//g_csCountArray[1565] = 0;

		
		if (g_csCountArray[1565] % 1565 == 0)
		{
			//printf("\n------ %d:%d, AllFrameCount = %d", g_csCountArray[1565] / 1565, g_csCountArray[1565], ++AllFrameCount);
			++AllFrameCount;
			g_csCountArray[1565] = 0;
		}
		else
		{
			ErrorFlag += g_csCountArray[1565] % 1565;
			ErrorFlag = ErrorFlag % 1565;

			printf("\n！%d:%d ,ErrorCount = %d, AllFrameCount = %d, ErrorFlag = %d", g_csCountArray[1565] % 1565, g_csCountArray[1565], ++ErrorCount, ++AllFrameCount, ErrorFlag);
			g_csCountArray[1565] = 0;
			
		}

		//if (g_csCountArray[1565] == 1565 * 10)
		//{
		//	g_csCountArray[1565] = 0;
		//}

	}
	

	LeaveCriticalSection(&g_csCountMemory);
}

class CClientContext  //To store and manage client related information
{
private:

	OVERLAPPED        m_pol;
	WSABUF            m_pwbuf;

	int               m_nTotalBytes;
	int               m_nSentBytes;

	SOCKET			  m_UDPSocket;//UDP端口socket
	struct sockaddr_in		  *m_ClientAddress;//客户端地址信息
	int				  m_ClientAddressLen;//存储数据来源IP地址长度
	int               m_nOpCode; //will be used by the worker thread to decide what operation to perform
	char              m_szBuffer[MAX_BUFFER_LEN];

public:
	//测试序号
	
	Header *m_Header;// = new Header;
	unsigned char* BigLittle;// = new unsigned char[sizeof(Header)];

public:

	//Get/Set calls
	void SetOpCode(int n)
	{
		m_nOpCode = n;
	}

	int GetOpCode()
	{
		return m_nOpCode;
	}

	void SetTotalBytes(int n)
	{
		m_nTotalBytes = n;
	}

	int GetTotalBytes()
	{
		return m_nTotalBytes;
	}

	void SetSentBytes(int n)
	{
		m_nSentBytes = n;
	}

	void IncrSentBytes(int n)
	{
		m_nSentBytes += n;
	}

	int GetSentBytes()
	{
		return m_nSentBytes;
	}

	void SetUDPSocket(SOCKET socket)
	{
		m_UDPSocket = socket;
	}

	SOCKET GetUDPSocket()
	{
		return m_UDPSocket;
	}

	void SetAddress(sockaddr_in ClientAddress)
	{
		memcpy(m_ClientAddress, &ClientAddress, sizeof(sockaddr_in));
	}

	sockaddr_in* GetAddressPtr()
	{
		return this->m_ClientAddress;
	}
	
	int* GetClientAddressLenPtr()
	{
		return &this->m_ClientAddressLen;
	}

	int GetClientAddressLen()
	{
		return this->m_ClientAddressLen;
	}
	
	void SetBuffer(char *szBuffer)
	{
		strcpy(m_szBuffer, szBuffer);
	}
	
	void SetBuffer(char *szBuffer, int length)
	{
		memcpy(m_szBuffer, szBuffer, length);
	}

	void SetBuffer_QS()
	{
		m_szBuffer[0] = 0x53;
		m_szBuffer[1] = 0x45;
		m_szBuffer[2] = 0x54;
		m_szBuffer[3] = 0x3c;
		m_szBuffer[4] = 0x51;
		m_szBuffer[5] = 0x53;
		m_szBuffer[6] = 0x01;
		m_szBuffer[7] = 0xff;

	}

	void GetBuffer(char *szBuffer)
	{
		strcpy(szBuffer, m_szBuffer);
	}

	void ZeroBuffer()
	{
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
	}

	void SetWSABUFLength(int nLength)
	{
		m_pwbuf.len = nLength;
	}

	int GetWSABUFLength()
	{
		return m_pwbuf.len;
	}

	WSABUF* GetWSABUFPtr()
	{
		return &m_pwbuf;
	}

	OVERLAPPED* GetOVERLAPPEDPtr()
	{
		return &m_pol;
	}

	void ResetWSABUF()
	{
		ZeroBuffer();
		m_pwbuf.buf = m_szBuffer;
		m_pwbuf.len = MAX_BUFFER_LEN;
	}

	//测试序号
	void CheckCount()
	{		
		//大小端转换
		changeBigLittle(BigLittle, m_Header);

		csCheckCount(m_Header->y);

		
		//if (Count != m_Header->y)
		//{
		//	
		//	printf("序号错误 address: %s, Port: %d, Socket : %d, (%d,%d) \n", inet_ntoa(m_ClientAddress->sin_addr), m_ClientAddress->sin_port, m_UDPSocket, Count, m_Header->y);
		//	Count = m_Header->y;
		//}
	}

	Header *GetHeader_UDP()
	{
		return m_Header;
	}


	//Constructor
	CClientContext()
	{
		//m_pol = new OVERLAPPED;
		//m_pwbuf = new WSABUF;
		m_ClientAddress = new sockaddr_in;

		ZeroMemory(&m_pol, sizeof(OVERLAPPED));
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
		ZeroMemory(m_ClientAddress, sizeof(sockaddr_in));

		m_pwbuf.buf = m_szBuffer;
		m_pwbuf.len = MAX_BUFFER_LEN;

		m_nOpCode = 0;
		m_nTotalBytes = 0;
		m_nSentBytes = 0;

		m_ClientAddressLen = sizeof(sockaddr_in);

		//测试序号
		m_Header = new Header;
		BigLittle = new unsigned char[sizeof(Header)];
	}

	//destructor
	~CClientContext()
	{
		//Wait for the pending operations to complete
		while (!HasOverlappedIoCompleted(&m_pol))
		{
			Sleep(0);
		}

		//Cleanup
		//delete m_pol;
		//delete m_pwbuf;
		delete m_ClientAddress;

		delete m_Header;
		delete BigLittle;
	}
};


//Vector to store pointers of dynamically allocated ClientContext.
//map class can also be used.
//Link list can also be created.
std::vector<CClientContext *> g_ClientContext;

//global functions
bool InitializeIOCP();//
bool Initialize();//

void SetSocketSendBufferToMaximum(SOCKET s);
void SetSocketRecvBufferToMaximum(SOCKET s);


void CleanUp();//
void DeInitialize();//
void AcceptConnection_UDP(SOCKET ListenSocket);
bool AssociateWithIOCP(SOCKET ListenSocket);
DWORD WINAPI WorkerThread(LPVOID lpParam);
void WriteToConsole(char *szFormat, ...);//
void AddToClientList(CClientContext *pClientContext);//
void RemoveFromClientListAndFreeMemory(CClientContext   *pClientContext);
void CleanClientList();
int GetNoOfProcessors();//

#endif