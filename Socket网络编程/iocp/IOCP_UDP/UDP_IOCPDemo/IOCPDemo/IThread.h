#pragma once

class IThread
{
public:
	IThread(void);
	virtual ~IThread(void);

protected:
	// 线程函数
	virtual DWORD WINAPI ThreadExecute(void)=0;	

	typedef union _thread_proc
	{
		DWORD (WINAPI *ThreadRun)(PVOID para);
		DWORD (WINAPI IThread::*ThreadExecute)(void);
	}THREADPROC;    // 线程函数指针类型,使用于普通成员函数作线程函数时使用;

	THREADPROC m_threadFunc; // 以普通成员函数作为线程函数的中间变量
};




