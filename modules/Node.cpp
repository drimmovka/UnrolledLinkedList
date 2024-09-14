#include "ULL.h"

#include <algorithm>

UnrolledLinkedList::Node::Node(int node_capacity)
{
    next_ = nullptr;
    size_ = 0;
    capacity_ = node_capacity;
    array_ = new int[capacity_];
}

UnrolledLinkedList::Node::~Node()
{
    delete[] array_;
}

int UnrolledLinkedList::Node::size()
{
    return size_;
}

int& UnrolledLinkedList::Node::operator[](int index)
{
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return array_[index];
}

void UnrolledLinkedList::Node::insert(int index, int value)
{
    if (size_ == capacity_) {
        throw std::runtime_error("Not enough place to insert value");
    }

    index = std::max(0, std::min(size_, index));

    for (int i = size_; i > index; i--) {
        array_[i] = array_[i-1];
    }
    array_[index] = value;
    size_++;
}

void UnrolledLinkedList::Node::pop(int index)
{
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Index out of range");
    }

    for (int i = index; i < size_-1; i++) {
        array_[i] = array_[i+1];
    }
    size_--;
}

void UnrolledLinkedList::Node::relocateReservedPartToNewNode(Node *new_node)
{
    for (int i = capacity_/2; i < size_; i++) {
        new_node->insert(new_node->size_, array_[i]);
    }

    size_ = capacity_/2;
}


void UnrolledLinkedList::Node::relocateElementFromNext(Node *next_node)
{
    insert(size_, (*next_node)[0]);
    next_node->pop(0);
}

void UnrolledLinkedList::Node::mergeNodes(Node *next_node)
{
    for (int i = 0; i < next_node->size_; i++) {
        insert(size_, (*next_node)[i]);
    }
    next_ = next_node->next_;
}