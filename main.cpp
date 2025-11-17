#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include "BST.h"
#include "AVL.h"
#include "Treap.h"
#include "SkipList.h"

using namespace std;
using namespace chrono;

// Generate random data based on assignment specs
struct DataPoint {
    int id;
    int score;
};

vector<DataPoint> generateRandomData(int n, unsigned seed = 42) {
    vector<DataPoint> data;
    mt19937 gen(seed);
    uniform_int_distribution<> id_dist(1, 1048576);  // 2^20
    uniform_int_distribution<> score_dist(0, 100);

    for (int i = 0; i < n; i++) {
        data.push_back({id_dist(gen), score_dist(gen)});
    }
    return data;
}

// Test function for BST
void testBST(const vector<DataPoint>& data) {
    cout << "========== Testing BST ==========" << endl;

    // Create with first element using CreateBST
    BSTNode* root = CreateBST(data[0].id, data[0].score);

    auto start = high_resolution_clock::now();

    // Insert remaining elements using InsertBST
    for (size_t i = 1; i < data.size(); i++) {
        root = InsertBST(data[i].id, data[i].score, root);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Insertion time: " << duration.count() << " microseconds" << endl;
    cout << "Height: " << HeightBST(root) << endl;

    // Test search
    int testId = data[0].id;
    double avg = SearchAVGBST(root, testId);
    cout << "Average score for ID " << testId << ": " << avg << endl;

    // Test non-existent ID
    avg = SearchAVGBST(root, 999999999);
    cout << "Average score for non-existent ID: " << avg << endl;

    // Print tree structure (for small datasets)
    if (data.size() <= 15) {
        PrintBST(root);
    }

    cout << endl;

    // Cleanup
    DestroyBST(root);
}

// Test function for AVL
void testAVL(const vector<DataPoint>& data) {
    cout << "========== Testing AVL Tree ==========" << endl;

    // Create with first element using CreateAVL
    AVLNode* root = CreateAVL(data[0].id, data[0].score);

    auto start = high_resolution_clock::now();

    // Insert remaining elements using InsertAVL
    for (size_t i = 1; i < data.size(); i++) {
        root = InsertAVL(data[i].id, data[i].score, root);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Insertion time: " << duration.count() << " microseconds" << endl;
    cout << "Height: " << HeightAVL(root) << endl;

    int testId = data[0].id;
    double avg = SearchAVGAVL(root, testId);
    cout << "Average score for ID " << testId << ": " << avg << endl;

    avg = SearchAVGAVL(root, 999999999);
    cout << "Average score for non-existent ID: " << avg << endl;

    if (data.size() <= 15) {
        PrintAVL(root);
    }

    cout << endl;

    // Cleanup
    DestroyAVL(root);
}

// Test function for Treap
void testTreap(const vector<DataPoint>& data) {
    cout << "========== Testing Treap ==========" << endl;

    // Create with first element using CreateTreap
    TreapNode* root = CreateTreap(data[0].id, data[0].score);

    auto start = high_resolution_clock::now();

    // Insert remaining elements using InsertTreap
    for (size_t i = 1; i < data.size(); i++) {
        root = InsertTreap(data[i].id, data[i].score, root);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Insertion time: " << duration.count() << " microseconds" << endl;
    cout << "Height: " << HeightTreap(root) << endl;

    int testId = data[0].id;
    double avg = SearchAVGTreap(root, testId);
    cout << "Average score for ID " << testId << ": " << avg << endl;

    avg = SearchAVGTreap(root, 999999999);
    cout << "Average score for non-existent ID: " << avg << endl;

    if (data.size() <= 15) {
        PrintTreap(root);
    }

    cout << endl;

    // Cleanup
    DestroyTreap(root);
}

// Test function for Skip List
void testSkipList(const vector<DataPoint>& data) {
    cout << "========== Testing Skip List ==========" << endl;

    // Create with first element using CreateSkipList
    SkipListNode* header = CreateSkipList(data[0].id, data[0].score);

    auto start = high_resolution_clock::now();

    // Insert remaining elements using InsertSkipList
    for (size_t i = 1; i < data.size(); i++) {
        header = InsertSkipList(data[i].id, data[i].score, header);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Insertion time: " << duration.count() << " microseconds" << endl;
    cout << "Height: " << HeightSkipList(header) << endl;

    int testId = data[0].id;
    double avg = SearchAVGSkipList(header, testId);
    cout << "Average score for ID " << testId << ": " << avg << endl;

    avg = SearchAVGSkipList(header, 999999999);
    cout << "Average score for non-existent ID: " << avg << endl;

    if (data.size() <= 15) {
        PrintSkipList(header);
    }

    cout << endl;

    // Cleanup
    DestroySkipList(header);
}

int main() {
    cout << "Data Structure Comparison Assignment" << endl;
    cout << "=====================================" << endl << endl;

    // Test with small dataset first
    cout << "Testing with SMALL dataset (15 elements):" << endl;
    cout << "==========================================" << endl << endl;

    vector<DataPoint> smallData = generateRandomData(15, 42);

    testBST(smallData);
    testAVL(smallData);
    testTreap(smallData);
    testSkipList(smallData);

    // Reset skiplist_max_level for next test
    skiplist_max_level = 0;

    // Test with larger dataset
    cout << "\n\nTesting with LARGE dataset (10000 elements):" << endl;
    cout << "=============================================" << endl << endl;

    vector<DataPoint> largeData = generateRandomData(10000, 42);

    testBST(largeData);
    testAVL(largeData);
    testTreap(largeData);
    testSkipList(largeData);

    // Summary comparison
    cout << "\n========== SUMMARY ==========" << endl;
    cout << "All functions follow the specified signatures:" << endl;
    cout << "  - addr CreateXXX(int id, int score)" << endl;
    cout << "  - addr InsertXXX(int id, int score, addr root)" << endl;
    cout << "  - void PrintXXX(addr root)" << endl;
    cout << "  - int HeightXXX(addr root)" << endl;
    cout << "  - double SearchAVGXXX(addr root, int id)" << endl;
    cout << "\nAVL and Treap use upward rotation strategy:" << endl;
    cout << "  - Insert at leaf position first" << endl;
    cout << "  - Then rotate upward to maintain balance" << endl;
    cout << "\nPerformance characteristics:" << endl;
    cout << "  BST:       Simple, O(n) worst case height" << endl;
    cout << "  AVL Tree:  Strictly balanced, O(log n) guaranteed" << endl;
    cout << "  Treap:     Randomized, O(log n) expected" << endl;
    cout << "  Skip List: Probabilistic, O(log n) expected" << endl;

    return 0;
}
