/* ************************************************************************
> File Name:     stress_test.cc
> Author:        ryan
> Mail:          xieg678@baidu.com
> Created Time:  Sun 30 Jul 2023 20:03:04 PM CST
> Description:
 ************************************************************************/

#include <ctime>
#include <chrono>
#include <cstdlib>
#include <iostream>

#include <pthread.h>

#include "skiplist.hpp"

constexpr int NUM_THREADS = 1;
constexpr int TEST_COUNT = 100000;
SkipList<int, std::string> skipList(18);

void* InsertElement(void* threadid) {
  long tid;
  tid = (long)threadid;
  std::cout << tid << std::endl;
  int tmp = TEST_COUNT / NUM_THREADS;
  for (int i = tid * tmp, count = 0; count < tmp; i++) {
    count++;
    skipList.InsertElement(rand() % TEST_COUNT, "a");
  }
  pthread_exit(nullptr);
}

void* GetElement(void* threadid) {
  long tid;
  tid = (long)threadid;
  std::cout << tid << std::endl;
  int tmp = TEST_COUNT / NUM_THREADS;
  for (int i = tid * tmp, count = 0; count < tmp; i++) {
    count++;
    skipList.SearchElement(rand() % TEST_COUNT);
  }
  pthread_exit(nullptr);
}

int main() {
  srand(time(nullptr));
  {
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    auto start = std::chrono::high_resolution_clock::now();

    for (i = 0; i < NUM_THREADS; i++) {
      std::cout << "main() : creating thread, " << i << std::endl;
      rc = pthread_create(&threads[i], nullptr, InsertElement, (void*)i);

      if (rc) {
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
      }
    }

    void* ret;
    for (i = 0; i < NUM_THREADS; i++) {
      if (pthread_join(threads[i], &ret) != 0) {
        perror("pthread_create() error");
        exit(3);
      }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "insert elapsed:" << elapsed.count() << std::endl;
  }
  // skipList.displayList();

  // {
  //     pthread_t threads[NUM_THREADS];
  //     int rc;
  //     int i;
  //     auto start = std::chrono::high_resolution_clock::now();

  //     for( i = 0; i < NUM_THREADS; i++ ) {
  //         std::cout << "main() : creating thread, " << i << std::endl;
  //         rc = pthread_create(&threads[i], nullptr, getElement, (void *)i);

  //         if (rc) {
  //             std::cout << "Error:unable to create thread," << rc <<
  //             std::endl; exit(-1);
  //         }
  //     }

  //     void *ret;
  //     for( i = 0; i < NUM_THREADS; i++ ) {
  //         if (pthread_join(threads[i], &ret) !=0 )  {
  //             perror("pthread_create() error");
  //             exit(3);
  //         }
  //     }

  //     auto finish = std::chrono::high_resolution_clock::now();
  //     std::chrono::duration<double> elapsed = finish - start;
  //     std::cout << "get elapsed:" << elapsed.count() << std::endl;
  // }

  pthread_exit(nullptr);
  return 0;
}
