#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;

#define MAX_PER_LINE 1024
#define MAX_XNUM 30000
#define MAX_SEN 80000
#define INF 1<<30

char buf[MAX_PER_LINE];
int ind;
int xNum;
int senNum;
int conNum; 
int fnum[MAX_XNUM] = {};
int nfnum[MAX_XNUM] = {}; 
int totv = 0;


class fea{
public:
	int hornClause; 
	int v;
	int c;
	int vcMax_du;
	int vcMin_du; 
	float avef;
	int fall;
	float acoe;
	float v_c; 
	float vavec;
	int node_num;
	int du_all;
	int acoee;
	int ave_du;
	float asd;
	fea(){
	  asd = 0;
	  vavec = 0;
	  v_c = 0;
          hornClause = 0;
          v = 0;
          c = 0;
          vcMax_du = 0;
          vcMin_du = INF;
          avef = 0;
          acoe = 0;
          node_num = 0;
          fall = 0;
    }
}feature;

int hornClause; 

class sentNode{
public:
	int xnum;
	int du_num;
	sentNode * next;
	
} sen[MAX_SEN];

int getNum()
{
	int k = 0;
	bool fu = false;
	if(buf[ind] == '-'){
		ind ++;
		fu = true;
	}
	if((ind-1>0&&buf[ind-1]=='\0')||buf[ind] == '0') {
		ind = -1;
		return 0;
	}
	while(true)
	{
		if(buf[ind] == ' '||buf[ind] == '\0'||buf[ind] == '\n')   break;
		k = k * 10 + buf[ind] - '0';
        ind++;
	} 
	ind ++;
	if(fu) k = -k;
	return k;
}

class GraphNode{
public:
	int xnum;
	int sennum;
	GraphNode * next;
} edge[5000];

int edgeNum = 0;

void BuildGraph(int SenNum)
{
	sentNode * tmp = &sen[SenNum];
	while(tmp!=NULL)
	{
		int x = tmp->xnum;
		edge[edgeNum].xnum = x;
		edge[edgeNum].sennum = SenNum;
		GraphNode * tmpedge = &edge[edgeNum];
		edgeNum ++;
		tmp = tmp->next;
		for(int i = SenNum+1;i<senNum; i++)
		{
			sentNode * tmp2 = &sen[i];
			while(tmp2 != NULL)
			{
				if(tmp2->xnum + x == 0){
					tmp2 = tmp2->next;
					continue;
				} 
				tmpedge->next = new GraphNode;
				tmpedge = tmpedge->next;
				tmpedge->xnum = tmp2->xnum;
				tmpedge->sennum = i;
				tmp2 = tmp2->next; 
			}
		}
	}
}


 
void getvcdu()
{
	for(int i = 0;i < senNum; i++)
	{
		sentNode * tmp = &sen[i];	
		while(tmp!=NULL)
		{

			int du_num = 0;
			for(int j = 0;j<senNum; j++)
			{
				if(j == i)  continue;
				sentNode* tmp2 = &sen[j];
				while(tmp2 != NULL)
				{
					if(tmp2->xnum + tmp->xnum == 0){
						tmp2 = tmp2->next;
						continue;
					} 
					du_num++;
					tmp2 = tmp2->next; 
				}
			}
			feature.du_all += du_num;
			if(du_num > feature.vcMax_du) feature.vcMax_du = du_num;
			if(du_num < feature.vcMin_du) feature.vcMin_du = du_num;
			tmp->du_num = du_num;
			tmp = tmp->next;
		}
	}
}

void getavee(){
     int ave;
     long long sqsum = 0;
     ave = feature.du_all / feature.node_num;
     feature.ave_du = ave;
     for(int i = 0;i < senNum; i++)
  	{
		sentNode * tmp = &sen[i];	
		while(tmp!=NULL)
		{
		
			sqsum = sqsum + (tmp->du_num - ave)*(tmp->du_num - ave);
			tmp = tmp->next;
		}
	}
	cout << sqsum << endl;
	feature.acoee = (sqsum * 1.0)/feature.node_num; 
}


