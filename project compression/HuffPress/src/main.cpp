#include <iostream>
#include <memory>
#include "huffman/HuffmanNode.h"

// Because HuffmanNode.h already has "using namespace std",
// and we include it here, we get it here too.
// But it's good practice to also write it explicitly in every .cpp file
// so the file is self-contained and readable on its own.
using namespace std;

int main() {
    // make_shared<LeafNode> creates a LeafNode on the heap
    // and wraps it in a shared_ptr automatically.
    // This is better than writing: shared_ptr<LeafNode> l(new LeafNode(...))
    auto leaf1 = make_shared<LeafNode>('A', 45);
    auto leaf2 = make_shared<LeafNode>('B', 12);

    // InternalNode's frequency will be 45 + 12 = 57 automatically
    auto internal = make_shared<InternalNode>(leaf1, leaf2);

    cout << "=== HuffmanNode Test ===" << endl;

    cout << "leaf1 byte:      " << leaf1->byteValue << endl;
    cout << "leaf1 frequency: " << leaf1->frequency  << endl;
    cout << "leaf1 isLeaf:    " << leaf1->isLeaf()   << endl;

    cout << "leaf2 byte:      " << leaf2->byteValue << endl;
    cout << "leaf2 frequency: " << leaf2->frequency  << endl;

    cout << "internal freq:   " << internal->frequency << endl;
    cout << "internal isLeaf: " << internal->isLeaf()  << endl;

    // Polymorphism demo: we access a LeafNode through a HuffmanNode pointer.
    // C++ calls the CORRECT isLeaf() at runtime — this is dynamic dispatch.
    shared_ptr<HuffmanNode> basePtr = leaf1;
    cout << "basePtr isLeaf (should be 1): " << basePtr->isLeaf() << endl;

    return 0;
    // When main returns, all shared_ptrs go out of scope.
    // Reference counts drop to zero → memory freed automatically. No leaks.
}