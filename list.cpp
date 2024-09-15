#include <list>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

#define START                0
#define MIDDLE               1
#define END                  2

class List
{
public:
    List()
    {
        head_ = nullptr;
        size_ = 0;
    }
    ~List() = default;

    int size()
    {
        return size_;
    }

    int &operator[](int index)
    {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        Node *current = head_;
        while (index > 0) {
            current = current->next_;
            index--;
        }
        return current->data_;
    }

    void insert(int index, int value)
    {
        Node *new_node = new Node(value);
        size_++;

        index = std::max(0, std::min(size_, index));
        
        if (index == 0) {
            new_node->next_ = head_;
            head_ = new_node;
            return;
        }

        Node *current = head_;
                 
        while (index > 1) {
            current = current->next_;
            index--;
        }
        
        new_node->next_ = current->next_;
        current->next_ = new_node;
        
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
            throw std::out_of_range("Index out of range");
        }

        size_--;

        if (index == 0) {
            head_ = head_->next_;
            return;
        }

        Node *current = head_;
                 
        while (index > 1) {
            current = current->next_;
            index--;
        }
        
        current->next_ = current->next_->next_;
    }

    void popBack()
    {
        pop(size_-1);
    }

    void popFront()
    {
        pop(0);
    }

    int find(int value)
    {
        Node *current = head_;
        int index = 0;
        while (current != nullptr) {
            if (current->data_ == value) {
                return index;
            }
            current = current->next_;
            index++;
        }
        return -1;
    }

private:
    class Node
    {
    public:
        int data_;
        Node* next_;

        Node(int value)
        {
            data_ = value;
            next_ = nullptr;
        }
    };

    Node *head_;
    int size_;
};

float measureFinding(int elements_number, int type)
{
    List lst;
    for (int i = 0; i < elements_number; i++) {
        lst.pushFront(elements_number-i-1);
    }

    auto begin = std::chrono::high_resolution_clock::now();
    switch (type)
    {
    case START:
        lst.find(0);
        break;
    case MIDDLE:
        lst.find(elements_number/2);
        break;
    case END:
        lst.find(elements_number-1);
        break;
    default:
        return -1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    return duration.count()/1000.0;
}

float measureInsertion(int elements_number, int insertions_number, int type)
{
    std::vector<int> input_values(elements_number);
    List lst;
    for (int i = 0; i < input_values.size(); i++) {
        lst.pushFront(input_values[i]);
    }

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < insertions_number; i++) {
        switch (type)
        {
        case START:
            lst.pushFront(0);
            break;
        case MIDDLE:
            lst.insert(lst.size()/2, 0);
            break;
        case END:
            lst.pushBack(0);
            break;
        default:
            return -1;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    return duration.count()/1000.0;
}

float measureDeletion(int elements_number, int deletion_number, int type)
{
    deletion_number = std::min(elements_number, deletion_number);

    std::vector<int> input_values(elements_number);
    List lst;
    for (int i = 0; i < input_values.size(); i++) {
        lst.pushFront(input_values[i]);
    }

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < deletion_number; i++) {
        switch (type)
        {
        case START:
            lst.popFront();
            break;
        case MIDDLE:
            lst.pop(lst.size()/2);
            break;
        case END:
            lst.popBack();
            break;
        default:
            return -1;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    return duration.count()/1000.0;
}

int main()
{
    std::cout << "Finding in the beginning: " << measureFinding(100000, START) << " ms" << std::endl;
    std::cout << "Finding in the end: " << measureFinding(100000, END) << " ms" << std::endl;
    std::cout << "Finding in the middle: " << measureFinding(100000, MIDDLE) << " ms" << std::endl;
    
    std::cout << "Insertion in the beginning: " << measureInsertion(0, 100000, START) << " ms" << std::endl;
    std::cout << "Insertion in the end: " << measureInsertion(0, 100000, END) << " ms" << std::endl;
    std::cout << "Insertion in the middle: " << measureInsertion(0, 100000, MIDDLE) << " ms" << std::endl;

    std::cout << "Deletion in the beginning: " <<measureDeletion(100000, 100000, START) << " ms" << std::endl;
    std::cout << "Deletion in the end: " << measureDeletion(100000, 100000, END) << " ms" << std::endl;
    std::cout << "Deletion in the middle: " << measureDeletion(100000, 100000, MIDDLE) << " ms" << std::endl;
}