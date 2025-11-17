#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <vector>
#include <iostream>
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <climits>

static const int MAX_LEVEL = 32; // Maximum level for skip list
static bool skiplist_seeded = false;
static float skiplist_probability = 0.5f;
int skiplist_max_level = 0; // Global max level tracker (externally modifiable)

// Skip List Node structure
struct SkipListNode
{
    int id;
    std::vector<int> scores;
    std::vector<SkipListNode *> forward; // Forward pointers at each level
    bool is_header;                      // Flag to identify header node

    SkipListNode(int id, int score, int level) : id(id), is_header(false)
    {
        scores.push_back(score);
        forward.resize(level + 1, nullptr);
    }

    SkipListNode(int level) : id(INT_MIN), is_header(true)
    { // Header node
        forward.resize(level + 1, nullptr);
    }
};

void EnsureSkipListSeed()
{
    if (!skiplist_seeded)
    {
        srand(time(nullptr));
        skiplist_seeded = true;
    }
}

// Random level generator (coin flip approach)
int RandomLevel()
{
    int level = 0;
    while (((float)rand() / RAND_MAX) < skiplist_probability && level < MAX_LEVEL)
    {
        level++;
    }
    return level;
}

// Search for a node with given id (helper)
SkipListNode *SearchSkipListNode(SkipListNode *header, int id)
{
    SkipListNode *current = header;

    // Start from highest level and move down
    for (int i = skiplist_max_level; i >= 0; i--)
    {
        while (current->forward[i] != nullptr && current->forward[i]->id < id)
        {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != nullptr && current->id == id)
    {
        return current;
    }
    return nullptr;
}

// CreateSkipList - 生成僅包含id與score的資料結構，並return其root(header)
SkipListNode *CreateSkipList(int id, int score)
{
    EnsureSkipListSeed();

    SkipListNode *header = new SkipListNode(MAX_LEVEL);

    int level = RandomLevel();
    SkipListNode *newNode = new SkipListNode(id, score, level);

    // Update forward pointers
    for (int i = 0; i <= level; i++)
    {
        newNode->forward[i] = header->forward[i];
        header->forward[i] = newNode;
    }

    if (level > skiplist_max_level)
    {
        skiplist_max_level = level;
    }

    return header;
}

// InsertSkipList - 將(id, score)插入資料結構中，return更新後的root(header)
SkipListNode *InsertSkipList(int id, int score, SkipListNode *header)
{
    std::vector<SkipListNode *> update(MAX_LEVEL + 1, nullptr);
    SkipListNode *current = header;

    // Find position to insert
    for (int i = skiplist_max_level; i >= 0; i--)
    {
        while (current->forward[i] != nullptr && current->forward[i]->id < id)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    // If id already exists, add score to existing node
    if (current != nullptr && current->id == id)
    {
        current->scores.push_back(score);
    }
    else
    {
        // Create new node with random level
        int newLevel = RandomLevel();

        // Update skiplist_max_level if necessary
        if (newLevel > skiplist_max_level)
        {
            for (int i = skiplist_max_level + 1; i <= newLevel; i++)
            {
                update[i] = header;
            }
            skiplist_max_level = newLevel;
        }

        SkipListNode *newNode = new SkipListNode(id, score, newLevel);

        // Update forward pointers
        for (int i = 0; i <= newLevel; i++)
        {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    return header;
}

// PrintSkipList - 列印資料結構中的所有節點資訊
void PrintSkipList(SkipListNode *header)
{
    std::cout << "Skip List Structure:" << std::endl;

    if (header->forward[0] == nullptr)
    {
        std::cout << "(empty)" << std::endl;
        std::cout << std::endl;
        return;
    }

    // Print level by level
    for (int i = skiplist_max_level; i >= 0; i--)
    {
        SkipListNode *current = header->forward[i];
        std::cout << "Level " << i << ": ";
        while (current != nullptr)
        {
            std::cout << "ID:" << current->id << " ";
            current = current->forward[i];
        }
        std::cout << std::endl;
    }

    // Print detailed node information
    std::cout << "\nDetailed nodes:" << std::endl;
    SkipListNode *current = header->forward[0];
    while (current != nullptr)
    {
        std::cout << "ID:" << current->id << " Scores:[";
        for (size_t i = 0; i < current->scores.size(); i++)
        {
            std::cout << current->scores[i];
            if (i < current->scores.size() - 1)
                std::cout << ",";
        }
        std::cout << "] Height:" << current->forward.size() << std::endl;
        current = current->forward[0];
    }
    std::cout << std::endl;
}

// HeightSkipList - 輸出資料結構的高度（返回當前最大level + 1）
int HeightSkipList(SkipListNode *header)
{
    (void)header; // Unused parameter
    return skiplist_max_level + 1;
}

// SearchAVGSkipList - 輸出該id所有score的平均（如無資料，則輸出-1）
double SearchAVGSkipList(SkipListNode *header, int id)
{
    SkipListNode *node = SearchSkipListNode(header, id);
    if (node == nullptr)
    {
        return -1.0;
    }

    double sum = std::accumulate(node->scores.begin(), node->scores.end(), 0.0);
    return sum / node->scores.size();
}

// Cleanup
void DestroySkipList(SkipListNode *header)
{
    SkipListNode *current = header->forward[0];
    while (current != nullptr)
    {
        SkipListNode *next = current->forward[0];
        delete current;
        current = next;
    }
    delete header;
}

#endif // SKIPLIST_H
