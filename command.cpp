#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <climits>
#include <vector>
#include <map>

#include "command.h"
using namespace std;
//initialize
void initial(Command_t& re){
	re._date[0] = MIN_TIME;
	re._date[1] = MAX_TIME;
	re._from = "";
	re._from = "";
	re._keyword.clear();
}
//date to int
long long int datetoll(int year, char *month, int day, int hour, int min){
	long long int re=0;
	int monthint;
	//transfer month to int
	switch(month[2]){
	case 'n':   //1 6
		if(month[1] == 'a'){  //1
			monthint=1;
		}else{  //6
			monthint=6;
		}
		break;
	case 'b': //2
		monthint=2;
		break;
	case 'r':  //3 4
		if(month[1] == 'a'){  //3
			monthint=3;
		}else{ //4
			monthint=4;
		}
		break;
	case 'y':  //5
		monthint=5;
		break;
	case 'l':  //7
		monthint=7;
		break;
	case 'g':  //8
		monthint=8;
		break;
	case 'p':  //9
		monthint=9;
		break;
	case 't':  //10
		monthint=10;
		break;
	case 'v': //11
		monthint=11;
		break;
	case 'c':  //12
		monthint=12;
		break;
	defauft:
		cout<<"**ERROR** Unknowed month:"<<month<<endl;
		monthint=0;
		break;
	}
	re += min;      //Just some magic numbers, never mind.
	re += hour*100;
	re += monthint*10000;
	re += year*1000000;
	return re;
}
//main parsing func.
Command_t command_parse(string in){
	Command_t re;
	initial(re);
	size_t templocation, templocation2;
	if(in[0] == 'a'){   //add
		re._type = 1;
		templocation = in.find(" ") + 1;
		re._addmailpath = in.substr(templocation);
	}	
	else if(in[0] == 'r'){   //remove
		re._type = 2;
		templocation = in.find(" ") + 1;
		re._mailid = atoi(in.substr(templocation).c_str());
	}
	else if(in[0] == 'q'){  //query
		re._type = 3;
		templocation = in.find(" ") + 1;
		while(templocation != string::npos and in[templocation]=='-'){
			templocation++;
			switch(in[templocation]){
			case 'f':   //from
				templocation+=2;
				templocation2 = in.find("\"",templocation);
				re._from = in.substr( templocation, templocation2 - templocation );
				templocation = in.find("-", templocation2);
			break;
			case 't':    //to
				templocation+=2;
				templocation2 = in.find("\"", templocation);
				re._to = in.substr( templocation, templocation2 - templocation );
				templocation = in.find("-", templocation2);
			break;
			case 'd':   //date
				templocation++;
				templocation2 = in.find("~", templocation);
				if(in[templocation] == '~'){
					re._date[0] = MIN_TIME;  //Can be comment (Already initialized)
				}else{
					re._date[0] = atoll(in.substr( templocation, templocation2 - templocation ).c_str());
				}
				templocation = in.find(" ", templocation2);
				if(templocation - templocation2 == 1){
					re._date[1] = MAX_TIME;  //Can be comment (Already initialized)
				}else{
					templocation2++;
					re._date[1] = atoll(in.substr( templocation2, templocation - templocation2 ).c_str());
				}
				templocation = in.find("-", templocation);
			break;
			default:
				cout<<"**ERROR** Unexpected char after '-' in query:"<<in<<"\n";
			break;
			}
		}
		//find the start of expression
		//I assume there is no any blank in expression. And this can avoid extra blank at the end.
		templocation = in.rfind(" ", in.length()-2) + 1;  

		//tokenize expression and turn into posorder
		int tempint =  in.length() - templocation;
		char seq[5000]; strcpy(seq, in.substr(templocation, tempint).c_str()); 
		//use strpbrk may be easier in this case
		if(seq[tempint-1]=='\n') seq[tempint-1]='\0';  //avoid an extra '\n' at the end 
		string tmp;   //a tempstring, nothing special
		vector<int> buf;  //simulate a stack.
		map<string, int> opval;  //store priority,  op -> val
		map<int, string> valop;  //val -> op
		opval["("] = 12;	valop[12]="(";
		opval[")"] = 11;	valop[11]=")";
		opval["&"] = 6;		valop[6] ="&";
		opval["|"] = 5;		valop[5] ="|";
		opval["!"] = 10;	valop[10]="!";

		char *s, *last;
		//init condition
		s = strpbrk(seq, "()|&!");   
		last = seq-1;
		while(s != NULL){   //process until there is no special operator left anymore
			if(s-last != 1){    //there is word between operators
				re._keyword.push_back(string(last+1, s-last-1));
			}
			tmp=string(s, 1);
			if(tmp=="("){
				buf.push_back(opval[tmp]);    
			}else if( tmp==")"){
				while(not buf.empty() and buf.back()!=opval["("]){  //dump out all possible operator  
					re._keyword.push_back(valop[buf.back()]);   
					buf.pop_back();
				}
				if(buf.back()==opval["("]){
					buf.pop_back();
				}
			}else if(tmp=="|" or tmp=="&" or tmp=="!"){
				//deal with priority problems
				while(not buf.empty() and buf.back()!=opval["("] and buf.back()>opval[tmp]){   
					re._keyword.push_back(valop[buf.back()]);
					buf.pop_back();
				}
				buf.push_back(opval[tmp]);
			}
			
			last = s;
			s = strpbrk(s+1, "()|&!");
		}
		if(last != seq+strlen(seq)-1){    //if there are words after all operators
			re._keyword.push_back(string(last+1, seq+strlen(seq)-last-1));
		}
		while(not buf.empty()){  
		//dump the rest operator out from buf
		//note that this step must be done after the previous checking step
			re._keyword.push_back(valop[buf.back()]);
			buf.pop_back();
		}
	}
	else{    //WTF your tiping?
		re._type = -1;
	}
	return re;
}

void CHECK_COMMAND(Command_t q){   //DEBUG, no comment
	switch(q._type){
	case 1:
		cout<<"add\n";
		cout<<"\""<<q._addmailpath <<"\"\n";
		cout<<"id="<<q._mailid<<'\n';
	break;
	case 2:
		cout<<"remove\n";
		cout<<"id="<<q._mailid<<'\n';
	break;
	case 3:
		cout<<"query\n";
		cout<<"from \""<<q._from<<"\"\n";
		cout<<"to \""<<q._to<<"\"\n";
		cout<<"date:"<<q._date[0]<<"->"<<q._date[1]<<'\n';
		for(int i=0 ; i<q._keyword.size() ; i++)
			cout<<"\""<<q._keyword[i]<<"\" ";
		cout<<"\n";
	break;
	default:
		cout<<"Unknow command.\n";

	}

}


