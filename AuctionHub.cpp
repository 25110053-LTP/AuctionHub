#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
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

// ---------- TODO: Implement these ----------
void insertionSort(vector<Bid>& v);          // Task A
void selectionSort(vector<Bid>& v);          // Task B
void interchangeSort(vector<Bid>& v);        // Task C
bool bubbleSortEarlyStop(vector<Bid>& v);    // Task D — returns true if stable (no swaps on a full pass)
void quickSort(vector<Bid>& v, int lo, int hi); // Task E

// ---------- Driver ----------
int main(int argc, char* argv[]) {
    // See per-task instructions.
    return 0;
}

static bool before(const Bid& a, const Bid& b) {
    if (a.amount != b.amount) return a.amount > b.amount;
    return a.timestamp < b.timestamp;
}

void insertionSort(vector<Bid>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        Bid key = v[i];
        size_t j = i;
        while (j > 0 && before(key, v[j - 1])) {
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
    for (size_t i = 0; i + 1 < v.size(); ++i)
        for (size_t j = i + 1; j < v.size(); ++j)
            if (before(v[j], v[i])) swap(v[i], v[j]);
}