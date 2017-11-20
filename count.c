#include <stdio.h>
#include <stdlib.h>

#define nn 3

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
	int n = atoi(argv[1]) , i = 1 , j , cp = 0 , index = atoi(argv[2]) ;
	printf("%d\n", n) ;
	char sat, temp , solver[5] ;
	float l = 0 , g = 0 , r = 0 , c = 0 , m = 0 , f, tm[5] ; // from 1
	int nl = 0, ng = 0, nm = 0, nc = 0 , nr = 0 ;
	FILE * fp = fopen("record.txt", "r");
	FILE * fp2 = fopen("in.txt", "a");
	while(i ++ <= n)
	{
		 fscanf(fp, "%c %f %c ", &temp, &f, &sat) ;
		 if(temp == 'l') {l += f ; nl ++ ;}
		 if(temp == 'g') {g += f ; ng ++ ;}
		 if(temp == 'm') {m += f ; nm ++ ;}
		 if(temp == 'c') {c += f ; nc ++ ;}
		 if(temp == 'r') {r += f ; nr ++ ;}
		 if( i % nn == 2 ) cp = 0 ;       //i is the next i
		 if(f > 0.1){
		 	cp ++ ;
		 	printf("%f %d\n", f, cp) ;
		 }
		 solver[(i+1)%nn+1] = temp;
		 tm[(i+1)%nn+1] = f;
		 if(cp == nn){
		 	float min=9999, sec=9999;
		 	for(j=1;j<4;j++)
		 		if(min > tm[j] ) min = tm[j];
		 	for(j=1;j<4;j++)
		 		if(tm[j]<sec&& tm[j] > min) sec = tm[j];
		 	float ff = sec - min;
		 	if(ff / min < 0.05 ) { printf("%f %f %f\n",ff/min, ff, min) ; continue; }
		 	j = i/nn ;
		 	char s[100] = "cp -f test/" ;
			char num[10] = "1" ;
			itoa(j, num) ;
			strcat(s, num) ;
			char t[100] = ".cnf in/" ;
			strcat(s, t) ;
			itoa(index, num) ;
			strcat(s, num) ;
			char e[100] = ".cnf" ;
			strcat(s, e) ;
			printf("%s\n", s) ;
			system(s) ;
			index ++ ;
			fprintf(fp2, "%c %f\n", solver[1], tm[1] ) ;
			fprintf(fp2, "%c %f\n", solver[2], tm[2] ) ;
			fprintf(fp2, "%c %f\n", solver[3], tm[3] ) ;
		}
		 	
	}
	printf("l %f %d\n", l, nl) ;
	printf("g %f %d\n", g, ng) ;
	printf("m %f %d\n", m, nm) ;
	printf("c %f %d\n", c, nc) ;
	printf("r %f %d\n", r, nr) ;
}
	
