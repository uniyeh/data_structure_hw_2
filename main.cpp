#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <random>

#include "BST.h"
#include "AVL.h"
#include "Treap.h"
#include "SkipList.h"

using namespace std;
using namespace chrono;

// Data generation structure
struct DataPoint {
    int id;
    int score;
};

// Function to generate random data
vector<DataPoint> generateRandomData(int n, unsigned seed) {
    vector<DataPoint> data;
    data.reserve(n);
    mt19937 gen(seed);
    uniform_int_distribution<> id_dist(1, 1 << 20); // 2^20
    uniform_int_distribution<> score_dist(0, 100);

    for (int i = 0; i < n; i++) {
        data.push_back({id_dist(gen), score_dist(gen)});
    }
    return data;
}

// Function to generate random search keys
vector<int> generateSearchKeys(int m, unsigned seed) {
    vector<int> keys;
    keys.reserve(m);
    mt19937 gen(seed);
    uniform_int_distribution<> id_dist(1, 1 << 20);

    for (int i = 0; i < m; i++) {
        keys.push_back(id_dist(gen));
    }
    return keys;
}


int main() {
    // --- Experiment Configuration ---
    vector<int> n_values;
    for (int i = 10; i <= 20; ++i) {
        n_values.push_back(pow(2, i));
    }

    const int RUNS = 10; // Number of runs to average over
    const int SEARCH_COUNT = 100000000; // Number of searches in one batch (100 million)
    const int BENCH_LOOPS = 5; // Run benchmark loop 5 times and take the minimum to reduce noise

    // --- CSV Header ---
    cout << "DataStructure,N,Run,InsertionTime_us,Height,AvgSearchTime_ns" << endl;

    // --- Main Experiment Loop ---
    for (int n : n_values) {
        cerr << "Processing N = " << n << "..." << endl;
        for (int run = 1; run <= RUNS; ++run) {
            cerr << "  Run " << run << "/" << RUNS << "..." << endl;
            
            unsigned seed = time(0) + run;
            vector<DataPoint> data = generateRandomData(n, seed);
            vector<int> search_keys = generateSearchKeys(SEARCH_COUNT, seed + 1);

            // --- BST Test ---
            {
                BSTNode* root = nullptr;
                auto start_insert = high_resolution_clock::now();
                for (const auto& dp : data) {
                    root = InsertBST(dp.id, dp.score, root);
                }
                auto end_insert = high_resolution_clock::now();
                long long insert_time = duration_cast<microseconds>(end_insert - start_insert).count();
                int height = HeightBST(root);

                long long min_duration_ns = -1;
                for (int i = 0; i < BENCH_LOOPS; ++i) {
                    auto start_search = high_resolution_clock::now();
                    for (int key : search_keys) {
                        SearchAVGBST(root, key);
                    }
                    auto end_search = high_resolution_clock::now();
                    long long current_duration = duration_cast<nanoseconds>(end_search - start_search).count();
                    if (min_duration_ns == -1 || current_duration < min_duration_ns) {
                        min_duration_ns = current_duration;
                    }
                }
                double avg_search_time_ns = static_cast<double>(min_duration_ns) / SEARCH_COUNT;

                cout << "BST," << n << "," << run << "," << insert_time << "," << height << "," << fixed << setprecision(2) << avg_search_time_ns << endl;
                DestroyBST(root);
            }

            // --- AVL Test ---
            {
                AVLNode* root = nullptr;
                auto start_insert = high_resolution_clock::now();
                for (const auto& dp : data) {
                    root = InsertAVL(dp.id, dp.score, root);
                }
                auto end_insert = high_resolution_clock::now();
                long long insert_time = duration_cast<microseconds>(end_insert - start_insert).count();
                int height = HeightAVL(root);

                long long min_duration_ns = -1;
                for (int i = 0; i < BENCH_LOOPS; ++i) {
                    auto start_search = high_resolution_clock::now();
                    for (int key : search_keys) {
                        SearchAVGAVL(root, key);
                    }
                    auto end_search = high_resolution_clock::now();
                    long long current_duration = duration_cast<nanoseconds>(end_search - start_search).count();
                    if (min_duration_ns == -1 || current_duration < min_duration_ns) {
                        min_duration_ns = current_duration;
                    }
                }
                double avg_search_time_ns = static_cast<double>(min_duration_ns) / SEARCH_COUNT;

                cout << "AVL," << n << "," << run << "," << insert_time << "," << height << "," << fixed << setprecision(2) << avg_search_time_ns << endl;
                DestroyAVL(root);
            }

            // --- Treap Test ---
            {
                EnsureTreapSeed();
                TreapNode* root = nullptr;
                auto start_insert = high_resolution_clock::now();
                for (const auto& dp : data) {
                    root = InsertTreap(dp.id, dp.score, root);
                }
                auto end_insert = high_resolution_clock::now();
                long long insert_time = duration_cast<microseconds>(end_insert - start_insert).count();
                int height = HeightTreap(root);

                long long min_duration_ns = -1;
                for (int i = 0; i < BENCH_LOOPS; ++i) {
                    auto start_search = high_resolution_clock::now();
                    for (int key : search_keys) {
                        SearchAVGTreap(root, key);
                    }
                    auto end_search = high_resolution_clock::now();
                    long long current_duration = duration_cast<nanoseconds>(end_search - start_search).count();
                    if (min_duration_ns == -1 || current_duration < min_duration_ns) {
                        min_duration_ns = current_duration;
                    }
                }
                double avg_search_time_ns = static_cast<double>(min_duration_ns) / SEARCH_COUNT;

                cout << "Treap," << n << "," << run << "," << insert_time << "," << height << "," << fixed << setprecision(2) << avg_search_time_ns << endl;
                DestroyTreap(root);
            }

            // --- Skip List Tests ---
            float probabilities[] = {0.5f, 0.25f, 0.75f};
            string sl_names[] = {"SkipList_p05", "SkipList_p025", "SkipList_p075"};

            for (int i = 0; i < 3; ++i) {
                skiplist_probability = probabilities[i];
                skiplist_max_level = 0;
                EnsureSkipListSeed();

                SkipListNode* header = nullptr;
                auto start_insert = high_resolution_clock::now();
                for (const auto& dp : data) {
                    if (header == nullptr) {
                        header = CreateSkipList(dp.id, dp.score);
                    } else {
                        header = InsertSkipList(dp.id, dp.score, header);
                    }
                }
                auto end_insert = high_resolution_clock::now();
                long long insert_time = duration_cast<microseconds>(end_insert - start_insert).count();
                int height = HeightSkipList(header);

                long long min_duration_ns = -1;
                for (int j = 0; j < BENCH_LOOPS; ++j) {
                    auto start_search = high_resolution_clock::now();
                    for (int key : search_keys) {
                        SearchAVGSkipList(header, key);
                    }
                    auto end_search = high_resolution_clock::now();
                    long long current_duration = duration_cast<nanoseconds>(end_search - start_search).count();
                    if (min_duration_ns == -1 || current_duration < min_duration_ns) {
                        min_duration_ns = current_duration;
                    }
                }
                double avg_search_time_ns = static_cast<double>(min_duration_ns) / SEARCH_COUNT;

                cout << sl_names[i] << "," << n << "," << run << "," << insert_time << "," << height << "," << fixed << setprecision(2) << avg_search_time_ns << endl;
                DestroySkipList(header);
            }
        }
    }

    cerr << "Experiment finished." << endl;
    return 0;
}