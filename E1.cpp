#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int NUM_THREADS=0; // thread number
int TOTAL_STEPS=0;// total interval
pthread_mutex_t mutex;
double result = 0.0; 
int steps_per_thread = 0;
double step = 0;
//  f(x) = 4 / (1 + x^2)
double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// help message
void usage(char* usagename){
    fprintf(stderr,"usage: %s,<number of threads>, total steps\n",usagename);
    fprintf(stderr,"number of threads shall greater than 0 less than 1024\n ");
    fprintf(stderr,"number of steps cannot less than or equal 0 \n ");
    exit(1);
}


// sum each part
void* partial_sum(void* a) {
    int thread_id = *(int*)a;
    int start_step = thread_id * steps_per_thread;
    int end_step = (thread_id == NUM_THREADS - 1) ? TOTAL_STEPS : (start_step + steps_per_thread);
    double partial_sum = 0.0;
    for (int i = start_step; i < end_step; i++) {
        double x = i*step;
        partial_sum += f(x)*step;
      //  printf("%f---%f---%f\n",x,partial_sum,f(x));
    }
    // add all to global variable
    pthread_mutex_lock(&mutex);
    result += partial_sum;
    pthread_mutex_unlock(&mutex);
    return 0;
}

int main(int argc, char* argv[]) 
{

    if(strcmp(argv[1], "-h") ==0){
        usage(argv[0]);
    }

    //printf("input NUM_THREADS and TOTAL_STEPS:\n");
   // scanf("%d%d",&NUM_THREADS,&TOTAL_STEPS);
    NUM_THREADS = atoi(argv[1]);
    TOTAL_STEPS = atoi(argv[2]);
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex,NULL);
    int thread_ids[NUM_THREADS];
    steps_per_thread = TOTAL_STEPS / NUM_THREADS;
    step = 1.0 / TOTAL_STEPS;
    // printf("%f\n",step);
    clock_t start_t,finish_t;
    start_t = clock();
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i],NULL, partial_sum, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    finish_t = clock();
    printf("the  total area about: %f \n", result);
    double time_sum = (double)(finish_t - start_t) / CLOCKS_PER_SEC;//将时间转换为秒
    printf("CPU time:%f\n", time_sum);
    pthread_mutex_destroy(&mutex);
    return 0;
}

