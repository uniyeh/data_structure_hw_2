# Data Structure Comparison - HW2

This project compares the performance and characteristics of four different tree-based data structures:
- **Binary Search Tree (BST)** - no rotation after insertion
- **AVL Tree** - self-balancing with upward rotation
- **Treap** - randomized tree with upward rotation
- **Skip List** - probabilistic multi-level linked list

## Project Structure

```
HW2/
├── BST.h         # Binary Search Tree implementation
├── AVL.h         # AVL Tree implementation
├── Treap.h       # Treap implementation
├── SkipList.h    # Skip List implementation
├── main.cpp      # Test program
├── Makefile      # Build configuration
└── README.md     # This file
```

## Features

Each data structure implements the following operations:

1. **Constructor** - Create structure with initial (id, score) pair
2. **Insert** - Insert new (id, score) pair
   - If id already exists, adds score to that node's score list
3. **Print** - Display the structure (tree/list visualization)
4. **Height** - Return the height of the structure
5. **SearchAVG** - Return average of all scores for a given id
   - Returns -1 if id doesn't exist

## Implementation Details

### BST (Binary Search Tree)
- Simple insertion without any balancing
- O(n) worst-case height for sorted input
- O(log n) average height for random input

### AVL Tree
- **Upward rotation strategy**: Insert at leaf position, then propagate upward fixing imbalances
- Maintains balance factor: |height(left) - height(right)| ≤ 1
- Guaranteed O(log n) height
- Four rotation cases: LL, RR, LR, RL

### Treap
- **Upward rotation strategy**: Insert at leaf, rotate upward based on random priority
- Combines BST (by id) with max-heap (by random priority)
- Expected O(log n) height
- Randomization provides good average-case performance

### Skip List
- Probabilistic multi-level linked list
- Each node has random height (geometric distribution, p=0.5)
- Expected O(log n) height and search time
- Simpler to implement than balanced trees

## Compilation

```bash
# Compile the project
make

# Or compile manually
g++ -std=c++11 -Wall -Wextra -O2 main.cpp -o main

# Run the program
make run
# Or
./main

# Clean build files
make clean
```

## Usage Example

```cpp
#include "BST.h"
#include "AVL.h"
#include "Treap.h"
#include "SkipList.h"

// Create structures
BST bst(1, 85);           // Student id=1, score=85
AVL avl(1, 85);
Treap treap(1, 85);
SkipList skiplist(1, 85);

// Insert more data
bst.Insert(2, 90);
bst.Insert(1, 92);        // Same id, adds another score

// Get average score
double avg = bst.SearchAVG(1);  // Returns (85+92)/2 = 88.5

// Get height
int h = bst.Height();

// Print structure
bst.Print();
```

## Test Results

The `main.cpp` program runs two test scenarios:

### Small Dataset (15 elements)
- Visualizes the tree structures
- Verifies correctness of all operations

### Large Dataset (10,000 elements)
- Measures insertion time
- Compares heights
- Demonstrates performance characteristics

**Expected Results:**
- **AVL**: Smallest height (~14-16 for 10k elements)
- **Skip List**: Similar to AVL (~13-15)
- **Treap**: Slightly larger (~25-30)
- **BST**: Varies greatly depending on input order

## Assignment Requirements

✅ **Input Generation**: id ~ UNI(1, 2^20), score ~ UNI(0, 100)

✅ **Required Functions** (as methods):
- `CreateXXX` → Constructor
- `InsertXXX` → Insert method
- `PrintXXX` → Print method
- `HeightXXX` → Height method
- `SearchAVGXXX` → SearchAVG method

✅ **Constraints**:
- AVL and Treap use **upward rotation only**
- Multiple scores per id supported
- Object-oriented design

## Performance Observations

From the 10,000 element test:

| Structure  | Height | Insertion Time | Notes |
|-----------|--------|----------------|-------|
| BST       | ~35    | Fast           | Unbalanced, poor worst-case |
| AVL       | ~16    | Medium         | Best height guarantee |
| Treap     | ~29    | Medium         | Good randomized performance |
| Skip List | ~14    | Slower         | Good height, simpler code |

## Key Concepts

### Upward Rotation (AVL & Treap)
Both AVL and Treap follow the "insert at leaf, then fix upward" strategy:
1. Insert node at leaf position (standard BST insertion)
2. Traverse back up the tree
3. Apply rotations to fix violations
   - AVL: Fix balance factor violations
   - Treap: Fix heap property violations

This satisfies the assignment's "upward rotation only" constraint.

### Multiple Scores per ID
Since `SearchAVG` needs to return the average of all scores for an id, each node stores a `vector<int>` of scores. When inserting a duplicate id, we append to this vector rather than creating a new node.

## Author
NCCU Data Structure HW2

## License
Educational use only
