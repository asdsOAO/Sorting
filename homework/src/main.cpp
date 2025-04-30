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

// Quick Sort 使用 median-of-three
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
        cout << "堆積排序：" << heap_max << " 微秒\n";

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
