#ifndef _REDIS_CONNECT_POOL_H_
#define _REDIS_CONNECT_POOL_H_
#include<string>
#include<pthread.h>
#include"ngx_redis.h"
class RedisConnectPool
{
	public:
		RedisConnectPool();
	public:
		~RedisConnectPool();
	private:
		pthread_mutex_t lock;
		std::string ip;
		std::int    port;
	public:
		Redis*GetRedisConnect();
		bool Create();
		Redis*CreateConnect();
		void Drop();
		
};
#endif
