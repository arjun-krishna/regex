#include "cnf.h"
#include "regex.h"

int main(int argc,char* argv[]){

	char s[15] = "grammer.cnf";
	CNF G(s);
	//G.print_productions();
	char regex[151];
	cin >> regex;
	
	if(G.CYK(regex) == false){
		//G.print_productions();
		cout<<"Wrong Expression"<<endl;
		return -1;
	}
	automaton M  = create_automaton(regex);
	char string[110000];
	int N;
	cin>>N;
	while(N--){
		cin>>string;
		if(match_string(M,string))
			cout<<"Yes"<<endl;
		else
			cout<<"No"<<endl;
	}
}

