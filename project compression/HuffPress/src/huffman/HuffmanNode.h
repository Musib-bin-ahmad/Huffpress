#pragma once

// These are the standard library headers we need.
// memory  → gives us shared_ptr and make_shared
// iostream → gives us cout (used in main for testing)
#include <memory>
#include <iostream>

// This line means: whenever we write shared_ptr, it means std::shared_ptr
// Without this we'd have to write std::shared_ptr everywhere — tedious.
using namespace std;


// ─── ABSTRACT BASE CLASS ─────────────────────────────────────────────────────
// HuffmanNode is the parent of ALL nodes in the tree.
// It is ABSTRACT — you cannot create a HuffmanNode directly.
// What makes it abstract? The "= 0" on isLeaf() below.

class HuffmanNode {
public:
    // Every node (leaf or internal) stores a frequency.
    // frequency = how many times this byte (or group of bytes) appeared.
    int frequency;

    // Constructor: when any node is created, it must be given a frequency.
    // "explicit" means you cannot accidentally write: HuffmanNode n = 5;
    // You must write: HuffmanNode n(5); — forces intentional use.
    explicit HuffmanNode(int freq) : frequency(freq) {}

    // Virtual destructor.
    // Rule: if a class has ANY virtual function, always make the destructor virtual.
    // Why? If you hold a LeafNode* through a HuffmanNode* pointer and delete it,
    // without virtual, C++ calls the WRONG destructor → memory leak or crash.
    // "= default" means: compiler generates the usual destructor body for us.
    virtual ~HuffmanNode() = default;

    // Pure virtual function — the "= 0" makes this class abstract.
    // Every subclass MUST override this and provide their own version.
    // "const" means calling isLeaf() does not modify the object.
    [[nodiscard]] virtual bool isLeaf() const = 0;
};


// ─── LEAF NODE ────────────────────────────────────────────────────────────────
// A LeafNode represents ONE unique byte value in the file.
// Example: the byte 'A' (65) appeared 45 times → LeafNode('A', 45)
// It has NO children — it is a leaf of the tree.

class LeafNode : public HuffmanNode {
public:
    // The actual byte this leaf stands for.
    // unsigned char = values 0..255, covers every possible byte in any file.
    unsigned char byteValue;

    // Constructor takes the byte value and its frequency.
    // ": HuffmanNode(freq)" calls the parent constructor first — mandatory.
    LeafNode(unsigned char val, int freq)
        : HuffmanNode(freq), byteValue(val) {}

    // "override" tells the compiler: I am intentionally replacing the parent's
    // isLeaf(). If the parent had no such function, compiler gives an error.
    // A leaf always returns true — it IS a leaf.
    [[nodiscard]] bool isLeaf() const override { return true; }
};


// ─── INTERNAL NODE ────────────────────────────────────────────────────────────
// An InternalNode is NOT a leaf — it is a branch of the tree.
// It has a left child and a right child.
// Its frequency = left->frequency + right->frequency (sum of children).
// It does NOT store a byte value.

class InternalNode : public HuffmanNode {
public:
    // shared_ptr = smart pointer.
    // It automatically deletes the object when nobody points to it anymore.
    // No manual delete needed — no memory leaks.
    // We use shared_ptr<HuffmanNode> so it can point to EITHER a LeafNode
    // OR another InternalNode — polymorphism at work.
    shared_ptr<HuffmanNode> left;
    shared_ptr<HuffmanNode> right;

    // Constructor takes two child nodes (already built).
    // Its own frequency is automatically set to sum of children.
    // "move(l)" transfers ownership of the pointer without copying — efficient.
    InternalNode(shared_ptr<HuffmanNode> l,
                 shared_ptr<HuffmanNode> r)
        : HuffmanNode(l->frequency + r->frequency),
          left(std::move(l)),
          right(std::move(r)) {}

    // An internal node is never a leaf → always returns false.
    [[nodiscard]] bool isLeaf() const override { return false; }
};