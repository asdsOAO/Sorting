# 資料結構 作業一
## 41243230 莊晶瑞
## 41243239 陳裕翔



## 解題說明

本題要求設計一個複合排序函式，在任何資料大小與情境下都能維持良好的最壞情況效能。為達成此目標，需實作四種排序演算法並進行比較分析，最終依照測試結果組合出最佳的排序策略。

### 解題策略

寫出插入、快速、合併、堆疊四種排序函式，並保證它們在邏輯上正確

產生用來測試排序效率的資料型態，插入排序最壞的情況為反向排序資料，快速排序、堆積排序使用多個隨機排列，取其中最慢的當作近似最壞情況。

測量不同資料大小的排序時間都要測試一次，並記錄下來。如果資料筆數很小，導致排序時間小於時鐘解析度，則多次重複測試並取平均時間。

## 程式實作

### main.cpp
```cpp
#include <iostream>
#include "sorts.h"

using namespace std;
using namespace std::chrono;

// 插入排序
void insertion_sort(vector<int> a) {
    for (size_t i = 1; i < a.size(); ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

// 快速排序 使用 median-of-three
int median_of_three(vector<int>& a, int low, int high) {
    int mid = low + (high - low) / 2;
    if (a[high] < a[low]) swap(a[high], a[low]);
    if (a[mid] < a[low]) swap(a[mid], a[low]);
    if (a[high] < a[mid]) swap(a[high], a[mid]);
    return mid;
}

void quick_sort_recursive(vector<int> a, int low, int high) {
    if (low >= high) return;
    int pivotIndex = median_of_three(a, low, high);
    swap(a[pivotIndex], a[high]);
    int pivot = a[high];
    int i = low;
    for (int j = low; j < high; ++j) {
        if (a[j] < pivot)
            swap(a[i++], a[j]);
    }
    swap(a[i], a[high]);
    quick_sort_recursive(a, low, i - 1);
    quick_sort_recursive(a, i + 1, high);
}

void quick_sort(vector<int> a) {
    quick_sort_recursive(a, 0, a.size() - 1);
}

// 迭代版 Merge Sort
void iterative_merge_sort(vector<int> a) {
    int n = a.size();
    vector<int> temp(n);
    for (int width = 1; width < n; width *= 2) {
        for (int i = 0; i < n; i += 2 * width) {
            int left = i;
            int mid = min(i + width, n);
            int right = min(i + 2 * width, n);
            int k = left, l = left, r = mid;
            while (l < mid && r < right)
                temp[k++] = (a[l] <= a[r]) ? a[l++] : a[r++];
            while (l < mid) temp[k++] = a[l++];
            while (r < right) temp[k++] = a[r++];
        }
        swap(a, temp);
    }
}

// Heap Sort
void heap_sort(vector<int> a) {
    make_heap(a.begin(), a.end());
    sort_heap(a.begin(), a.end());
}

// 資料生成函式
vector<int> generate_random(int n) {
    vector<int> a(n);
    iota(a.begin(), a.end(), 1);
    random_device rd;
    mt19937 g(rd());
    shuffle(a.begin(), a.end(), g);
    return a;
}

vector<int> generate_reverse(int n) {
    vector<int> a(n);
    iota(a.begin(), a.end(), 1);
    reverse(a.begin(), a.end());
    return a;
}

// Merge Sort 
vector<int> generate_merge_worst_case(int n) {
    if (n <= 1) return { 1 };
    vector<int> left = generate_merge_worst_case(n / 2);
    vector<int> right = generate_merge_worst_case(n - n / 2);
    vector<int> result;
    size_t i = 0, j = 0;
    while (i < left.size() || j < right.size()) {
        if (i < left.size()) result.push_back(left[i++]);
        if (j < right.size()) result.push_back(right[j++]);
    }
    return result;
}

// 計時函式
microseconds measure_time(function<void()> f) {
    auto start = high_resolution_clock::now();
    f();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

int main() {
    vector<int> ns = { 500, 1000, 2000, 3000, 4000, 5000 };
    const int trials = 10;


    for (int n : ns) {
        cout << "\n陣列大小: " << n << "\n";

        // Insertion Sort
        auto arr = generate_reverse(n);
        auto t1 = measure_time([&]() {
            insertion_sort(arr);
            });
        cout << "插入排序：" << t1.count() << " 微秒\n";

        // Merge Sort
        arr = generate_merge_worst_case(n);
        auto t2 = measure_time([&]() {
            iterative_merge_sort(arr);
            });
        cout << "合併排序：" << t2.count() << " 微秒\n";

        // Heap Sort 
        long long heap_max = 0;
        for (int i = 0; i < trials; ++i) {
            arr = generate_random(n);
            auto t = measure_time([&]() {
                heap_sort(arr);
                });
            heap_max = max(heap_max, t.count());
        }
        cout << "堆疊排序：" << heap_max << " 微秒\n";

        // Quick Sort 
        long long quick_max = 0;
        for (int i = 0; i < trials; ++i) {
            arr = generate_random(n);
            auto t = measure_time([&]() {
                quick_sort(arr);
                });
            quick_max = max(quick_max, t.count());
        }
        cout << "快速排序：" << quick_max << " 微秒\n";
    }

    return 0;
}

```

