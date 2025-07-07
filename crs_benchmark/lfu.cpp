#include <unordered_map>
#include <list>
#include <iostream>
#include <cstdlib>
#include <ctime>
#define CACHE_SIZE 16
#define MY_PAGE_SIZE 4096
#define PAGE_NUM 100

class LFUCache {
private:
    int capacity;
    int min_freq;
    std::unordered_map<int, std::pair<int, std::list<int>::iterator>> key_map;
    std::unordered_map<int, std::list<int>> freq_map;

    void updateMinFreq() {
        if (freq_map.empty()) {
            min_freq = 0;
            return;
        }
        min_freq = freq_map.begin()->first;
        for (const auto& pair : freq_map) {
            if (pair.first < min_freq) {
                min_freq = pair.first;
            }
        }
    }

public:
    LFUCache(int cap) : capacity(cap), min_freq(0) {}

    bool get(int key) {
        if (capacity == 0) return false;
        auto it = key_map.find(key);
        if (it == key_map.end()) {
            return false;
        }
        int freq = it->second.first;
        auto iter = it->second.second;
        freq_map[freq].erase(iter);
        if (freq_map[freq].empty()) {
            freq_map.erase(freq);
            if (freq == min_freq) {
                updateMinFreq();
            }
        }
        freq++;
        freq_map[freq].push_back(key);
        it->second.first = freq;
        it->second.second = --freq_map[freq].end();
        return true;
    }

    void put(int key) {
        if (capacity == 0) return;
        if (get(key)) {
            return;
        }
        if (key_map.size() >= capacity) {
            if (min_freq == 0) return;
            int key_to_remove = freq_map[min_freq].front();
            freq_map[min_freq].pop_front();
            if (freq_map[min_freq].empty()) {
                freq_map.erase(min_freq);
                updateMinFreq();
            }
            key_map.erase(key_to_remove);
        }
        freq_map[1].push_back(key);
        key_map[key] = {1, --freq_map[1].end()};
        if (min_freq == 0 || 1 < min_freq) {
            min_freq = 1;
        }
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
    LFUCache cache(CACHE_SIZE);

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
