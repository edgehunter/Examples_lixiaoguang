// Pool.h: interface for the Pool class.
//
//////////////////////////////////////////////////////////////////////

#ifndef  __POOL_H__
#define __POOL_H__


#include <queue>
#include <windows.h>

using namespace std;

const int DEFAULT_MAX_THREADS = 1;

// forward declarations
struct RequestWrapper;
class ThreadRequestBase;



// class Pool manages requests from clients. management is storing requests in a priority queue
// and executing them in paralel. the maximum number of executed requests in paralel is max - variable of ctor.
// Run function opens a new thread called the main thread, which does the management.
// Enqueue function adds a new request to a pool, this function is thread safe.
// Wait function is blocking until all(or spesific request) requests are ended.
// End of management after a dtor of pool.
class Pool
{
public:
	Pool(int max = DEFAULT_MAX_THREADS);
	virtual ~Pool();
	
	bool Run();
	void Stop();
	void Enqueue(ThreadRequestBase *request);
	DWORD Wait(DWORD timeToWait = INFINITE);
	DWORD Wait(ThreadRequestBase *request, DWORD timeToWait = INFINITE);



private:
	// disabled
	Pool(const Pool&);
	Pool&operator=(const Pool&);

	// friend functions - defined in Pool.cpp
	friend DWORD WINAPI RunMainThread	(LPVOID ptr);
	friend DWORD WINAPI ThreadProc		(LPVOID lpParameter);

	// functor - used in stl container - priority queue.  
	template <class ClassT>
	struct less_ptr : public binary_function<ClassT, ClassT, bool> 
	{
		bool operator()(ClassT x, ClassT y) const
		{
			return x->GetPriority() < y->GetPriority();
		}
	};

	RequestWrapper* Dequeue();
	int ChangeNumRunningThread(int add);
	


	int m_Max;								// Maximum of paralel running threads
	int m_NumRunningThread;					// number of current running threads
	HANDLE m_MainThread;					// handle to the main thread(func : RunMainThread)
	
	priority_queue <
		RequestWrapper*, 
		vector<RequestWrapper*>, 
		less_ptr<RequestWrapper*>  
	> m_RequestQueue;						// priority queue of requests
	
	ThreadRequestBase *m_WaitedRequest;		// client waits for this request

	CRITICAL_SECTION m_CSecQueue;			// synchronizes access to queue 
	CRITICAL_SECTION m_CSecNumRunningThread;// Synchronizes access to number of running threads

	HANDLE m_EventRequestFinish;			// signals to main thread when thread finished
	HANDLE m_EventRequestAdded;				// signals to main thread when a new request added
	HANDLE m_EventWantedRequestEnd;			// signals to wait for spesific request function
	HANDLE m_EventAllRequestEnd;			// sygnals wnen all requests are finished
};

#endif
