#ifndef _NGX_MYSQL_H_
#define _NGX_MYSQL_H_
#include<mysql.h>
#include<string>
#include<map>
struct data;
typedef std::map<std::string,data> SQLDATA;
using XDATA=std::map<std::string,std::string>;
struct data
{
	data();
	data(std::string d);
	data(int a);
	const char*bin_data;
	string data;
	int integer_data;
	int type;//1:varchar 2:int 
	int func_flag;//1:now()
	bool LoadFile(const char*path);
	void drop();
};
class LXmysql
{
	public:
		LXmysql();
	public:
		~LXmysql();
	private:
		static MYSQL*mysql;
		static MYSQL_RES*result;
	public:
		bool Query(std::string sql);
		bool Connect(std::string ip,std::string user,std::string password,std::string dbname,int port=3306);
		bool Insert(std::string tablename,SQLDATA Hdata);
		std::pair<int,XDATA> Fetch();
		bool StoreResult();
		void freeresult();
		std::map<std::string,XDATA>GetResult();
		bool UpdateData(std::string tablename,SQLDATA d,std::string where);
		bool StartTransaction();
		bool Rollback();
		bool Commit();
		bool StopTransaction();
};
#endif
