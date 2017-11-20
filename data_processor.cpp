#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<stdlib.h>

using namespace std;
int main(int argc, char** argv)
{
	int num = atoi(argv[1]) ;
	FILE* p1 = fopen("in.txt", "r");
	FILE* p2 = fopen("feature.txt", "r");
	FILE* p3 = fopen("naive_Bayes-master/iris.data", "w");
	float tmp, t1, t2, t3, t4, t5, t6, t7, t8, s1, s2, s3;
	char c ;
	
	int i = 1 ;
	for( ; i <= num ; i ++ )
	{
		fscanf(p2, "%f ", &tmp);
		fscanf(p2, "%f ", &t1);
		fscanf(p2, "%f ", &t2);
		fscanf(p2, "%f ", &t3);
		fscanf(p2, "%f ", &t4);
		fscanf(p2, "%f ", &t5);
		fscanf(p2, "%f ", &t6);
		fscanf(p2, "%f ", &t7);
		fscanf(p2, "%f ", &t8);
		fscanf(p1, "%c ", &c);
		fscanf(p1, "%f ", &s1);
		fscanf(p1, "%c ", &c);
		fscanf(p1, "%f ", &s2);
		fscanf(p1, "%c ", &c);
		fscanf(p1, "%f ", &s3);
		//fprintf(p3, "%f %f %f %f %f %f %f ", t1, t2, t3, t4, t5, t6, t7);
		fprintf(p3, "%d %d %d %d %d %d %d ", (int)(t1/10000), (int)(t6), (int)(t3), (int)(t7*10), int(t4/10), int(t5/1000), int(t8/1000) );
		//fprintf(p3, "%f %f %f %f ", t6*500, t5, t3, t4 );
		if(s1 > s2 && s3 > s2 )
		{
			fprintf(p3, "m\n");
		}
		else if(s1 < s3 && s1 < s2 )
		{
			fprintf(p3, "g\n");
		}
		else fprintf(p3, "c\n");
	}
	fclose(p1);
	fclose(p2);
	fclose(p3);
	return 0;
 }
