#include <climits>
#include <map>

#ifndef COMMAND_H
#define COMMAND_H 1

#define MIN_TIME LLONG_MIN
#define MAX_TIME LLONG_MAX
using namespace std;
typedef struct COMMAND{
	int _type;   //1=add, 2=remove, 3=query
	string _addmailpath;   //mail's path that is added if _type=1
	int _mailid;  //mail's id that is removed
	
	//things below are for command "query"
	vector<string> _keyword;
	long long int _date[2];
	//MIN_TIME = 0, MAX_TIME = 2147483647
	string _from;  //from who
	string _to; //to who
	
}Command_t;

void initial(Command_t& re);
Command_t command_parse(string in);
long long int datetoll(int, char*, int, int, int);
void CHECK_COMMAND(Command_t q);

#endif
