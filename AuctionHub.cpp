#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

struct Bid {
    string bidderId;
    double amount;       // bid price
    long long timestamp; // Unix ms
    int auctionId;
};

// ---------- Utilities ----------
void printBids(const vector<Bid>& v, size_t limit = 10) {
    for (size_t i = 0; i < min(v.size(), limit); ++i)
        cout << "[" << v[i].bidderId << "] $" << v[i].amount
             << " @ " << v[i].timestamp << "\n";
    if (v.size() > limit) cout << "... (" << v.size() << " total)\n";
}

bool loadBids(const string& path, vector<Bid>& out) {
    ifstream f(path);
    if (!f) return false;
    Bid b;
    while (f >> b.bidderId >> b.amount >> b.timestamp >> b.auctionId)
        out.push_back(b);
    return true;
}

bool lessAsc(const Bid& a, const Bid& b);
bool isSortedAsc(const vector<Bid>& v);
void quickSortWrapper(vector<Bid>& v);

// ---------- TODO: Implement these ----------
void insertionSort(vector<Bid>& v);          // Task A
void selectionSort(vector<Bid>& v);          // Task B
void interchangeSort(vector<Bid>& v);        // Task C
bool bubbleSortEarlyStop(vector<Bid>& v);    // Task D — returns true if stable (no swaps on a full pass)
void quickSort(vector<Bid>& v, int lo, int hi); // Task E

// ---------- Driver ----------
int main(int argc, char* argv[]) {
    // See per-task instructions.

      if (argc >= 2) {
        vector<Bid> data;
        if (!loadBids(argv[1], data)) {
            cerr << "Cannot open " << argv[1] << "\n";
            return 1;
        }
        cout << "Loaded " << data.size() << " bids from " << argv[1] << "\n";
        auto t0 = steady_clock::now();
        quickSortWrapper(data);
        double ms = duration<double, milli>(steady_clock::now() - t0).count();
        cout << "Time: " << ms << " ms, "<< (isSortedAsc(data) ? "SORTED OK" : "NOT SORTED!") << "\n";
        printBids(data, 5);
        return 0;
    }

    vector<Bid> base = {
        {"alice", 100.0, 1000, 1},
        {"bob",   102.5, 1005, 1},
        {"carol", 101.0, 1003, 1},
        {"dave",  103.0, 1010, 1}
    };

    // Task A
    cout << "== Task A: insertion (ascending) ==\n";
    vector<Bid> a = base;
    insertionSort(a);
    printBids(a);

    // Task B
    cout << "\n== Task B: selection (descending), Top 3 ==\n";
    vector<Bid> b = base;
    selectionSort(b);
    printBids(b, 3);

    // Task C
    cout << "\n== Task C: interchange (ascending) ==\n";
    vector<Bid> c = base;
    interchangeSort(c);
    printBids(c);

    // Task D
    cout << "\n== Task D: bubble early stop ==\n";
    vector<Bid> d1 = a;                      
    bool s1 = bubbleSortEarlyStop(d1);
    cout << "Sorted input   -> " << (s1 ? "STABLE" : "UNSTABLE") << "\n";
    vector<Bid> d2(a.rbegin(), a.rend());     
    bool s2 = bubbleSortEarlyStop(d2);
    cout << "Reversed input -> " << (s2 ? "STABLE" : "UNSTABLE") << "\n";

    // Task E
    cout << "\n== Task E: quicksort, 100,000 bids ==\n";
    mt19937 rng(42);
    uniform_real_distribution<double> price(1.0, 1000.0);
    vector<Bid> big;
    big.reserve(100000);
    for (int i = 0; i < 100000; ++i)
        big.push_back({"user" + to_string(i % 1000), price(rng), (long long)i, 1});
    auto t0 = steady_clock::now();
    quickSortWrapper(big);
    double ms = duration<double, milli>(steady_clock::now() - t0).count();
    cout << "Time: " << ms << " ms, " << (isSortedAsc(big) ? "SORTED OK" : "NOT SORTED!") << "\n";
    return 0;

}

bool lessAsc(const Bid& a, const Bid& b) {
    if (a.amount != b.amount) return a.amount < b.amount;
    return a.timestamp < b.timestamp;
}

static bool before(const Bid& a, const Bid& b) {
    if (a.amount != b.amount) return a.amount > b.amount;
    return a.timestamp < b.timestamp;
}

bool isSortedAsc(const vector<Bid>& v) {
    for (size_t i = 1; i < v.size(); ++i)
        if (lessAsc(v[i], v[i - 1])) return false;
    return true;
}

void insertionSort(vector<Bid>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        Bid key = v[i];
        size_t j = i;
        while (j > 0 && lessAsc(key, v[j - 1])) {
            v[j] = v[j - 1];
            --j;
        }
        v[j] = key;
    }
}

void selectionSort(vector<Bid>& v) {
    for (size_t i = 0; i + 1 < v.size(); ++i) {
        size_t m = i;
        for (size_t j = i + 1; j < v.size(); ++j)
            if (before(v[j], v[m])) m = j;
        if (m != i) swap(v[i], v[m]);
    }
}

void interchangeSort(vector<Bid>& v) {
    long long swaps = 0;
    for (size_t i = 0; i + 1 < v.size(); ++i)
        for (size_t j = i + 1; j < v.size(); ++j)
            if (lessAsc(v[j], v[i])) {swap(v[i], v[j]), ++swaps; };
    cout << "Total swaps: " << swaps << "\n";
}

bool bubbleSortEarlyStop(vector<Bid>& v) {
    size_t n = v.size();
    bool firstPassNoSwap = false;
    for (size_t i = 0; i + 1 < n; ++i) {
        bool swapped = false;
        for (size_t j = 0; j + 1 < n - i; ++j) {
            if (lessAsc(v[j + 1], v[j])) {
                swap(v[j], v[j + 1]);
                swapped = true;
            }
        }
        if (i == 0) firstPassNoSwap = !swapped;  
        if (!swapped) break;                     
    }
    return (n <= 1) ? true : firstPassNoSwap;
}

void quickSort(vector<Bid>& v, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    if (lessAsc(v[mid], v[lo])) swap(v[mid], v[lo]);
    if (lessAsc(v[hi],  v[lo])) swap(v[hi],  v[lo]);
    if (lessAsc(v[hi],  v[mid])) swap(v[hi], v[mid]);
    Bid p = v[mid];                               

    int i = lo, j = hi;                           
    while (i <= j) {
        while (lessAsc(v[i], p)) ++i;
        while (lessAsc(p, v[j])) --j;
        if (i <= j) { swap(v[i], v[j]); ++i; --j; }
    }
    if (lo < j) quickSort(v, lo, j);
    if (i < hi) quickSort(v, i, hi);
}

void quickSortWrapper(vector<Bid>& v) {
    if (!v.empty()) quickSort(v, 0, (int)v.size() - 1);
}