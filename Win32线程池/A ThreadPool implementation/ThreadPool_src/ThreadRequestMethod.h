#ifndef  __THREADREQUESTMETHOD_H__
#define __THREADREQUESTMETHOD_H__

#include "ThreadRequestBase.h"



// request for a methods (member function of some class)
template <typename ClassT, typename ParamT = LPVOID>
class ThreadRequestMethod : public ThreadRequestBase
{
public:
	ThreadRequestMethod(
		ClassT *client, 
		void (ClassT::*method)(ParamT *param), 
		ParamT *param, 
		int pr = DEFAULT_PRIORITY,
		ClientCallback callback = NULL
		)
		:ThreadRequestBase(pr, callback), m_Client(client), m_ClientMethod(method), m_Param(param) 
	{}
	virtual void Execute() { (m_Client->*m_ClientMethod)(m_Param);	}
	virtual void Destroy() 
	{ 
		delete m_Param; 
		delete m_Client;
		m_Param = NULL;
		m_Client = NULL;
	}
 


private:
	ClassT *m_Client;
	void (ClassT::*m_ClientMethod)(ParamT *param);
	ParamT *m_Param;
};

#endif