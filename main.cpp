#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

#include "command.h"

using namespace std;
/**///DEBUG
int add(string q){return 6666;};
int remove(int q){return 7777;};
vector<int> query(Command_t q){vector<int> re;re.clear();return re;};
/**///

Command_t parse_command(string);
void CHECK_COMMAND(Command_t);

int main(){
	char __tempchar[5000];
	string tmp;
	Command_t command;
	vector<int> possible_mail;
	int temp_int;
	while(fgets(__tempchar, 5000, stdin) != NULL){
		//parse coommand
		command = command_parse(string(__tempchar));
			//DEBUG
			CHECK_COMMAND(command);
			//

		//switch command type, case 
		switch(command._type){
		case 1:
			temp_int = add(command._addmailpath);  //return the number of mails after adding, if already exist, return -1;
			if(temp_int == -1) 
				printf("-\n");
			else 
				printf("%d\n",temp_int);
			break;
		case 2:
			temp_int = remove(command._mailid);  //return the number of mails after removing, if failed, return -1;
			if(temp_int == -1) 
				printf("-\n");
			else 
				printf("%d\n",temp_int);
			break;
		case 3:
			possible_mail.clear();
			possible_mail = query(command);  //Just pass all information to query(). return a vector<int> storing all possible mails. 
			if(possible_mail.size() == 0) 
				printf("-\n");
			else{
				for(int i=0 ; i<possible_mail.size() ; i++)
					printf("%d ",possible_mail[i]);
				printf("\n");
			}		
			break;
		default:
			cout<<"**ERROR** Unknowed command:"<<tmp<<"\n";
		break;

		}
	}
return 0;
}
