#include "StdAfx.h"
#include "IThread.h"

IThread::IThread(void)
{
	m_threadFunc.ThreadExecute=&IThread::ThreadExecute;
}

IThread::~IThread(void)
{}
