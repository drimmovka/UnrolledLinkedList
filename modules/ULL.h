#ifndef ULL_H
#define ULL_H

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#define MINIMUM_CACHE_LINE_SIZE 64


class UnrolledLinkedList
{
public:
    UnrolledLinkedList();

    UnrolledLinkedList(std::vector<int>& values);

    ~UnrolledLinkedList();
    
    int size();

    int getNodeCapacity();

    int& operator[](int index);

    void insert(int index, int value);

    void pushFront(int value);

    void pushBack(int value);

    void pop(int index);

    void popFront();

    void popBack();
    
    int find(int value);

    void print();

private:
    class Node
    {
    public:
        Node *next_;

        Node(int node_capacity);
        
        ~Node();

        int size();

        int& operator[](int index);

        void insert(int index, int value);

        void pop(int index);

        void relocateReservedPartToNewNode(Node *new_node);

        void relocateElementFromNext(Node *next_node);

        void mergeNodes(Node *next_node);

    private:
        int *array_;
        int capacity_;
        int size_;
    };

    int calculateOptimalNodeCapacity(int elements_number);

    void deleteLastNode();
    
    Node *head_;
    int size_;
    int node_capacity_;
};

#endif