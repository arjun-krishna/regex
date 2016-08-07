#include <cstdio>
#include <cstdlib>
#include <vector>
#include <utility>
#include <cstring>

using namespace std;

class automaton{
public:
	vector <int> S;
	vector <int> F;
	vector <vector <pair<int,char> > > adj;
	int Q;

	automaton(char a='\0'){
		if(a=='\0'){
			Q=0;
		}
		else{
			S.push_back(0);
			F.push_back(1);
			Q = 2;
			vector <pair <int,char> > v;
			adj.push_back(v);
			adj.push_back(v);
			adj[0].push_back(make_pair(1,a));
		}
	}
};

automaton nfa_union(automaton& A,automaton& B){
	automaton* C = new automaton();
	C->Q = A.Q + B.Q;
	vector <pair <int,char> > v;
	for(int i=0;i<C->Q;i++)
		C->adj.push_back(v);
	for(int i=0;i<(A.S.size());i++)
		C->S.push_back(A.S[i]);
	for(int i=0;i<(B.S.size());i++)
		C->S.push_back((B.S[i])+A.Q);
	for(int i=0;i<(A.F.size());i++)
		C->F.push_back(A.F[i]);
	for(int i=0;i<(B.F.size());i++)
		C->F.push_back((B.F[i])+A.Q);

	for(int i=0;i<A.Q;i++){
		for(int j=0;j<A.adj[i].size();j++)
			C->adj[i].push_back(A.adj[i][j]);
	}

	for(int i=0;i<B.Q;i++){
		for(int j=0;j<B.adj[i].size();j++){
			C->adj[i+A.Q].push_back(make_pair(B.adj[i][j].first+A.Q,B.adj[i][j].second));
		}
	}
	return *C;
}

automaton dot(automaton& A,automaton& B){
	automaton* C = new automaton();
	C->Q = A.Q + B.Q;
	vector <pair <int,char> > v;
	for(int i=0;i<C->Q;i++)
		C->adj.push_back(v);
	for(int i=0;i<(A.S.size());i++)
		C->S.push_back(A.S[i]);
	for(int i=0;i<(B.F.size());i++)
		C->F.push_back((B.F[i])+A.Q);

	for(int i=0;i<A.F.size();i++){
		for(int j=0;j<B.S.size();j++){
			C->adj[A.F[i]].push_back(make_pair(B.S[j]+A.Q,'#'));
		}
	}

	for(int i=0;i<A.Q;i++){
		for(int j=0;j<A.adj[i].size();j++)
			C->adj[i].push_back(A.adj[i][j]);
	}

	for(int i=0;i<B.Q;i++){
		for(int j=0;j<B.adj[i].size();j++){
			C->adj[i+A.Q].push_back(make_pair(B.adj[i][j].first+A.Q,B.adj[i][j].second));
		}
	}
	return *C;
}

automaton astrix(automaton& A){
	automaton* C = new automaton();
	C->Q =A.Q +1;
	vector <pair <int,char> > v;
	for(int i=0;i<C->Q;i++)
		C->adj.push_back(v);
	C->S.push_back((C->Q)-1);
	C->F.push_back((C->Q)-1);
	for(int i=0;i<A.S.size();i++)
		C->adj[C->S[0]].push_back(make_pair(A.S[i],'#'));
	for(int i=0;i<A.F.size();i++)
		C->adj[A.F[i]].push_back(make_pair(C->S[0],'#'));

	for(int i=0;i<A.Q;i++){
		for(int j=0;j<A.adj[i].size();j++){
			C->adj[i].push_back(A.adj[i][j]);
		}
	}
	return *C;
}


automaton create_automaton_rec(char* s,int k,int l){
	if(k==l)
		return automaton(s[k]);
	if(s[l-1] == '*'){
		automaton A = create_automaton_rec(s,k+1,l-2);
		return astrix(A);
	}
	else{
		int bracket =0;
		int i;
		for(i=k;i<=l;i++){
			if(s[i]=='(')
				bracket++;
			if(s[i]==')')
				bracket--;
			if(((s[i]=='+')||(s[i]=='.')) && bracket==1)
				break;
		}
		if(s[i]=='+'){
			automaton A = create_automaton_rec(s,k+1,i-1);
			automaton B = create_automaton_rec(s,i+1,l-1);
			return nfa_union(A,B);
		}
		if(s[i]=='.'){
			automaton A = create_automaton_rec(s,k+1,i-1);
			automaton B = create_automaton_rec(s,i+1,l-1);
			return dot(A,B);
		}
	}
}

automaton create_automaton(char* s){
	return create_automaton_rec(s,0,strlen(s)-1);
}

vector <int> run_NFA(automaton& M,char* s){
	vector<int> L;
	for(int i=0;i<M.S.size();i++){
		L.push_back(M.S[i]);
	}
	for(int k=0;k<strlen(s);k++){
		for(int i=0;i<L.size();i++){
			for(int j=0;j<M.adj[L[i]].size();j++){
				if(M.adj[L[i]][j].second == '#'){
					L.push_back(M.adj[L[i]][j].first);
				}
			}
		}
		vector <int> N;
		for(int i=0;i<L.size();i++){
			for(int j=0;j<M.adj[L[i]].size();j++){
				if(M.adj[L[i]][j].second==s[k]){
					N.push_back(M.adj[L[i]][j].first);
				}
			}
		}

		L = N;
	}
	for(int i=0;i<L.size();i++){
		for(int j=0;j<M.adj[L[i]].size();j++){
			if(M.adj[L[i]][j].second == '#'){
				L.push_back(M.adj[L[i]][j].first);
			}
		}
	}
	return L;
}

bool match_string(automaton& M,char* s){
	vector <int> L = run_NFA(M,s);
	for(int i=0;i<L.size();i++){
		for(int j=0;j<M.F.size();j++){
			if(L[i]==M.F[j])
				return true;
		}
	}
	return false;
}