#ifndef  __THREADREQUESTFUNCTION_H__
#define __THREADREQUESTFUNCTION_H__

#include "ThreadRequestBase.h"

// request for a C-style functions
template <typename ParamT = LPVOID>
class ThreadRequestFunction : public ThreadRequestBase
{
	typedef void (*ClientRequestProc)(ParamT *param);
	
public:
	ThreadRequestFunction(
		ClientRequestProc proc, 
		ParamT *param, 
		int pr = DEFAULT_PRIORITY, 
		ClientCallback callback = NULL
		)
		:ThreadRequestBase(pr, callback), m_Param(param), m_ClientProc(proc)
	{}

	virtual void Execute() { m_ClientProc(m_Param); }
	virtual void Destroy() { delete m_Param;		}



private:
	ParamT *m_Param;
	ClientRequestProc m_ClientProc;
};

#endif