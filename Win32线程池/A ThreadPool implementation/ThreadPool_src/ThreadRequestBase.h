#ifndef  __THREADREQUESTBASE_H__
#define __THREADREQUESTBASE_H__

#include <windows.h>

const int DEFAULT_PRIORITY = 0;

// abstract base class for requests
class ThreadRequestBase
{
protected:
	typedef void (*ClientCallback)(ThreadRequestBase*) ;	

public:
	ThreadRequestBase(int pr, ClientCallback callback)
		: m_Priority(pr), m_Callback(callback)	
	{
		static int cnt = 1;
		m_Id = cnt++;
	}
	virtual ~ThreadRequestBase(void) = 0 {}

	virtual void Destroy() = 0;
	virtual void Execute() = 0;

	void CallbackCall(ThreadRequestBase *request)
	{
		if(m_Callback != NULL)
		{
			m_Callback(request);
		}
	}

	inline int GetPriority()const {return m_Priority; }
	inline bool Equals(const ThreadRequestBase & arg)const { return m_Id == arg.m_Id; } 

private:
	// disabled
	ThreadRequestBase(const ThreadRequestBase&);
	ThreadRequestBase& operator=(const ThreadRequestBase&);

	int				m_Id;
	const int		m_Priority;
	ClientCallback	m_Callback;
};

#endif