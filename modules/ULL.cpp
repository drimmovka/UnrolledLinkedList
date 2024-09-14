#include "ULL.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

UnrolledLinkedList::UnrolledLinkedList()
{
    node_capacity_ = calculateOptimalNodeCapacity(0);
    size_ = 0;
    head_ = nullptr;
}

UnrolledLinkedList::UnrolledLinkedList(std::vector<int>& values)
{
    node_capacity_ = calculateOptimalNodeCapacity(values.size());
    size_ = values.size();

    if (values.size() == 0) {
        head_ = nullptr;
    } else {
        head_ = new Node(node_capacity_);
    }
    
    Node *current = head_;
    for (int i = 0; i < (int)values.size(); i++) {
        if (current->size() == node_capacity_/2) {
            Node *new_node = new Node(node_capacity_);
            current->next_ = new_node;
            current = new_node;
        }
        current->insert(current->size(), values[i]);
    }
}

UnrolledLinkedList::~UnrolledLinkedList() = default;

int UnrolledLinkedList::size()
{
    return size_;
}

int UnrolledLinkedList::getNodeCapacity()
{
    return node_capacity_;
}

int& UnrolledLinkedList::operator[](int index)
{
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Index out of range");
    }

    Node *current = head_;
    while (index >= current->size()) {
        index -= current->size();
        current = current->next_;
    }
    return (*current)[index];
}

void UnrolledLinkedList::insert(int index, int value)
{
    if (head_ == nullptr) {
        head_ = new Node(node_capacity_);
    }

    index = std::max(0, std::min(size_, index));

    Node *current = head_;
    while (index > current->size()) {
        index -= current->size();
        current = current->next_;
    }
    
    if (current->size() == node_capacity_) {
        Node *new_node = new Node(node_capacity_);
        current->relocateReservedPartToNewNode(new_node);
        
        new_node->next_ = current->next_;
        current->next_ = new_node;
        
        if (index <= node_capacity_/2) {
            current->insert(index, value);
        } else {
            current->next_->insert(index - current->size(), value);
        }
        
    } else {
        current->insert(index, value);
    }

    size_++;
}

void UnrolledLinkedList::pushFront(int value)
{
    insert(0, value);
}

void UnrolledLinkedList::pushBack(int value)
{
    insert(size_, value);
}

void UnrolledLinkedList::deleteLastNode()
{
    if (head_ == nullptr) {
        return;
    }

    Node *current = head_;
    while (current->next_->next_ != nullptr) {
        current = current->next_;
    }
    current->next_ = nullptr;
}

void UnrolledLinkedList::pop(int index)
{
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    
    Node *current = head_;
    while (index >= current->size()) {
        index -= current->size();
        current = current->next_;
    }
    current->pop(index);
    
    if (current->size() < node_capacity_/2) { 
        if (current->next_ != nullptr) {
            if ( current->next_->size() <= node_capacity_/2 ) {
                current->mergeNodes(current->next_);
            } else {
                current->relocateElementFromNext(current->next_);
            }
        }
    }
    size_--;

    if (size_ == 0) {
        head_ = nullptr;
    } else {
        if (current->size() == 0) {
            deleteLastNode();
        }
    }
}

void UnrolledLinkedList::popFront()
{
    pop(0);
}

void UnrolledLinkedList::popBack()
{
    pop(size_-1);
}

int UnrolledLinkedList::find(int value)
{
    int counter = 0;
    Node *current = head_;
    while (current != nullptr) {
        for (int i = 0; i < (int)current->size(); i++) {
            if ( (*current)[i] == value ) {
                return counter;
            }
            counter++;
        }
        current = current->next_;

    }
    return -1;
}

void UnrolledLinkedList::print()
{
    Node *current = head_;
    int node_number = 0;
    while (current != nullptr) {
        std::cout << "Node " << node_number << ":";
        for (int i = 0; i < current->size(); i++) {
            std::cout << " " << (*current)[i];
        }
        node_number++;
        std::cout << std::endl;
        current = current->next_;
    }
}

int UnrolledLinkedList::calculateOptimalNodeCapacity(int elements_number)
{   
    float memory_capacity = sizeof(int) * elements_number;
    float cache_lines_number = ceil(memory_capacity / MINIMUM_CACHE_LINE_SIZE);
    return (cache_lines_number + 1) * 2;
}