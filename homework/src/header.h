#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include <chrono>
#include <functional>
#include <random>
#include <algorithm>
#include <numeric>  


// 排序函式宣告
void insertion_sort(std::vector<int> a);
void quick_sort(std::vector<int> a);
void iterative_merge_sort(std::vector<int> a);
void heap_sort(std::vector<int> a);

// 資料生成函式
std::vector<int> generate_random(int n);
std::vector<int> generate_reverse(int n);
std::vector<int> generate_merge_worst_case(int n);

// 計時函式
std::chrono::microseconds measure_time(std::function<void()> f);

#endif
