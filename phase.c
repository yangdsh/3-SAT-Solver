#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void itoa (int n,char s[])
{
	int i,j;
	
	i=0;
	do{
      		s[i++]=n%10+'0';//取下一个数字
	}
	while ((n/=10)>0);//删除该数字
	
	s[i]='\0';
	for(j=i-1;j>=i/2;j--)//生成的数字是逆序的，所以要逆序输出
	{
    		int temp = s[j] ;
    		s[j] = s[i-1-j] ;
    		s[i-1-j] = temp ;
    	}
} 

int main(int argc, char** argv)
{
	int n = atoi(argv[1]) ;
	char num[10] ;
	int i , j = 0 ;
	for( i = 1 ; i <= n ; i ++ )
	{ 
		char r[100] = "./analy.out " ;
		itoa(i, num) ;
		strcat(r, num) ;
		printf("%s\n", r) ;
		system(r) ;
	}
	return 0 ;
}
