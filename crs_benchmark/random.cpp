#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#define CACHE_SIZE 16
#define MY_PAGE_SIZE 4096
#define PAGE_NUM 100

using namespace std;

class RandomCache {
private:
    int capacity;
    unordered_map<int, size_t> key_map;
    vector<int> cache_keys;

public:
    RandomCache(int cap) : capacity(cap) {
        srand(time(0));
    }

    bool get(int key) {
        return key_map.find(key) != key_map.end();
    }

    void put(int key) {
        if (capacity <= 0) return;
        if (get(key)) return;

        if (cache_keys.size() >= capacity) {
            int random_index = rand() % cache_keys.size();
            int victim_key = cache_keys[random_index];

            key_map.erase(victim_key);
            if (random_index != cache_keys.size() - 1) {
                cache_keys[random_index] = cache_keys.back();
                key_map[cache_keys.back()] = random_index;
            }
            cache_keys.pop_back();
        }

        cache_keys.push_back(key);
        key_map[key] = cache_keys.size() - 1;
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
    RandomCache cache(CACHE_SIZE);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < 4096; ++j) {
            pages[i][j] = 'a' + (i + j) % 26;
        }
    }

    for (int i = 0; i < iterNum; ++i) {
        int page_num = rand() % N;
        if (!cache.get(page_num)) {
            for (int i = 0; i < 1000; i++) ;
            cache.put(page_num);
        }
    }

    return 0;
}
