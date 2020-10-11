#ifndef REDIS_H
#define REDIS_H
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<map>
#include<vector>
#include<string>
#include<hiredis/hiredis.h>
typedef std::map<std::string,std::string> MDATA;
typedef std::vector<std::string> MGVEC;
class Redis
{
public:
  Redis();
public:
  ~Redis();
private:
  redisContext*context;
  redisReply*reply;
public:
  bool rConnect(std::string ip,int port);
  bool set(std::string key,std::string val);
  std::string get(std::string key);
  bool Mset(MDATA data);
  MDATA Mget(MGVEC k);
  bool Hmset(std::string key,MDATA fields);
  MDATA Hmget(std::string key,MGVEC fields);
  bool RedisCommand(std::string s);
public:
  MDATA fieldsmap;
};

#endif
