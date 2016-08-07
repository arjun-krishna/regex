#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <cstring>

using namespace std;
//higher these numbers larger strings can be handelled
#define number_productions 1500
#define max_length_productions 1500

/* file format
 ----------- CNF FORMAT --------------
 <A> = <B><C>	#non-terminal
 <A> = $a$		#terminal
 <S> #start state
*/


class CNF{
	pair <char*, pair<char*,char*> >  P2[number_productions];	// A --> BC type
	pair <char*,char*> P1[number_productions];					// A --> a type
	int p2_size,p1_size;

	public:
		CNF(char* file){

			p2_size=0;
			p1_size=0;
			pair <char*, pair<char*,char*> > p2_temp;
			pair <char*,char*> p1_temp;

			char buffer[100];
			ifstream fin;
			fin.open(file);
			int length=0,j=0,k=0;
			char a[number_productions][3][max_length_productions];
			int level=0;

			while(fin>>buffer){
				// buffer contains productions
				length = strlen(buffer);
				j=0;
				k=0;
				for(int i=0;i<length;i++){
					
					if(buffer[i]=='<'){
						k=0;
						i++;
						while(buffer[i]!='>'){
							a[level][j][k++]=buffer[i];
							i++;
						}
						a[level][j][k]='\0';
						j++;

					}
					else if(buffer[i]==' ' || buffer[i]=='\t'){
						i++;
					}
					
					else if(buffer[i]=='$'){
						k=0;
						i++;
						while(buffer[i]!='$'){
							a[level][j][k++]=buffer[i++];
						}
						a[level][j++][k]='\0';
					}

				}
				if(j==3){
					//cout<<a[level][0]<<" --> "<<a[level][1]<<' '<<a[level][2]<<endl;
					p2_temp = make_pair(a[level][0],make_pair(a[level][1],a[level][2]));
					P2[p2_size++]=p2_temp;
					level++;
				}
				else{
					//cout<<a[level][0]<<" --> "<<a[level][1]<<endl;
					p1_temp = make_pair(a[level][0],a[level][1]);
					P1[p1_size++]=p1_temp;
					level++;
				}
			}
			fin.close();
		}
		void print_productions(){
			int len= p2_size;
			for(int i=0;i<len;i++){
				cout<<P2[i].first<<" --> "<<P2[i].second.first<<" "<<P2[i].second.second<<endl;
			}
			len = p1_size;	
			for(int i=0;i<len;i++){

				cout<<P1[i].first<<" --> "<<P1[i].second<<endl;
			}
		}

		bool CYK(char* s){
			int lenstr = strlen(s);
			vector<char*> ARRAY[lenstr+1][lenstr+1];
			char temp[2]="";
			for(int i=0;i<lenstr;i++){	//base case
				// current sysmbol to be produced s[i]
				temp[0]=s[i];
				temp[1]='\0';
				for(int l=0;l<p1_size;l++){

					if(strcmp(P1[l].second,temp)==0){
						ARRAY[i][i+1].push_back(P1[l].first);
					}
				}
			}			
			pair <char*,char*> tmp;
			int k=2;
			while(k<lenstr+1){
				for(int i=0;i<=(lenstr-k);i++){
					//ARRAY[i][i+k]
					for(int j=i+1;j<=(i+k-1);j++){
						char* B,*C;
						for(int x=0;x<ARRAY[i][j].size();x++){
							B = ARRAY[i][j][x];
							for(int y=0;y<ARRAY[j][i+k].size();y++){
								C = ARRAY[j][i+k][y];
								for(int l=0;l<p2_size;l++){
									if((strcmp(P2[l].second.first,B)==0)&& (strcmp(P2[l].second.second,C)==0) ){
										ARRAY[i][i+k].push_back(P2[l].first);
									}
								}
							}
						}
					}
				}
				k++;
			}

			for(int l=0;l<ARRAY[0][lenstr].size();l++){
				if(strcmp(ARRAY[0][lenstr][l],"S")==0){
					return true;
				}
				else {
					return false;
				}
			}
			return false;
		}

};

