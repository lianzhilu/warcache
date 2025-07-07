#include <iostream>
#include <unordered_map>
#include <list>
#include <cstdlib>
#include <ctime>
#define CACHE_SIZE 16 // 缓存大小
#define MY_PAGE_SIZE 4096
#define PAGE_NUM 100

using namespace std;

class LRUCache {
private:
    struct Node {
        int key;
        Node* prev;
        Node* next;
        Node(int k) : key(k), prev(nullptr), next(nullptr) {}
    };
    
    int capacity;
    Node* head;  // 虚拟头节点
    Node* tail;  // 虚拟尾节点
    unordered_map<int, Node*> cache_map;
    
    void addToHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
    
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }
    
    Node* removeTail() {
        Node* node = tail->prev;
        removeNode(node);
        return node;
    }

public:
    LRUCache(int cap) : capacity(cap) {
        head = new Node(-1);
        tail = new Node(-1);
        head->next = tail;
        tail->prev = head;
    }

    bool get(int key) {
        if (capacity == 0) return false;
        auto it = cache_map.find(key);
        if (it == cache_map.end()) {
            return false;
        }
        moveToHead(it->second);
        return true;
    }

    void put(int key) {
        if (capacity == 0) return;
        if (get(key)) {  // 已存在则更新位置
            return;
        }
        if (cache_map.size() >= capacity) {
            Node* removed = removeTail();
            cache_map.erase(removed->key);
            delete removed;
        }
        Node* newNode = new Node(key);
        cache_map[key] = newNode;
        addToHead(newNode);
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
    LRUCache cache(CACHE_SIZE);

    // 初始化页池
    initPages();

    // 模拟访问
    for (int i = 0; i < iterNum; ++i) {
        int page_num = rand() % N;
        if (!cache.get(page_num)) {
            for (int i = 0; i < 1000; i++) ;
            cache.put(page_num);
        }
    }

    return 0;
}
