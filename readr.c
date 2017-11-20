#include<stdio.h>

int main()
{
	FILE * fp =
	fopen("record.txt", "a+");
	fprintf(fp, "r ") ;
	
	char s[1000] , S ;
	while(gets(s)) 
	{
		if( s[0] == 's')  S = s[2] ;
		if( s[2] == 'R' && s[3] == 'u' && s[4] == 'n' ) break;
	}
	int i = 0 ;
	for( ; ; i ++ )
	{
		if( s[i] == ':' )
		{
			for( i += 2 ; s[i] != ' ' ; i ++ )
				fprintf(fp, "%c", s[i]) ;
			fprintf(fp, " %c\n", S) ;
			return 0 ;
		}
	}
}
	
