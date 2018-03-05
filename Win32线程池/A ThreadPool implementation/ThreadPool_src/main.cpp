
#pragma warning (disable : 4786) 


#include "Pool.h"
#include "ThreadRequestFunction.h"
#include "ThreadRequestMethod.h"

#include <iostream>
#include <assert.h>
#include <vector>
using namespace std;


// parameter for request function
struct Param
{
	int Id;
	int Priority;

	Param(int id, int pr)
		:Id(id), Priority(pr)
	{}
};

// request function
void func (Param *param)
{
	cout<<"Function: ID : "<<param->Id<<"\t ; Priority : "<<param->Priority<<endl<<flush;
}

// request class
class ClientTest
{
public:
	void TestFunction(Param *param)
	{
		cout<<"Method\t: ID : "<<param->Id<<"\t ; Priority : "<<param->Priority<<endl<<flush;
	}
};

void callback(ThreadRequestBase *request)
{
	cout<<"callback : "<<request->GetPriority()<<endl<<flush;
}


void CreateRequestFunction(vector<ThreadRequestBase*> *requests, int numOfRequest)
{
	requests->reserve(numOfRequest);
	for(int i = 0 ; i < numOfRequest ; i++)
	{
		int id = i;
		int priority = rand();
		Param *param = new Param(id, priority);

		ThreadRequestBase *r = new ThreadRequestFunction<Param>(&func, param, priority, &callback);
		requests->push_back(r);
	}
}

void CreateRequestMethod(vector<ThreadRequestBase*> *requests, int numOfRequest)
{
	requests->reserve(numOfRequest);
	for(int i = 0 ; i < numOfRequest ; i++)
	{
		int id = i;
		int priority = rand();

		ClientTest *client = new ClientTest();
		Param *param = new Param(id, priority);

		ThreadRequestBase *r = new ThreadRequestMethod<ClientTest,Param>(
			client,
			&(ClientTest::TestFunction), 
			param,
			param->Priority
			);

		requests->push_back(r);
	}
}

void InsertRequests(vector<ThreadRequestBase*> *requests, Pool *pool)
{
	vector<ThreadRequestBase*>::iterator iter	= requests->begin();
	vector<ThreadRequestBase*>::iterator end	= requests->end();
	while(iter != end)
	{
		ThreadRequestBase *r = (*iter);
		pool->Enqueue(r);

		iter++;
	}
}

void DeleteRequests(vector<ThreadRequestBase*> *requests)
{
	while(requests->empty() == false)
	{
		ThreadRequestBase *r = requests->back();
		requests->pop_back();

		r->Destroy();
		delete r;
	}
}

int main()
{
	Pool pool;
	vector<ThreadRequestBase*> function_requests;
	vector<ThreadRequestBase*> method_requests;

	srand(GetTickCount());

	CreateRequestFunction	(&function_requests, 10);
	CreateRequestMethod		(&method_requests, 10);

	InsertRequests(&function_requests	, &pool);
	InsertRequests(&method_requests		, &pool);

	bool res = pool.Run();
	assert(res);	

	pool.Wait();

	cout<<endl<<endl<<"Press Enter : ";
	char ch;	
	gets(&ch);

	DeleteRequests (&function_requests);
	DeleteRequests (&method_requests);

	return 0;
}