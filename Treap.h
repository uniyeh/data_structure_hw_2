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
    double priority;  // Random priority for heap property (0.0 ~ 1.0)
    TreapNode* left;
    TreapNode* right;

    TreapNode(int id, int score) : id(id), left(nullptr), right(nullptr) {
        scores.push_back(score);
        priority = (double)rand() / RAND_MAX;  // Random priority between 0 and 1
    }

    // Constructor with manual priority (for testing)
    TreapNode(int id, int score, double prio) : id(id), priority(prio), left(nullptr), right(nullptr) {
        scores.push_back(score);
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

// 向上旋轉 - Pull child up when it has higher priority
// Right rotation: left child rotates UP to become parent
TreapNode* RotateUp_Right(TreapNode* parent) {
    TreapNode* child = parent->left;

    // Restructure: child moves up, parent becomes right child
    parent->left = child->right;
    child->right = parent;

    return child;  // Child is now the root
}

// Left rotation: right child rotates UP to become parent
TreapNode* RotateUp_Left(TreapNode* parent) {
    TreapNode* child = parent->right;

    // Restructure: child moves up, parent becomes left child
    parent->right = child->left;
    child->left = parent;

    return child;  // Child is now the root
}

// Insert helper with manual priority - for testing
TreapNode* InsertTreapHelperWithPriority(TreapNode* node, int id, int score, double priority) {
    // 1. Standard BST insertion (insert at leaf)
    if (node == nullptr) {
        return new TreapNode(id, score, priority);
    }

    if (id < node->id) {
        node->left = InsertTreapHelperWithPriority(node->left, id, score, priority);

        // 2. Going upward - check heap property
        // If left child has higher priority, rotate it UP
        if (node->left->priority > node->priority) {
            return RotateUp_Right(node);  // Left child rotates up
        }
    } else if (id > node->id) {
        node->right = InsertTreapHelperWithPriority(node->right, id, score, priority);

        // 2. Going upward - check heap property
        // If right child has higher priority, rotate it UP
        if (node->right->priority > node->priority) {
            return RotateUp_Left(node);  // Right child rotates up
        }
    } else {
        // Same id, add score to existing node
        node->scores.push_back(score);
    }

    return node;
}

// Insert helper - inserts at leaf then rotates upward based on priority (向上旋轉策略)
TreapNode* InsertTreapHelper(TreapNode* node, int id, int score) {
    // 1. Standard BST insertion (insert at leaf)
    if (node == nullptr) {
        return new TreapNode(id, score);
    }

    if (id < node->id) {
        node->left = InsertTreapHelper(node->left, id, score);

        // 2. Going upward - check heap property
        // If left child has higher priority, rotate it UP
        if (node->left->priority > node->priority) {
            return RotateUp_Right(node);  // Left child rotates up
        }
    } else if (id > node->id) {
        node->right = InsertTreapHelper(node->right, id, score);

        // 2. Going upward - check heap property
        // If right child has higher priority, rotate it UP
        if (node->right->priority > node->priority) {
            return RotateUp_Left(node);  // Right child rotates up
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

// InsertTreap with manual priority - for testing
TreapNode* InsertTreapWithPriority(int id, int score, double priority, TreapNode* root) {
    return InsertTreapHelperWithPriority(root, id, score, priority);
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
