#include <iostream>
#include <list>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#define CACHE_SIZE 16
#define MY_PAGE_SIZE 4096
#define PAGE_NUM 100

using namespace std;

class FIFOCache {
private:
    int capacity;
    list<int> fifo_queue;
    unordered_map<int, list<int>::iterator> cache_map;

public:
    FIFOCache(int cap) : capacity(cap) {}

    bool get(int key) {
        return cache_map.find(key) != cache_map.end();
    }

    void put(int key) {
        if (capacity <= 0) return;
        if (get(key)) return;
        
        if (fifo_queue.size() >= capacity) {
            int oldest_key = fifo_queue.front();
            fifo_queue.pop_front();
            cache_map.erase(oldest_key);
        }

        fifo_queue.push_back(key);
        cache_map[key] = prev(fifo_queue.end());
    }
};

const int N = 100;
char pages[N][4096];
void initPages() {
    for (int i = 0; i < PAGE_NUM; ++i) {
        for (int j = 0; j < MY_PAGE_SIZE; ++j) {
            pages[i][j] = 'a' + (i + j) % 26;
        }
    }
}

int main(int argc, char **argv) {
    int iterNum = 10;
    if(argc >= 2) {
        iterNum = atol(argv[1]);
    }
    srand(time(0));
    FIFOCache cache(CACHE_SIZE);

    initPages();

    for (int i = 0; i < iterNum; ++i) {
        int page_num = rand() % N;
        if (!cache.get(page_num)) {
            for (int i = 0; i < 1000; i++) ;
            cache.put(page_num);
        }
    }

    return 0;
}
