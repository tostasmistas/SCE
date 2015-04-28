#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * func_1(void * arg){
	int * n = arg;
	while(1){
		printf("\t%d\n", *n);
	}	
}
int main(){
	int n;
	pthread_t t1;
	n = 10;
	
	pthread_create(&t1, NULL, func_1, &n);
	while(1){
		printf("%d\n", n);
		n = n+1;
	}
	exit(0);
}
