#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

// Node structure for doubly linked list
struct CacheBlock {
    int address;
    int data;
    
    CacheBlock(int addr, int val) : address(addr), data(val) {}
};

class LRUCache {
private:
    int capacity;           // Cache size
    int hits;              // Number of cache hits
    int misses;            // Number of cache misses
    int totalAccesses;     // Total memory accesses
    
    // Doubly linked list to maintain LRU order (front = most recent, back = least recent)
    list<CacheBlock> cacheList;
    
    // Hash map for O(1) lookup: address -> iterator to list node
    unordered_map<int, list<CacheBlock>::iterator> cacheMap;
    
public:
    LRUCache(int size) : capacity(size), hits(0), misses(0), totalAccesses(0) {
        cout << "\n=== LRU Cache Simulator Initialized ===" << endl;
        cout << "Cache Capacity: " << capacity << " blocks" << endl;
        cout << "Replacement Policy: Least Recently Used (LRU)\n" << endl;
    }
    
    // Access memory address
    int access(int address, int data = -1) {
        totalAccesses++;
        
        // Check if address is in cache (HIT)
        if (cacheMap.find(address) != cacheMap.end()) {
            hits++;
            
            // Move accessed block to front (most recently used)
            auto it = cacheMap[address];
            CacheBlock block = *it;
            cacheList.erase(it);
            cacheList.push_front(block);
            cacheMap[address] = cacheList.begin();
            
            cout << "HIT  | Address: 0x" << hex << setw(4) << setfill('0') 
                 << address << dec << " | Data: " << block.data << endl;
            
            return block.data;
        }
        // Address not in cache (MISS)
        else {
            misses++;
            
            cout << "MISS | Address: 0x" << hex << setw(4) << setfill('0') 
                 << address << dec;
            
            // If cache is full, remove LRU block (back of list)
            if (cacheList.size() >= capacity) {
                int evictedAddr = cacheList.back().address;
                cout << " | Evicting: 0x" << hex << setw(4) << setfill('0') 
                     << evictedAddr << dec;
                
                cacheMap.erase(evictedAddr);
                cacheList.pop_back();
            }
            
            // Add new block to front (most recently used)
            int value = (data == -1) ? (address * 10) : data; // Simulate data
            cacheList.push_front(CacheBlock(address, value));
            cacheMap[address] = cacheList.begin();
            
            cout << " | Loaded: " << value << endl;
            
            return value;
        }
    }
    
    // Display current cache state
    void displayCache() {
        cout << "\n--- Current Cache State ---" << endl;
        cout << "Order: [Most Recent] -> [Least Recent]" << endl;
        
        if (cacheList.empty()) {
            cout << "Cache is empty." << endl;
            return;
        }
        
        int pos = 0;
        for (const auto& block : cacheList) {
            cout << "Block " << pos++ << ": Addr=0x" << hex << setw(4) 
                 << setfill('0') << block.address << dec 
                 << ", Data=" << block.data << endl;
        }
        cout << endl;
    }
    
    // Display performance statistics
    void displayStats() {
        cout << "\n========== Performance Statistics ==========" << endl;
        cout << "Total Accesses:  " << totalAccesses << endl;
        cout << "Cache Hits:      " << hits << endl;
        cout << "Cache Misses:    " << misses << endl;
        
        if (totalAccesses > 0) {
            double hitRate = (double)hits / totalAccesses * 100;
            double missRate = (double)misses / totalAccesses * 100;
            
            cout << fixed << setprecision(2);
            cout << "Hit Rate:        " << hitRate << "%" << endl;
            cout << "Miss Rate:       " << missRate << "%" << endl;
        }
        
        cout << "Cache Utilization: " << cacheList.size() << "/" << capacity 
             << " blocks" << endl;
        cout << "==========================================\n" << endl;
    }
    
    // Reset statistics
    void reset() {
        hits = 0;
        misses = 0;
        totalAccesses = 0;
        cacheList.clear();
        cacheMap.clear();
        cout << "Cache reset complete.\n" << endl;
    }
};

// Demonstration function
void runDemo() {
    cout << "\n***** LRU Cache Simulator Demo *****\n" << endl;
    
    // Create cache with capacity of 4 blocks
    LRUCache cache(4);
    
    // Test sequence 1: Basic access pattern
    cout << "Test 1: Sequential Access Pattern" << endl;
    cout << "-----------------------------------" << endl;
    cache.access(0x1000, 100);
    cache.access(0x2000, 200);
    cache.access(0x3000, 300);
    cache.access(0x4000, 400);
    cache.displayCache();
    
    // Test sequence 2: Accessing existing item (should be HIT)
    cout << "\nTest 2: Re-accessing Address 0x2000" << endl;
    cout << "------------------------------------" << endl;
    cache.access(0x2000);
    cache.displayCache();
    
    // Test sequence 3: Cache overflow (LRU eviction)
    cout << "\nTest 3: Cache Overflow - Eviction Test" << endl;
    cout << "---------------------------------------" << endl;
    cache.access(0x5000, 500);  // Should evict 0x1000 (LRU)
    cache.displayCache();
    
    // Test sequence 4: Multiple hits and misses
    cout << "\nTest 4: Mixed Access Pattern" << endl;
    cout << "----------------------------" << endl;
    cache.access(0x3000);  // HIT
    cache.access(0x6000, 600);  // MISS - evicts 0x4000
    cache.access(0x2000);  // HIT
    cache.access(0x7000, 700);  // MISS - evicts 0x5000
    cache.displayCache();
    
    // Display final statistics
    cache.displayStats();
}

// Interactive mode
void interactiveMode() {
    int cacheSize;
    cout << "\n***** Interactive LRU Cache Simulator *****\n" << endl;
    cout << "Enter cache size (number of blocks): ";
    cin >> cacheSize;
    
    LRUCache cache(cacheSize);
    
    while (true) {
        cout << "\nOptions:" << endl;
        cout << "1. Access memory address" << endl;
        cout << "2. Display cache state" << endl;
        cout << "3. Display statistics" << endl;
        cout << "4. Reset cache" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            int addr, data;
            cout << "Enter address (decimal or 0x for hex): ";
            cin >> hex >> addr >> dec;
            cout << "Enter data value (-1 for auto): ";
            cin >> data;
            cache.access(addr, data);
        }
        else if (choice == 2) {
            cache.displayCache();
        }
        else if (choice == 3) {
            cache.displayStats();
        }
        else if (choice == 4) {
            cache.reset();
        }
        else if (choice == 5) {
            cout << "\nExiting simulator...\n" << endl;
            break;
        }
        else {
            cout << "Invalid choice. Try again." << endl;
        }
    }
}

int main() {
    cout << "╔════════════════════════════════════════╗" << endl;
    cout << "║   LRU CACHE SIMULATOR - C++            ║" << endl;
    cout << "║   Data Structures & Computer Arch      ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    cout << "\nSelect Mode:" << endl;
    cout << "1. Run Demonstration" << endl;
    cout << "2. Interactive Mode" << endl;
    cout << "Enter choice: ";
    
    int mode;
    cin >> mode;
    
    if (mode == 1) {
        runDemo();
    }
    else if (mode == 2) {
        interactiveMode();
    }
    else {
        cout << "Invalid choice. Running demo..." << endl;
        runDemo();
    }
    
    return 0;
}