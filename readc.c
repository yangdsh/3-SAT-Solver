#include<stdio.h>

int main()
{
	FILE * fp =
	fopen("record.txt", "a+");
	fprintf(fp, "c ") ;
	
	char s[1000] ;
	while(gets(s)) 
	{
		if( s[2] == 'C' && s[3] == 'P' && s[4] == 'U' ) break;
	}
	int i = 0 ;
	for( ; ; i ++ )
	{
		if( s[i] == ':' )
		{
			for( i += 2 ; s[i] != ' ' ; i ++ )
				fprintf(fp, "%c", s[i]) ;
			break ;
		}
	}
	gets(s) ;
	fprintf(fp, " %c\n", s[2]) ;
}
	
