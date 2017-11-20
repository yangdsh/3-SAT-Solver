#include<stdio.h>

int main()
{
	char S ;
	FILE * fp =
	fopen("record.txt", "a+");
	fprintf(fp, "l ") ;
	char s[1000] ;
	while(gets(s)){
		if(s[0] == 's')  S = s[2] ;
	}
	int i = 0 ;
	for( ; ; i ++ )
	{
		if( s[i] == ' ' )
		{
			for( ++i ; s[i] != ' ' ; i ++ )
				fprintf(fp, "%c", s[i]) ;
			fprintf(fp, " %c\n", S) ;
			return 0 ;
		}
	}
}
	
