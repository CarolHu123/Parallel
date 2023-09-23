#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
//#define MAX_NUMBER 100 // 最大整数值
//#define NUM_THREADS 4 // 线程数

int primes[20000000]; // 存储素数的数组
int MAX_NUMBER=0;
int NUM_THREADS=0;
void* mark_primes(void *arg) {
    int *args = (int *)arg;
    int start = args[0];
    int end = args[1];
    
    for (int i = 2; i <= sqrt(MAX_NUMBER); i++) {
        for (int j = fmax(i * i, (start / i) * i); j < end; j += i) {
            primes[j] = 1; // 标记非素数
        }
    }
}

int main() {
    // 顺序预处理，计算最大数的平方根以内的素数
    // ...
   
    printf("please input MAX_NUMBER and NUM_THREADS:");
    scanf("%d%d",&MAX_NUMBER,&NUM_THREADS);
    // 将范围划分为不同的块，并分配线程
    int chunk_size = (MAX_NUMBER + 1) / NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    int args[NUM_THREADS][2]; // 存储每个线程的参数
    
    clock_t start_t,finish_t;
    start_t = clock();
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i][0] = i * chunk_size;
        args[i][1] = (i == NUM_THREADS - 1) ? MAX_NUMBER + 1 : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, mark_primes, args[i]);
    }
    
    // 等待线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    finish_t = clock();
    // 收集未标记的数字并打印素数
   /* for (int i = 2; i <= MAX_NUMBER; i++) {
        if (primes[i] == 0) {
            printf("%d ", i); // 输出素数
        }
    }
    printf("\n");
    */
    double time_sum = (double)(finish_t - start_t) / CLOCKS_PER_SEC;//将时间转换为秒
    printf("CPU 占用的总时间：%f\n", time_sum);
    return 0;
}

