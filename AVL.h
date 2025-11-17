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

// Right rotation (single rotation) - 向上旋轉
AVLNode* RotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights (bottom-up)
    UpdateHeight(y);
    UpdateHeight(x);

    return x;
}

// Left rotation (single rotation) - 向上旋轉
AVLNode* RotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights (bottom-up)
    UpdateHeight(x);
    UpdateHeight(y);

    return y;
}

// Insert helper - inserts at leaf then balances upward (向上旋轉策略)
AVLNode* InsertAVLHelper(AVLNode* node, int id, int score) {
    // Standard BST insertion (insert at leaf)
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

    // Update height of current node (going upward)
    UpdateHeight(node);

    // Get balance factor to check if this node became unbalanced
    int balance = GetBalance(node);

    // Left-Left Case - rotate right
    if (balance > 1 && id < node->left->id) {
        return RotateRight(node);
    }

    // Right-Right Case - rotate left
    if (balance < -1 && id > node->right->id) {
        return RotateLeft(node);
    }

    // Left-Right Case - double rotation
    if (balance > 1 && id > node->left->id) {
        node->left = RotateLeft(node->left);
        return RotateRight(node);
    }

    // Right-Left Case - double rotation
    if (balance < -1 && id < node->right->id) {
        node->right = RotateRight(node->right);
        return RotateLeft(node);
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
