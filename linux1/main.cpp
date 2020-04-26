#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;
// 提示出租车到达的条件变量
pthread_cond_t taxiCond;


// 同步锁
pthread_mutex_t taxiMutex;


// 旅客人数，初始为 0
int travelerCount = 0;


// 旅客到达等待出租车
void* traveler_arrive(void* name) {
    cout << " Traveler: " << (char*)name << " needs a taxi now! " << endl;
    pthread_mutex_lock(&taxiMutex);


    // 提示旅客人数增加
    travelerCount++;
    pthread_cond_wait(&taxiCond, &taxiMutex);
    pthread_mutex_unlock(&taxiMutex);
    cout << " Traveler: " << (char*)name << " now got a taxi! " << endl;
    pthread_exit((void*)0);
}


// 出租车到达
void* taxi_arrive(void* name)
{
    cout << " Taxi " << (char*)name << " arrives. " << endl;
    while (true)
    {
        pthread_mutex_lock(&taxiMutex);
        // 当发现已经有旅客在等待时，才触发条件变量
        if (travelerCount > 0)
        {
            pthread_cond_signal(&taxiCond);
            pthread_mutex_unlock(&taxiMutex);
            break;
        }
        pthread_mutex_unlock(&taxiMutex);
    }


    pthread_exit((void*)0);
}


int  main() {
    // 初始化
    taxiCond = PTHREAD_COND_INITIALIZER;
    taxiMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_t thread;
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);


    pthread_create(&thread, &threadAttr, taxi_arrive, (void*)(" Jack "));
    sleep(1);
    pthread_create(&thread, &threadAttr, traveler_arrive, (void*)(" Susan "));
    sleep(1);
    pthread_create(&thread, &threadAttr, taxi_arrive, (void*)(" Mike "));
    sleep(1);


    return 0;
}