### sorts.h
```cpp
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

```

## 效能分析

### 時間複雜度與空間複雜度比較

| 演算法 | 最佳情況 | 平均情況 | 最壞情況 | 空間複雜度 |
|---------|---------|---------|---------|---------|
| 插入排序 | O(n) | O(n^2) | O(n^2) | O(1) |
| 快速排序 | O(n log n) | O(n log n) | O(n^2) | O(logn) |
| 合併排序 | O(n log n) | O(n log n) | O(n log n) | O(n) |
| 堆疊排序 | O(n log n) | O(n log n) | O(n log n) | O(1) |

## 測試與驗證

### 測試案例

插入排序
| 測試資料 $n$ | 插入排序 | 合併排序 | 堆疊排序 | 快速排序 |
|----------|--------------|--------------|--------------|--------------|
| $n = 500$   | $1338µs$|$82µs$|$83µs$|$649µs$|
| $n = 1000$   | $5163µs$|$108µs$|$168µs$|$1217µs$|
| $n = 2000$   | $22480µs$|$233µs$|$721µs$|$4707µs$|
| $n = 3000$   | $45459µs$|$382µs$|$1008µs$|$3869µs$|
| $n = 4000$   | $83032µs$|$485µs$|$759µs$|$11016µs$|
| $n = 5000$   | $142483µs$|$670µs$|$1246µs$|$37243µs$|

## 程式簡單介紹


#### 標頭檔 (sorts.h)
使用 #ifndef, #define, #endif 來確保標頭檔不會被多次包含。

#### 插入排序
從第2個元素開始，將每個元素插入到已排序的部分中。
內部使用 while 迴圈將較大的元素推到右邊，直到找到插入位置。

#### 快速排序
median_of_three 函式選擇中位數作為基準元素，這可以幫助避免最壞情況的情形（例如，已排序或反排序的陣列）。
quick_sort_recursive 進行遞迴排序，將陣列分成兩部分並對每部分繼續排序。

####  迭代合併排序
使用迭代來合併兩兩小區段，直到全部合併成一個排序過的陣列。

#### 堆疊排序
make_heap 將陣列轉換為堆結構。
sort_heap 將堆結構排序並產生有序的陣列。

##### 資料生成函式
generate_random 用 iota 生成 1 到 n 的數列，然後隨機打亂。
generate_reverse 生成一個反向排序的數列。
generate_merge_worst_case 生成一個最壞情況的合併排序資料。

#### 計時函式
使用 high_resolution_clock::now() 來記錄函式執行前後的時間，然後計算時間差，單位是微秒。