void getave(){
	long long sqsum = 0;
	long long sum = 0;
	long long asum = 0;
	long long bsum = 0;
	for(int i=1;i<=xNum;i++)
	{
		sum += fnum[i];
		asum += fnum[i];
		sum += nfnum[i];
		bsum+= nfnum[i];
	}
	feature.avef = (sum*1.0)/(2*xNum);
	float ave = feature.avef;
	for(int i=1;i<=xNum;i++)
	{
		sqsum = sqsum + (fnum[i]-ave)*(fnum[i]-ave);
		sqsum = sqsum + (nfnum[i]-ave)*(nfnum[i]-ave);
	}
	feature.acoe = (sqsum*1.0)/(2*xNum);
	feature.asd = (asum*1.0)/bsum;
}

class conNode{
public:
	conNode * father;
	int num;
	conNode(){
		father = NULL;
	}
}* connode[2*MAX_XNUM];

void initcon()
{
	for(int i=0;i<2*MAX_XNUM;i++)
	{

		connode[i] = new conNode();
		connode[i]->num = i;
		connode[i]->father = NULL;
	}
}

int getRoot(int x)
{
	conNode * tmp = connode[x];
	while(tmp->father!= NULL)
		tmp = tmp ->father;
	return tmp->num;
}

void getcon()
{
	conNum = 0;
	for(int i=0;i<senNum;i++)
	{
		sentNode * tmp = &sen[i];
		conNode * tmpcon = connode[sen[i].xnum+MAX_XNUM];
		int fbnum = getRoot(tmpcon->num);
		if(fbnum!=tmpcon->num)
			tmpcon->father = connode[fbnum];
		while(tmp->next!=NULL)
		{
			int fanum = getRoot(tmp->next->xnum+MAX_XNUM);
			if(tmp->next->xnum+MAX_XNUM!=fanum)
				connode[tmp->next->xnum+MAX_XNUM]->father = connode[fanum];
			if(fanum!=fbnum) connode[fanum]->father = tmpcon;
			tmp = tmp->next;
		}
	}

	for(int i=MAX_XNUM-xNum;i<xNum+MAX_XNUM;i++)
	{
		if(connode[i]->father == NULL)
			conNum++;
	}

}
int main(int argc, char ** argv)
{
	freopen("feature.txt","a",stdout); 
	FILE *inFile;
	char s[100] = "in/", e[100] = ".cnf" ;
	strcat(s, argv[1]) ;
	strcat(s, e) ;
	if((inFile = fopen(s, "r"))==NULL)
	{
		printf("fail to read");
		return 1;
	}
	cout << argv[1] << endl ;
	int preSenNum = 0;
	while(fgets(buf, MAX_PER_LINE,inFile)!= NULL)
	{
		if(buf[0]=='c') continue;
		if(buf[0]=='p') 
		{
			ind = 6;
			xNum = getNum();
			senNum = getNum();
			feature.v = xNum;
			feature.c = senNum;
			
			continue;
   		}
		ind = 0;
		int tmpnode=0;
		sentNode *tmp = &sen[preSenNum];
		bool first= true;
		int horn = 0;
		while((tmpnode = getNum() )!= 0)
		{ 
			totv++;
            if(tmpnode >= 0) fnum[tmpnode] ++;
            else nfnum[-tmpnode] ++;
			if(first) {
				sen[preSenNum].xnum = tmpnode;
				first = false;	
				if(tmpnode > 0) horn = 1; 
				feature.node_num ++;
				continue;
			}
			if(tmpnode > 0) horn++;
			tmp->next = new sentNode;
			tmp = tmp->next;
			tmp->xnum = tmpnode;
			feature.node_num++;
		} 
        tmp->next = NULL;
		if(horn <= 1) feature.hornClause ++;
		preSenNum ++ ;
	}
	
	feature.vavec = (totv*1.0)/senNum;
/*	for(int i= 0; i<senNum; i++){
		BuildGraph(i);
	}*/

	int i;
	for(i = 0;i<senNum;i++)
	{
		BuildGraph(i);
	}
	
	getvcdu();
	getavee();
	
	initcon();
	getcon();
	getave();
	feature.v_c = (xNum*1.0)/(senNum*1.0);
	
	cout << feature.vavec << endl;
	cout << xNum << endl;
	cout << feature.hornClause <<endl;
	cout << xNum*senNum-feature.ave_du << endl;
	cout << feature.acoe << endl;
	cout << feature.asd <<endl;
	cout << xNum * senNum << endl;
}
