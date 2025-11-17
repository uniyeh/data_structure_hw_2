#ifndef BST_H
#define BST_H

#include <vector>
#include <iostream>
#include <numeric>

// BST Node structure
struct BSTNode {
    int id;                    // key
    std::vector<int> scores;   // values (can have multiple scores per id)
    BSTNode* left;
    BSTNode* right;

    BSTNode(int id, int score) : id(id), left(nullptr), right(nullptr) {
        scores.push_back(score);
    }
};

// Helper function for insertion
BSTNode* InsertBSTHelper(BSTNode* root, int id, int score) {
    if (root == nullptr) {
        return new BSTNode(id, score);
    }

    if (id < root->id) {
        root->left = InsertBSTHelper(root->left, id, score);
    } else if (id > root->id) {
        root->right = InsertBSTHelper(root->right, id, score);
    } else {
        // Same id, add score to existing node
        root->scores.push_back(score);
    }

    return root;
}

// CreateBST - 生成僅包含id與score的資料結構，並return其root
BSTNode* CreateBST(int id, int score) {
    return new BSTNode(id, score);
}

// InsertBST - 將(id, score)插入資料結構中，return更新後的root
BSTNode* InsertBST(int id, int score, BSTNode* root) {
    return InsertBSTHelper(root, id, score);
}

// Helper function for printing
void PrintBSTHelper(BSTNode* node, int depth = 0) {
    if (node == nullptr) return;

    PrintBSTHelper(node->right, depth + 1);

    for (int i = 0; i < depth; i++) std::cout << "    ";
    std::cout << "ID:" << node->id << " Scores:[";
    for (size_t i = 0; i < node->scores.size(); i++) {
        std::cout << node->scores[i];
        if (i < node->scores.size() - 1) std::cout << ",";
    }
    std::cout << "]" << std::endl;

    PrintBSTHelper(node->left, depth + 1);
}

// PrintBST - 列印資料結構中的所有節點資訊
void PrintBST(BSTNode* root) {
    std::cout << "BST Structure:" << std::endl;
    if (root == nullptr) {
        std::cout << "(empty)" << std::endl;
    } else {
        PrintBSTHelper(root);
    }
    std::cout << std::endl;
}

// Helper function for height calculation
int HeightBSTHelper(BSTNode* node) {
    if (node == nullptr) return 0;
    return 1 + std::max(HeightBSTHelper(node->left), HeightBSTHelper(node->right));
}

// HeightBST - 輸出資料結構的高度
int HeightBST(BSTNode* root) {
    return HeightBSTHelper(root);
}

// Helper function for searching
BSTNode* SearchBSTHelper(BSTNode* node, int id) {
    if (node == nullptr) return nullptr;

    if (id < node->id) {
        return SearchBSTHelper(node->left, id);
    } else if (id > node->id) {
        return SearchBSTHelper(node->right, id);
    } else {
        return node;
    }
}

// SearchAVGBST - 輸出該id所有score的平均（如無資料，則輸出-1）
double SearchAVGBST(BSTNode* root, int id) {
    BSTNode* node = SearchBSTHelper(root, id);
    if (node == nullptr) {
        return -1.0;
    }

    double sum = std::accumulate(node->scores.begin(), node->scores.end(), 0.0);
    return sum / node->scores.size();
}

// Helper function for cleanup (optional utility)
void DestroyBST(BSTNode* node) {
    if (node) {
        DestroyBST(node->left);
        DestroyBST(node->right);
        delete node;
    }
}

#endif // BST_H
