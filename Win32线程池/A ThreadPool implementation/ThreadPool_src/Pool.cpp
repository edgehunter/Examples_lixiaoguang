// Pool.cpp: implementation of the Pool class.
//
//////////////////////////////////////////////////////////////////////

#include "Pool.h"
#include "ThreadRequestBase.h"

#include <assert.h>
#include <iostream>

using namespace std;

// RequestWrapper wrappes the request from client. it necessary because a function which can be executed
// in a separate thread must be a C-function or a static function of some class(I don't know another option).
// so the wrapper addes the pointer to a pool.
struct RequestWrapper
{
	Pool				*This;
	ThreadRequestBase	*ClientRequest;

	int GetPriority() { return ClientRequest->GetPriority(); }

	RequestWrapper(Pool *p, ThreadRequestBase *trb)
		:This(p), ClientRequest(trb)
	{}

private:
	// disabled
	RequestWrapper(const RequestWrapper&);
	RequestWrapper&operator=(const RequestWrapper&);
};





/********************************************************************/
/*					friend functions, called in a separate thread	*/
/********************************************************************/

DWORD WINAPI ThreadProc (LPVOID lpParameter)
{
	RequestWrapper *wr = static_cast<RequestWrapper*>(lpParameter);
	ThreadRequestBase	*request  = wr->ClientRequest;
	Pool	*_this	  = wr->This;

	// executes the request's procedure
	request->Execute();
	
	// call callback
	request->CallbackCall(request);

	_this->ChangeNumRunningThread(-1);
	SetEvent(_this->m_EventRequestFinish);

	// unblock the wait for specific request function 
	if(	_this->m_WaitedRequest != NULL && 
		request->Equals(*(_this->m_WaitedRequest)))
	{
		SetEvent(_this->m_EventWantedRequestEnd);
	}

	delete wr;

	return 0;
}

DWORD WINAPI RunMainThread(LPVOID ptr)
{
	Pool *_this = static_cast<Pool*>(ptr);

	while(true)
	{
		while(_this->ChangeNumRunningThread(0) < _this->m_Max)
		{
			RequestWrapper *wr = _this->Dequeue();
			if(wr != NULL)
			{
				HANDLE h = CreateThread(NULL, 
							0,
							ThreadProc, 
							wr,
							0,
							NULL
							);
				if(h != NULL)
				{
					_this->ChangeNumRunningThread(1);
				}
			}
			else if(wr == NULL) // the queue is empty, waits for a new requests
			{
				WaitForSingleObject(_this->m_EventRequestAdded, INFINITE);
			}
		}

		// waits until some request will end 
		WaitForSingleObject(_this->m_EventRequestFinish, INFINITE);
	}

	return 0;
}





/********************************************************************/
/*					Implementation of publics						*/
/********************************************************************/

Pool::Pool(int max)
{
	m_Max				= max;
	m_NumRunningThread	= 0;
	m_MainThread		= NULL;
	m_WaitedRequest		= NULL;

	InitializeCriticalSection(&m_CSecQueue);
	InitializeCriticalSection(&m_CSecNumRunningThread);

	m_EventRequestFinish    = CreateEvent(NULL, false, false, NULL);
	m_EventRequestAdded		= CreateEvent(NULL, false, false, NULL);
	m_EventWantedRequestEnd	= CreateEvent(NULL, false, false, NULL);
	m_EventAllRequestEnd	= CreateEvent(NULL, false, false, NULL);
}

Pool::~Pool()				
{ 
	CloseHandle(m_MainThread);
	
	DeleteCriticalSection(&m_CSecQueue);
	DeleteCriticalSection(&m_CSecNumRunningThread);

	CloseHandle(m_EventRequestFinish);
	CloseHandle(m_EventRequestAdded);
	CloseHandle(m_EventWantedRequestEnd);
	CloseHandle(m_EventAllRequestEnd);
}


void Pool::Enqueue(ThreadRequestBase *request)
{
	ResetEvent(m_EventAllRequestEnd);
	RequestWrapper *rw = new RequestWrapper(this, request);

	EnterCriticalSection(&m_CSecQueue);
	m_RequestQueue.push(rw);
	LeaveCriticalSection(&m_CSecQueue);

	SetEvent(m_EventRequestAdded);
}

bool Pool::Run()
{
	assert(m_MainThread == NULL);

	m_MainThread = CreateThread(NULL, 
								0, 
								RunMainThread, 
								this, 
								0, 
								NULL
								);
	return m_MainThread != NULL;
}

void Pool::Stop()
{
	assert(m_MainThread != NULL);

	CloseHandle(m_MainThread);
	m_MainThread = NULL;
}

DWORD Pool::Wait(DWORD timeToWait)
{
	return WaitForSingleObject(m_EventAllRequestEnd, timeToWait);
}

DWORD Pool::Wait(ThreadRequestBase *request, DWORD timeToWait)
{
	m_WaitedRequest = request;
	DWORD res = WaitForSingleObject(m_EventWantedRequestEnd, timeToWait);
	m_WaitedRequest = NULL;
	
	return res;
}






/********************************************************************/
/*					Implementation of privates						*/
/********************************************************************/

int Pool::ChangeNumRunningThread(int add)
{	
	EnterCriticalSection(&m_CSecNumRunningThread);
	m_NumRunningThread += add;
	int tmp = m_NumRunningThread;
	LeaveCriticalSection(&m_CSecNumRunningThread);

	EnterCriticalSection(&m_CSecQueue);
	bool isQueueEmpty = m_RequestQueue.empty();
	LeaveCriticalSection(&m_CSecQueue);

	if(tmp == 0 && isQueueEmpty)
	{
		SetEvent(m_EventAllRequestEnd);
	}

	return tmp;
}

RequestWrapper* Pool::Dequeue()
{
	RequestWrapper *wr = NULL;

	EnterCriticalSection(&m_CSecQueue);

	if(m_RequestQueue.empty() == false)
	{
		wr = m_RequestQueue.top();
		m_RequestQueue.pop();
	}

	LeaveCriticalSection(&m_CSecQueue);

	return wr;
}
