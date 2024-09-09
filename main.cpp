#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define MINIMUM_CACHE_LINE_SIZE 64

int calculateOptimalNodeCapacity(int elements_number)
{   
    float memory_capacity = sizeof(int) * elements_number;
    float cache_lines_number = ceil(memory_capacity / MINIMUM_CACHE_LINE_SIZE);
    return (cache_lines_number + 1) * 2;
}


class Node
{
public:
    Node *next_;

    Node(int node_capacity)
    {
        next_ = nullptr;
        size_ = 0;
        capacity_ = node_capacity;
        array_ = new int[capacity_];
    }
    
    ~Node()
    {
        delete[] array_;
    }

    int size()
    {
        return size_;
    }

    int& operator[](int index)
    {
        if (index < 0 || index >= size_) {
            // throw error
        }
        return array_[index];
    }

    void insert(int index, int value)
    {
        if (size_ == capacity_) {
            // throw error
            return;
        }

        index = std::max(0, std::min(size_, index));

        for (int i = size_; i > index; i--) {
            array_[i] = array_[i-1];
        }
        array_[index] = value;
        size_++;
    }

    void pop(int index)
    {
        if (size_ == 0 || index < 0 || index >= size_) {
            // throw error
            return;
        }

        for (int i = index; i < size_-1; i++) {
            array_[i] = array_[i+1];
        }
        size_--;
    }

    void relocateReservedPartToNewNode(Node *new_node)
    {
        for (int i = capacity_/2; i < size_; i++) {
            new_node->insert(new_node->size_, array_[i]);
        }

        size_ = capacity_/2;
    }


    void relocateToCurrentNodeFromNext(Node *next_node)
    {
        insert(size_, (*next_node)[0]);
        
        for (int i = 0; i < next_node->size_ - 1; i++) {
            (*next_node)[i] = (*next_node)[i+1];
        }
        next_node->size_ -= 1;
    }

    void mergeNodes(Node *next_node)
    {
        for (int i = 0; i < next_node->size_; i++) {
            insert(size_, (*next_node)[i]);
        }
        next_ = next_node->next_;
    }

private:
    int *array_;
    int capacity_;
    int size_;
};


class UnrolledLinkedList
{
public:
    UnrolledLinkedList(int elements_number)
    {
        node_capacity_ = calculateOptimalNodeCapacity(elements_number);
        size_ = 0;
        head_ = nullptr;
    }
    ~UnrolledLinkedList() = default;
    

    int size()
    {
        return size_;
    }

    int getNodeCapacity()
    {
        return node_capacity_;
    }

    int& operator[](int index)
    {
        if (index < 0 || index >= size_) {
            // throw error

        }

        Node *current = head_;
        while (index >= current->size()) {
            index -= current->size();
            current = current->next_;
        }
        return (*current)[index];
    }

    void insert(int index, int value)
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

            if (index < node_capacity_/2) {
                current->insert(index, value);
            } else {
                current->next_->insert(index - current->size(), value);
            }

            
        } else {
            current->insert(index, value);
        }

        size_++;
    }

    void pushBack(int value)
    {
        insert(size_, value);
    }
    
    void pushFront(int value)
    {
        insert(0, value);
    }

    void pop(int index)
    {
        if (index < 0 || index >= size_) {
            // throw error

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
                    current->relocateToCurrentNodeFromNext(current->next_);
                }
            }
        }
        size_--;

        if (size_ == 0) {
            head_ = nullptr;
        } else {
            if (current->size() == 0) {
                Node *current = head_;
                while (current->next_->next_ != nullptr) {
                    current = current->next_;
                }
                current->next_ = nullptr;
            }
        }
    }

    void popFront()
    {
        pop(0);
    }

    void popBack()
    {
        pop(size_-1);
    }

    int find(int value)
    {
        int counter = 0;
        Node *current = head_;
        while (current != nullptr) {
            for (int i = 0; i < current->size(); i++) {
                if ( (*current)[i] == value ) {
                    return counter;
                }
                counter++;
            }
            current = current->next_;

        }
        return -1;
    }

    void show()
    {
        std::cout << "SHOOOOOOW" << std::endl;
        Node *current = head_;
        int q = 0;
        while (current != nullptr) {
            std::cout << "Node " << q << ": ";
            for (int i = 0; i < current->size(); i++) {
                std::cout << (*current)[i];
                if (i != current->size()-1) {
                    std::cout << " ";
                }
            }
            q++;
            std::cout << std::endl;
            current = current->next_;
        }
    }

private:
    Node *head_;
    int size_;
    int node_capacity_;
};


int main()
{
    std::string input_array;
    std::getline(std::cin, input_array);
    std::vector<int> a;
    int j = 0;
    for (int i=0; i<(int)input_array.size(); i++) {
        if (input_array[i] == ' ') {
            a.push_back( stoi(input_array.substr(j, i-j)) );
            j = i + 1;
        }
    }
    a.push_back( stoi(input_array.substr(j, input_array.size()-j)) );

    UnrolledLinkedList ull(a.size());
    for (int i=0; i<(int)a.size(); i++) {
        ull.pushBack(a[i]);
    }

    ull.show();

    return 0;
}