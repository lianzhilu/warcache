#include <iostream>
#include <list>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#define CACHE_SIZE 16
#define PAGE_NUM 100

using namespace std;

class MRUCache {
private:
    int capacity;
    list<int> mru_list;
    unordered_map<int, list<int>::iterator> cache_map;

    void moveToHead(int key) {
        mru_list.erase(cache_map[key]);
        mru_list.push_front(key);
        cache_map[key] = mru_list.begin();
    }

public:
    MRUCache(int cap) : capacity(cap) {}

    bool get(int key) {
        if (capacity == 0) return false;
        auto it = cache_map.find(key);
        if (it == cache_map.end()) {
            return false;
        }
        moveToHead(key);
        return true;
    }

    void put(int key) {
        if (capacity == 0) return;
        if (get(key)) {
            return;
        }
        
        if (mru_list.size() >= capacity) {
            int victim_key = mru_list.front();
            mru_list.pop_front();
            cache_map.erase(victim_key);
        }

        mru_list.push_front(key);
        cache_map[key] = mru_list.begin();
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
    MRUCache cache(CACHE_SIZE);

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
