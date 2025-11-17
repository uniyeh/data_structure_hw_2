#ifndef AVL_H
#define AVL_H

#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

// AVL Node structure
struct AVLNode {
    int id;
    std::vector<int> scores;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int id, int score) : id(id), left(nullptr), right(nullptr), height(1) {
        scores.push_back(score);
    }
};

// Get height of node
int GetHeight(AVLNode* node) {
    return node ? node->height : 0;
}

// Update height of node
void UpdateHeight(AVLNode* node) {
    if (node) {
        node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
    }
}

// Get balance factor
int GetBalance(AVLNode* node) {
    return node ? GetHeight(node->left) - GetHeight(node->right) : 0;
}

// Trinode restructuring - 向上旋轉 (unified for all rotation cases)
// Identifies 3 nodes involved in imbalance, moves middle value to top
AVLNode* Restructure(AVLNode* z, AVLNode* y, AVLNode* x) {
    // z = grandparent (unbalanced node)
    // y = parent (taller child of z)
    // x = child (taller grandchild of z)

    // Determine the configuration and restructure
    // Goal: middle value node becomes root, smaller left, larger right

    AVLNode *a, *b, *c;  // Will be sorted by id: a < b < c
    AVLNode *T0, *T1, *T2, *T3;  // The 4 subtrees

    // Identify which case and assign a, b, c accordingly
    if (y == z->left && x == y->left) {
        // LL case: z > y > x
        //     z (c)
        //    /
        //   y (b)
        //  /
        // x (a)
        a = x; b = y; c = z;
        T0 = a->left; T1 = a->right; T2 = b->right; T3 = c->right;
    }
    else if (y == z->left && x == y->right) {
        // LR case: z > x > y
        //   z (c)
        //  /
        // y (a)
        //  \
        //   x (b)
        a = y; b = x; c = z;
        T0 = a->left; T1 = b->left; T2 = b->right; T3 = c->right;
    }
    else if (y == z->right && x == y->left) {
        // RL case: x > z > y
        //   z (a)
        //    \
        //     y (c)
        //    /
        //   x (b)
        a = z; b = x; c = y;
        T0 = a->left; T1 = b->left; T2 = b->right; T3 = c->right;
    }
    else {  // y == z->right && x == y->right
        // RR case: x > y > z
        //   z (a)
        //    \
        //     y (b)
        //      \
        //       x (c)
        a = z; b = y; c = x;
        T0 = a->left; T1 = b->left; T2 = c->left; T3 = c->right;
    }

    // Restructure: b becomes root, a left child, c right child
    b->left = a;
    b->right = c;
    a->left = T0;
    a->right = T1;
    c->left = T2;
    c->right = T3;

    // Update heights (bottom-up)
    UpdateHeight(a);
    UpdateHeight(c);
    UpdateHeight(b);

    return b;  // Middle value node becomes new root
}

// Insert helper - inserts at leaf then restructures upward (向上旋轉策略)
AVLNode* InsertAVLHelper(AVLNode* node, int id, int score) {
    // 1. Standard BST insertion (insert at leaf)
    if (node == nullptr) {
        return new AVLNode(id, score);
    }

    if (id < node->id) {
        node->left = InsertAVLHelper(node->left, id, score);
    } else if (id > node->id) {
        node->right = InsertAVLHelper(node->right, id, score);
    } else {
        // Same id, add score to existing node
        node->scores.push_back(score);
        return node;
    }

    // 2. Update height of current node (going upward)
    UpdateHeight(node);

    // 3. Check if this node became unbalanced
    int balance = GetBalance(node);

    // 4. If unbalanced, identify the 3 nodes and restructure
    if (balance > 1) {
        // Left subtree is taller
        AVLNode* y = node->left;
        AVLNode* x = (GetHeight(y->left) >= GetHeight(y->right)) ? y->left : y->right;
        return Restructure(node, y, x);
    }

    if (balance < -1) {
        // Right subtree is taller
        AVLNode* y = node->right;
        AVLNode* x = (GetHeight(y->right) >= GetHeight(y->left)) ? y->right : y->left;
        return Restructure(node, y, x);
    }

    return node;
}

// CreateAVL - 生成僅包含id與score的資料結構，並return其root
AVLNode* CreateAVL(int id, int score) {
    return new AVLNode(id, score);
}

// InsertAVL - 將(id, score)插入資料結構中，return更新後的root
AVLNode* InsertAVL(int id, int score, AVLNode* root) {
    return InsertAVLHelper(root, id, score);
}

// Print helper
void PrintAVLHelper(AVLNode* node, int depth = 0) {
    if (node == nullptr) return;

    PrintAVLHelper(node->right, depth + 1);

    for (int i = 0; i < depth; i++) std::cout << "    ";
    std::cout << "ID:" << node->id << " H:" << node->height << " Scores:[";
    for (size_t i = 0; i < node->scores.size(); i++) {
        std::cout << node->scores[i];
        if (i < node->scores.size() - 1) std::cout << ",";
    }
    std::cout << "]" << std::endl;

    PrintAVLHelper(node->left, depth + 1);
}

// PrintAVL - 列印資料結構中的所有節點資訊
void PrintAVL(AVLNode* root) {
    std::cout << "AVL Tree Structure:" << std::endl;
    if (root == nullptr) {
        std::cout << "(empty)" << std::endl;
    } else {
        PrintAVLHelper(root);
    }
    std::cout << std::endl;
}

// HeightAVL - 輸出資料結構的高度
int HeightAVL(AVLNode* root) {
    return GetHeight(root);
}

// Search helper
AVLNode* SearchAVLHelper(AVLNode* node, int id) {
    if (node == nullptr) return nullptr;

    if (id < node->id) {
        return SearchAVLHelper(node->left, id);
    } else if (id > node->id) {
        return SearchAVLHelper(node->right, id);
    } else {
        return node;
    }
}

// SearchAVGAVL - 輸出該id所有score的平均（如無資料，則輸出-1）
double SearchAVGAVL(AVLNode* root, int id) {
    AVLNode* node = SearchAVLHelper(root, id);
    if (node == nullptr) {
        return -1.0;
    }

    double sum = std::accumulate(node->scores.begin(), node->scores.end(), 0.0);
    return sum / node->scores.size();
}

// Cleanup
void DestroyAVL(AVLNode* node) {
    if (node) {
        DestroyAVL(node->left);
        DestroyAVL(node->right);
        delete node;
    }
}

#endif // AVL_H
