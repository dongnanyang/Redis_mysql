#include"mysql_connectpool.h"
myConnectPool::myConnectPool(int connectnum, std :: string url, std :: string user, std :: string pass, std :: string ip)
{
	maxSize=connectnum;
	dBname=url;
	password=pass;
	this->ip=ip;
	Username=user;
	curSize=0;
}

LXmysql* myConnectPool::CreateConnect()
{

	LXmysql*my=new LXmysql();
	if(!my->Connect(ip,Username,password,dBname,3306))
	{
		return nullptr;
	}
	return my;
}

bool myConnectPool::create()
{
	for(int i=0;i<maxSize/2;++i)
	{
		LXmysql*my=CreateConnect();
		if(my==NULL)
		{
			return false;
		}
		pthread_mutex_lock(&lock);
		connList.push_back(my);
		pthread_mutex_unlock(&lock);
		curSize++;
	}
}

LXmysql*myConnectPool::GetConnect()
{
	if(connList.empty())
	{
		if(curSize<maxSize)
		{
			LXmysql*my=GetConnect();
			if(my!=nullptr)
			{
				curSize++;
				return my;
			}
			return nullptr;
		}
	}
	pthread_mutex_lock(&lock);
	LXmysql*my=connList.front();
	connList.pop_front();
	pthread_mutex_unlock(&lock);
	if(!my->isConnect())
	{
		delete my;
		my=CreateConnect();
		if(!my)
		{
			curSize--;
			return nullptr;
		}
		return my;
	}
}

void myConnectPool::backpool(LXmysql * my)
{
	if(my!=NULL)
	{
		connList.push_back(my);
	}
}

void myConnectPool::DropPool()
{
	std::list<LXmysql*>::iterator pos;
	pthread_mutex_lock(&lock);
	for(pos=connList.begin();pos!=connList.end();++pos)
	{
		if((*pos)->isConnect())
		{
			(*pos)->Close();
		}
		delete *pos;
	}
	pthread_mutex_lock(&lock);
	return;
}

