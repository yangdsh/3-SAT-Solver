#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int p[40000];
int signp[3];
int buildRand(int* tp, int mx, int a, int b)
{	
	int sum = 0;
	int i;
	for(i = 0; i < mx; i++)
	{
		sum += rand()%a + b;
		tp[i+1] = sum;
	}
	return sum;
}

int myRand(int* tp, int mx)
{
	int sum = rand()%tp[mx];
	int i;
	for( i=1;i<=mx;i++){
		if(sum<tp[i]) break;
	}
	return i;
}

int main(int argc, char** argv)
{
	int n = atoi(argv[1]) , i , j ;
	int mode = atoi(argv[2]);
	int nx, ns, mx, ms ;
	srand(time(NULL));
	if( mode == 0 ) { mx = 400 , ms = 1000 ; }
	if( mode == 1 ) { mx = 10000 , ms = 20000 ; }
	char s[100] = ".cnf" , temp[100] , pre[100] = "./test/";
	for( j = 1 ; j <= n ; j ++ )
	{
		itoa(j, temp);
		pre[7] = 0 ;
		strcat(pre, temp) ;
		strcat(pre, s) ;
		FILE * fp = fopen(pre, "w");
		do{
		nx = rand() % mx , ns = rand() % ms ;
		} while( nx > ns/9 || nx < ns/11 || ns < ms/2 ) ;
		buildRand(p, nx, 2, 3);
		buildRand(signp, 2, 2, 3);
		fprintf(fp, "p cnf %d %d\n", nx, ns ) ;
		for( i = 1 ; i <= ns ; i ++ )
		{
			int ca = rand() % 3 ;
			int k = 0 ;
			for( k = -2 ; k <= ca; k++)
			{
			if( myRand(signp, 2)%2 - 1 ) fprintf(fp, "-") ;
			fprintf(fp, "%d ", rand()%nx + 1 ) ;
			}
			if( myRand(signp, 2)%2 - 1 ) fprintf(fp, "-") ;
			fprintf(fp, "%d 0\n", rand()%nx + 1 ) ;
		}
		fclose(fp) ;
	}
}
