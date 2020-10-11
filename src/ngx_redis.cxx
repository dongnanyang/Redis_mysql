//大量吸收优秀的代码
#include"ngx_redis.h"
#include<iostream>
#include<sstream>
using namespace std;
Redis::Redis()
{

}

Redis::~Redis()
{

}

bool Redis::rConnect(std::string ip,int port)
{
  context=redisConnect(ip.c_str(),port);
  if(context==NULL||context->err){
    if(context)
    {
    cerr<<context->errstr<<endl;
  }else{
    cout<<"redisConnect error"<<endl;
  }
  return false;
  }
  cout<<"redis 连接成功"<<endl;
  return true;
}

bool Redis::set(std::string key,std::string val)
{
  if(context==NULL)
  {
    cerr<<"连接断开"<<endl;
    return false;
  }
  reply=(redisReply*)redisCommand(context,"SET %s %s",key.c_str(),val.c_str());
  if(reply==NULL)
  {
    return false;
  }
  freeReplyObject(reply);
  return true;
}

string Redis::get(string key)
{
  string val="";
  reply=(redisReply*)redisCommand(context,"GET %s",key.c_str());
  if(reply==NULL)
  {
    return val;
  }
  if(reply->type==REDIS_REPLY_STRING){
    val=reply->str;
  }
  return val;
}

bool Redis::Mset(MDATA data)
{
  stringstream s_rd;
  s_rd<<"MSET ";
  std::map<string,string>::iterator pos;
  for(pos=data.begin();pos!=data.end();++pos)
  {
    s_rd<<pos->first;
    s_rd<<" ";
    s_rd<<pos->second;
  }
  reply=(redisReply*)redisCommand(context,s_rd.str().c_str());
  if(reply==NULL)
  {
    return false;
  }
  freeReplyObject(reply);
  return true;
}

MDATA Redis::Mget(MGVEC k)
{
  fieldsmap.clear();
  MDATA m;
  int length=k.size();
  stringstream s_rd;
  s_rd<<"MGET ";
  for(int i=0;i<length;++i)
  {
    s_rd<<k[i];
	s_rd<<" ";
  }
  reply=(redisReply*)redisCommand(context,s_rd.str().c_str());
   //cerr<<s_rd.str()<<endl;
  if(reply==NULL){
  cerr<<"Mget出错了"<<endl;
  return m;}
  if(reply->type==REDIS_REPLY_ARRAY){
    for(int i=0;i<reply->elements;++i){
      redisReply*childReply=reply->element[i];
      if(childReply->type==REDIS_REPLY_STRING){
        m[k[i]]=childReply->str;

      }
    }
  }
  freeReplyObject(reply);
  fieldsmap=m;
  return m;
}

bool Redis::Hmset(string key,MDATA fields)
{
   stringstream s_rd;
   s_rd<<"HMSET ";
   s_rd<<key;
   s_rd<<" ";
   for(auto pos=fields.begin();pos!=fields.end();++pos)
   {
     s_rd<<pos->first;
     s_rd<<" ";
     s_rd<<pos->second;
   }
   reply=(redisReply*)redisCommand(context,s_rd.str().c_str());
   if(reply==NULL)
   {
     return false;
   }
   return true;
}

MDATA Redis::Hmget(string key,MGVEC fields)
{
  fieldsmap.clear();
  MDATA m;
  stringstream s_rd;
  s_rd<<"HMGET ";
  s_rd<<key;
  s_rd<<" ";
  for(auto pos=fields.begin();pos!=fields.end();++pos)
  {
    s_rd<<*pos;
    s_rd<<" ";
  }
  reply=(redisReply*)redisCommand(context,s_rd.str().c_str());
  if(reply==NULL)return m;
  if(reply->type==REDIS_REPLY_ARRAY){
    for(int i=0;i<reply->elements;++i){
      redisReply*childReply=reply->element[i];
      if(childReply->type==REDIS_REPLY_STRING){
        m[fields[i]]=childReply->str;
      }
    }
  }
  fieldsmap=m;
  return m;
}

bool Redis::RedisCommand(std :: string s)
{
	reply=(redisReply*)redisCommand(context,s.c_str());
	if(reply==NULL)
	{
      freeReplyObject(reply);
		return false;
	}
    freeReplyObject(reply);
	return true;
}
