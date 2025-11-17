#ifndef TREAP_H
#define TREAP_H

#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Treap Node structure
struct TreapNode {
    int id;
    std::vector<int> scores;
    int priority;  // Random priority for heap property
    TreapNode* left;
    TreapNode* right;

    TreapNode(int id, int score) : id(id), left(nullptr), right(nullptr) {
        scores.push_back(score);
        priority = rand();  // Random priority
    }
};

// Static variable to ensure random seed is initialized once
static bool treap_seeded = false;

void EnsureTreapSeed() {
    if (!treap_seeded) {
        srand(time(nullptr));
        treap_seeded = true;
    }
}

// Right rotation - 向上旋轉
TreapNode* TreapRotateRight(TreapNode* y) {
    TreapNode* x = y->left;
    TreapNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

// Left rotation - 向上旋轉
TreapNode* TreapRotateLeft(TreapNode* x) {
    TreapNode* y = x->right;
    TreapNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

// Insert helper - inserts at leaf then rotates upward based on priority (向上旋轉策略)
TreapNode* InsertTreapHelper(TreapNode* node, int id, int score) {
    // Standard BST insertion (insert at leaf)
    if (node == nullptr) {
        return new TreapNode(id, score);
    }

    if (id < node->id) {
        node->left = InsertTreapHelper(node->left, id, score);

        // Rotate upward if child has higher priority (max-heap property)
        if (node->left->priority > node->priority) {
            return TreapRotateRight(node);
        }
    } else if (id > node->id) {
        node->right = InsertTreapHelper(node->right, id, score);

        // Rotate upward if child has higher priority (max-heap property)
        if (node->right->priority > node->priority) {
            return TreapRotateLeft(node);
        }
    } else {
        // Same id, add score to existing node
        node->scores.push_back(score);
    }

    return node;
}

// CreateTreap - 生成僅包含id與score的資料結構，並return其root
TreapNode* CreateTreap(int id, int score) {
    EnsureTreapSeed();
    return new TreapNode(id, score);
}

// InsertTreap - 將(id, score)插入資料結構中，return更新後的root
TreapNode* InsertTreap(int id, int score, TreapNode* root) {
    return InsertTreapHelper(root, id, score);
}

// Calculate height
int HeightTreapHelper(TreapNode* node) {
    if (node == nullptr) return 0;
    return 1 + std::max(HeightTreapHelper(node->left), HeightTreapHelper(node->right));
}

// HeightTreap - 輸出資料結構的高度
int HeightTreap(TreapNode* root) {
    return HeightTreapHelper(root);
}

// Print helper
void PrintTreapHelper(TreapNode* node, int depth = 0) {
    if (node == nullptr) return;

    PrintTreapHelper(node->right, depth + 1);

    for (int i = 0; i < depth; i++) std::cout << "    ";
    std::cout << "ID:" << node->id << " P:" << node->priority << " Scores:[";
    for (size_t i = 0; i < node->scores.size(); i++) {
        std::cout << node->scores[i];
        if (i < node->scores.size() - 1) std::cout << ",";
    }
    std::cout << "]" << std::endl;

    PrintTreapHelper(node->left, depth + 1);
}

// PrintTreap - 列印資料結構中的所有節點資訊
void PrintTreap(TreapNode* root) {
    std::cout << "Treap Structure:" << std::endl;
    if (root == nullptr) {
        std::cout << "(empty)" << std::endl;
    } else {
        PrintTreapHelper(root);
    }
    std::cout << std::endl;
}

// Search helper
TreapNode* SearchTreapHelper(TreapNode* node, int id) {
    if (node == nullptr) return nullptr;

    if (id < node->id) {
        return SearchTreapHelper(node->left, id);
    } else if (id > node->id) {
        return SearchTreapHelper(node->right, id);
    } else {
        return node;
    }
}

// SearchAVGTreap - 輸出該id所有score的平均（如無資料，則輸出-1）
double SearchAVGTreap(TreapNode* root, int id) {
    TreapNode* node = SearchTreapHelper(root, id);
    if (node == nullptr) {
        return -1.0;
    }

    double sum = std::accumulate(node->scores.begin(), node->scores.end(), 0.0);
    return sum / node->scores.size();
}

// Cleanup
void DestroyTreap(TreapNode* node) {
    if (node) {
        DestroyTreap(node->left);
        DestroyTreap(node->right);
        delete node;
    }
}

#endif // TREAP_H
