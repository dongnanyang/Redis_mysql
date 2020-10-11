#ifndef _MYSQL_CONNECT_POOL_H
#define _MYSQL_CONNECT_POOL_H
#include<string>
#include<atomic>
#include<pthread.h>
#include<list>
#include"ngx_mysql.h"
class myConnectPool
{
	public:
		myConnectPool(int connectnum,std::string url,std::string user,std::string pass,std::string ip);
	public:
		~myConnectPool();
	private:
		std::string ip;
		std::string Username;
		std::string dBname;
		std::string password;
		std::int port=3306;
		std::atomic<int>curSize;
		std::atomic<int>maxSize;
		pthred_mutex_t lock;
		std::list<LXmysql*>connList;
		static myConnectPool*m_Instance;
	public:
		static myConnectPool*GetInstance()
		{
			if(m_Instance==NULL)
			{
				if(m_Instance==NULL)
				{
					m_Instance=new myConnectPool(50,"laoxiaketang","root","qny172839","192.168.1.150");
				}
			}
			return m_Instance;
		}
	public:
		bool create();
		LXmysql*GetConnect();
		LXmysql*CreateConnect();
		void    DropPool();
		void    backpool(LXmysql*my);
};

#endif
