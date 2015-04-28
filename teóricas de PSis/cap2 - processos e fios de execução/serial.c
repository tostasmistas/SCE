#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
/* compilar com a opção -std=c99 */

long int func_2(long int n){
	long int c = 0;
	long int sum=0;
	n = n *   100000000;
	while(c < 100000000){
		sum = sum + n;
		n++;
		c ++;
	}	
	return sum;
}


long double func_1(long int n){
	int c = 0;
	long double sum=0;
	 n = n *  100000000.0;
	while(c < 100000000){
		sum = sum + sqrtl(sqrtl(n*1.0));
		n++;
		c ++;
	}	

	return sum;
}

int main(){
	int t_inic, t_final;
	long double  res1, res2, res;
	long int res3, res4;
	
	t_inic = clock();
	res1 = func_1(0);
	res2 = func_1(1);
	res3 = func_2(2);
	res4 = func_2(3);
	res = res1+ res2+ res3+res4;
	t_final = clock();
	printf("%d, %Lf\n", t_final- t_inic, res );

	exit(0);
}
