#include"ngx_mysql.h"
#include"ngx_redis.h"
#include<iostream>
#include<sstream>
#include<stdio.h>
#include"UserData.h"
using namespace std;
Redis r;
LXmysql my;
void cheack(int id)
{
	UserData h;
	my.Connect("192.168.1.126","root","qny172839","laoxiaketang",3306);
	r.rConnect("192.168.1.150", 6379);
	string key="UserTable:";
	key+=to_string(id);
	vector<string>mvec;
	mvec.push_back("cust_id");
	mvec.push_back("Username");
	mvec.push_back("password");
	auto m=r.Hmget(key,mvec);
	if(m["cust_id"]==to_string(id))
	{
		return;
	}
	auto x=my.GetResult("select * from UserTable where Username=\"dongnanyang\"");
	if(!x.empty())
	{

		h.cust_id=id;
		h.Username=x[id]["Username"];
		cout<<h.Username<<endl;
		h.password=x[id]["password"];
		cout<<h.password<<endl;
		stringstream rd_s;
		rd_s<<"hmset UserTable:"<<id;
		rd_s<<" cust_id "<<"\'"<<id<<"\'";

		rd_s<<" Username"<<" "<<h.Username;
		rd_s<<" password "<<h.password;
		cout<<rd_s.str()<<endl;
		r.RedisCommand(rd_s.str());
	}
}
int main(int argc,char*argv[])
{
	cheack(1);
	return 0;
}
