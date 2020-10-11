#include"ngx_mysql.h"
#include<iostream>
#include<sys/stat.h>
#include<stdio.h>
#include<sstream>
#include<vector>
using namespace std;
MYSQL *LXmysql::mysql=NULL;
MYSQL_RES *LXmysql::result=NULL;
bool data::LoadFile(const char * path)
{
	FILE*fp=fopen(path,"rb");
	struct stat statbuf;
	if(stat(path,&statbuf)!=0)
	{
		return false;
	}

	size_t size=statbuf.st_size;
	bin_data=new char[size];
	fread((char*)bin_data,size,1,fp);
	fclose(fp);
}

data::data()
{

}
data::data(int a)
{
	integer_data=a;
	type=2;
}

data::data(std :: string d)
{
	Data=d;
	type=1;
}

void data::drop()
{
	if(bin_data!=NULL)
	{
		delete []bin_data;
		bin_data=NULL;
	}
}
LXmysql::LXmysql()
{
	mysql=new MYSQL;
	mysql=mysql_init(mysql);
}

LXmysql::~LXmysql()
{
	//mysql_close(mysql);
	delete mysql;
}

bool LXmysql::Query(std::string sql)
{
	int err=mysql_query(mysql,sql.c_str());
	if(err!=0)
	{
		cerr<<"Query失败！"<<mysql_error(mysql)<<endl;
		return false;
	}
	return true;
}

bool LXmysql::Connect(std :: string ip, std :: string user, std :: string password, std :: string dbname, int port )
{
	auto err=mysql_real_connect(mysql,ip.c_str(),user.c_str(),password.c_str(),dbname.c_str(),port,0,0);
	if(!err)
	{
		cerr<<"mysql连接失败!"<<endl;
		return false;
	}
	return true;
}

bool LXmysql::Insert(std :: string tablename, SQLDATA Hdata)
{
	stringstream sql;
	sql<<"INSERT INTO `";
	sql<<tablename;
	sql<<"`";
	sql<<"( ";
	string val,key;
	for(auto pos=Hdata.begin();pos!=Hdata.end();++pos)
	{
		key+=pos->first;
		key+=",";
		if(pos->second.func_flag==1)
		{
			val+=pos->second.Data;
			val+=",";
		}
		else
		{
		if(pos->second.type==2)
		{
			val+=to_string(pos->second.integer_data);
			val+=",";
		}
		else if(pos->second.type==1)
		{
			val+="\"";
			val+=pos->second.Data;
			val+="\"";
			val+=",";
		}
	}

	}
	key[key.length()-1]=' ';
	val[val.length()-1]=' ';
	sql<<key;
	sql<<" )";
	sql<<"VALUES( ";
	sql<<val;
	sql<<" )";
	cout<<sql.str()<<endl;
	return Query(sql.str().c_str());
}

void LXmysql::freeresult()
{
	if(result!=NULL)
	{
		mysql_free_result(result);
		result=NULL;
	}
}

 std::pair<int,map<std::string,std::string>> LXmysql::Fetch()
 {
 	std::pair<int,map<std::string,std::string>> rx;
	std::map<std::string,std::string> re;
	if(!result)
	{
		return rx;
	}
	int num=mysql_num_fields(result);
	int rows=mysql_affected_rows(mysql);
	MYSQL_ROW currow=mysql_fetch_row(result);
	if(!currow)
	{
		return rx;
	}
	while(auto field=mysql_fetch_field(result))
	{
		string ss(field->name,field->name_length);
		fieldsvec.push_back(ss);
	}
	for(int i=0;i<num;++i)
	{
		string s=currow[i];
		re.insert(std::make_pair(fieldsvec[i],s));
	}
	string r=currow[0];
	rx=std::make_pair(stoi(r),re);
	return rx;
 }

 bool LXmysql::StoreResult()
 {
	freeresult();
	result=mysql_store_result(mysql);
	if(!result)
	{
		cerr<<"StoreResult Failed!"<<endl;
		return false;
	}
	return true;
 }

 std::map<int,map<std::string,std::string>>LXmysql::GetResult(string sql)
 //要先执行sql语句选取结果集
 {
	map<int,map<std::string,std::string>>ResultMap;
	if(!Query(sql))
	{
		return ResultMap;
	}
	if(!StoreResult())
	return ResultMap;
	for(;;)
	{
		auto re=Fetch();

		if(re.second.empty())
		{
			break;
		}

		//cout<<re.first<<endl;
		ResultMap.insert(std::make_pair(re.first,re.second));
	}
	//cout<<ResultMap[4]["name"]<<endl;
	fieldsvec.clear();
	return ResultMap;
 }

 bool LXmysql::StartTransaction()
 {
	string sql="set autocommit=0";
	return Query(sql);
 }

 bool LXmysql::Rollback()
 {
	string sql="ROLLBACK";
	return Query(sql);
 }

 bool LXmysql::Commit()
 {
	string sql="COMMIT";
	return Query(sql);
 }

 bool LXmysql::StopTransaction()
 {
	string sql="set autocommit=1";
	return Query(sql);
 }

 bool LXmysql::UpdateData(std :: string tablename, SQLDATA d, std :: string where)
 {
	stringstream sql;
	sql<<"UPDATE ";
	sql<<tablename;
	sql<<" SET ";
	for(auto pos=d.begin();pos!=d.end();++pos)
	{
		sql<<pos->first;
		sql<<"=";
		if(pos->second.func_flag==1)
		{
			sql<<pos->second.Data;
		}
		else
		{
			if(pos->second.type==2)
			{
				sql<<to_string(pos->second.integer_data);
			}
			else
			{
				sql<<"\"";
				sql<<pos->second.Data;
				sql<<"\"";
			}
		}
		sql<<",";
	}
	string s=sql.str();
	s[s.size()-1]=' ';
	s+=where;
	return Query(s);
 }

 void LXmysql:: Close()
 {
	mysql_close(mysql);
 }

 bool LXmysql::isConnect()
 {
		if(!mysql_ping(mysql))
		{
			return true;
		}
		return false;
 }
