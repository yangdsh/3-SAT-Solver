//main
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STDOUT 1

extern char **__environ;

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
	int mode = 1 ;
	int index = atoi(argv[2]) ;
	printf("mode: %d\n", mode) ;
	int i , j = 0 ;
	FILE * fp = fopen("record.txt", "w");
	
	char r[100] = "./random " ;
	strcat(r, argv[1]) ;
	strcat(r, " ") ;
	strcat(r, "1") ;
	system(r) ;              // random, make input
	int st = time(NULL) ;

	for( i = 1 ; i <= n ; i ++ )
	{
		char s[100] = "cp -f test/" ;
		char num[10] ;
		itoa(i, num) ;
		strcat(s, num) ;
		char t[100] = ".cnf sample.cnf" ;
		strcat(s, t) ;
		system(s) ; 			// prepare for input
		printf("%s\n", s) ;
		
		if( mode == 0 ){	// run SAT solver
			printf("l...") ;
			system("./lingeling/lingeling sample.cnf > result.txt") ;
			
			system("./readl < lingeling/result.txt") ;
			j ++ ;
		}
		
			printf("g...") ;
			system("cd glucose/core && ./glucose ../../sample.cnf >result.txt") ;	// run
				
			system("./readg < glucose/core/result.txt") ;	// read result
			j ++ ;
		
		
			printf("m...") ;
			system("cd minisat && minisat ../sample.cnf > result.txt") ;
			system("./readm < minisat/result.txt") ;
			j ++ ;
		
			printf("c...") ;
			system("cd cryptominisat && ./cryptominisat ../sample.cnf > result.txt") ;
			//printf("c finished\n") ;
			system("./readc < cryptominisat/result.txt") ;
			j ++ ;
			int et = time(NULL) ;
			printf("phase time: %d\n", et-st ) ;
		
		if( mode == 0 ){
			printf("r...") ;
			system("cd rsat && ./rsat ../sample.cnf > result.txt") ;
			j ++ ;
		}	
		printf("\n") ;
	}
	int et = time(NULL) ;
	printf("phase time: %d\n", et-st ) ;
	char s[100] = "./count ", num[10], space[3] = " " ;
	itoa(j, num) ;
	strcat(s, num) ;
	strcat(s, space) ;
	itoa(index, num) ;
	strcat(s, num) ;
	system(s) ;		// add up time
	return 0 ;
}
