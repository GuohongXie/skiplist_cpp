#include <iostream>
#include <chrono>
#include "skiplist.hpp" 

struct PerformanceData {
    int num_elements;
    double time;
    double qps;
};

PerformanceData TestInsertPerformance(int num_elements) {
    SkipList<int, int> skiplist(18);
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_elements; ++i) {
        int key = rand();
        int value = rand();
        skiplist.InsertElement(key, value);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    double qps = num_elements / diff.count() / 10000;

    return {num_elements, diff.count(), qps};
}

int main() {
    std::cout << "### insert\n\n";
    std::cout << "skiplist tree high:18\n";
    std::cout << "insert random key\n\n";
    std::cout << "|insert element num (w) | timecost (s)  |\n";
    std::cout << "|---|---|\n";

    PerformanceData data1 = TestInsertPerformance(100000);  // 10w
    PerformanceData data2 = TestInsertPerformance(500000);  // 50w
    PerformanceData data3 = TestInsertPerformance(1000000); // 100w

    std::cout << "|" << data1.num_elements / 10000 << " |" << data1.time << " |\n";
    std::cout << "|" << data2.num_elements / 10000 << " |" << data2.time << " |\n";
    std::cout << "|" << data3.num_elements / 10000 << " |" << data3.time << " |\n";

    std::cout << "\nQPS Results:\n";
    std::cout << "10w: " << data1.qps << "w\n";
    std::cout << "50w: " << data2.qps << "w\n";
    std::cout << "100w: " << data3.qps << "w\n";

    return 0;
}

