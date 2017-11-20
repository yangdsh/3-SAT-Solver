#include<iostream>
#include<cstdio>
using namespace std;

#define MAX_LINE 1024

char buf[MAX_LINE];

int getlocation()
{
	int k  = 0;
	while(buf[k]!='>'){
		k++;
		if (buf[k] == '\0') 
			return -1;
	}
	return k;
}

int getback()
{
	int k = 0;
	while(buf[k]!='\0')k++;
	return k;
}

int main()
{
	FILE *inFile;
	char s[100] = "r.txt";
	int num = 0;
	int sum = 0;
	if((inFile = fopen(s, "r"))==NULL)
	{
		printf("fail to read");
		return 1;
	}
	while(fgets(buf, MAX_LINE,inFile)!= NULL)
	{
		sum ++;
		int location = getlocation();
		char jkl;
		jkl = buf[location-1];
		location = getback();
		if(jkl == buf[location-2]) num ++;
	}
	cout << (num*1.0)/sum*100 << '%' << endl;
	
}
