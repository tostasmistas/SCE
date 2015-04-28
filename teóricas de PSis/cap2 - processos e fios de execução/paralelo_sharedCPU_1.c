 #define _GNU_SOURCE
 #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

/* compilar com a opção -std=c99 */

void * func_2(void * arg){
	long int n = (long int) arg;
	long int c = 0;
	long int sum=0;

	int j;
	cpu_set_t cpuset;
    pthread_t thread;

    thread = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET(((long int) arg)%2*2, &cpuset);
    pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);         
    

	n = n * 10000000;
	while(c < 10000000){
		sum = sum + n* n*n;
		n++;
		c ++;
	}	
	long int  * ret = malloc(sizeof(long int));
	*ret = sum; 

	pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
           

	for ( j = 0; j < 8; j++)
		if (CPU_ISSET(j, &cpuset))
			printf("THREAD %ld    CPU %d\n", (long int) arg, j);
			


	return ret;
}


void * func_1(void * arg){
	long int n = (long int) arg;
	int c = 0;
	long double sum=0.0;
	long double * ret;
	n = n * 10000000;
	
	int j;
	cpu_set_t cpuset;
    pthread_t thread;

    thread = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET(((long int) arg)%2*2, &cpuset);
    pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);         
              
	
	while(c < 100000000){
		sum = sum + sqrtl(sqrtl(n*1.0));
		n++;
		c ++;
	}	
	ret = malloc(sizeof(long double));
	*ret = sum;

	pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
           

	for ( j = 0; j < 8; j++)
		if (CPU_ISSET(j, &cpuset))
			printf("THREAD %ld    CPU %d\n", (long int) arg, j);
			
			
	return ret;
}

int main(){
	long double *ret1, *ret2;
	long double *ret3, *ret4;
	int t_inic, t_final;
	pthread_t t1[4];
	long double res;
	cpu_set_t cpuset;
	int j;
	pthread_t thread = pthread_self();
	int s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
           

	printf("Set returned by pthread_getaffinity_np() contained:\n");
	for ( j = 0; j < CPU_SETSIZE; j++)
		if (CPU_ISSET(j, &cpuset))
			printf("    CPU %d\n", j);
	t_inic = clock();
	
	printf("Floting point & integer threads (0 and 2) sharing the same Core (but in differenet hiperthreads) \n");
	printf("Floting point & integer threads (1 and 3) sharing the same Core (but in differenet hiperthreads) \n");

	pthread_create(&t1[0], NULL, func_1, (void*) 0);
	pthread_create(&t1[1], NULL, func_1, (void*) 1);
	pthread_create(&t1[2], NULL, func_2, (void*) 2);
	pthread_create(&t1[3], NULL, func_2, (void*) 3);
	
	pthread_join(t1[0], (void *) &ret1);
	pthread_join(t1[1], (void *) &ret2);
	pthread_join(t1[2], (void *) &ret3);
	pthread_join(t1[3], (void *) &ret4);




	t_final = clock();
	res = *ret1+*ret2+*ret3+*ret4;
	printf("%d %Lf\n",t_final- t_inic,  res);
	exit(0);
}
