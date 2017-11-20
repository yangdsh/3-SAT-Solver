#include<stdio.h>

int main()
{
	FILE * fp =
	fopen("record.txt", "a+");
	fprintf(fp, "m ") ;
	
	char s[1000] ;
	while(gets(s)) 
	{
		if( s[0] == 'C' && s[1] == 'P' && s[2] == 'U' ) break;
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
	gets(s) ;
	fprintf(fp, " %c\n", s[0] ) ; 
	return 0 ;
}
	
