#include"ngx_mysql.h"
int main()
{
	data d("dongnanyang");
	d.type=1;
	SQLDATA s;
	s.insert(std::make_pair("name",d));
	d.integer_data=d.Data.size();
	d.type=2;
	s.insert(std::make_pair("size",d));
	d.type=1;
	d.Data="home/kuangxiang";
	s.insert(std::make_pair("path",d));
	LXmysql my;
	my.Connect("192.168.1.150","root","qny172839","laoxiaketang",3306);
	my.Insert("customes",s);
	//my.GetResult("select * from t_image_2");
	return 0;
}