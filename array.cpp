#include <iostream>
#include <chrono>

#define START                0
#define MIDDLE               1
#define END                  2

class Array
{
public:
    Array()
    {
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    ~Array() = default;
    
    int size()
    {
        return size_;
    }

    int &operator[](int index)
    {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        return data_[index];
    }

    void insert(int index, int value)
    {
        if (size_ == capacity_) {
            expandCapacity();
        }

        index = std::max(0, std::min(size_, index));

        for (int i = size_; i > index; i--) {
            data_[i] = data_[i-1];
        }
        data_[index] = value;
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
            throw std::out_of_range("Index out of range");
        }
        
        for (int i = index; i < size_-1; i++) {
            data_[i] = data_[i+1];
        }

        size_--;

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
        for (int i = 0; i < size_; i++) {
            if (data_[i] == value) {
                return i;
            }
        }
        return -1;
    }


private:
    int* data_;
    int size_;
    int capacity_;

    void expandCapacity()
    {
        capacity_ = std::max(1, capacity_*2);
        int* new_data = new int[capacity_];
        for (int i = 0; i < size_; i++) {
            new_data[i] = data_[i];
        }
        data_ = new_data;
    }
};

float measureFinding(int elements_number, int type)
{

    Array arr;
    for (int i = 0; i < elements_number; i++) {
        arr.pushBack(i);
    }

    auto begin = std::chrono::high_resolution_clock::now();
    switch (type)
    {
    case START:
        arr.find(0);
        break;
    case MIDDLE:
        arr.find(elements_number/2);
        break;
    case END:
        arr.find(elements_number-1);
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
    Array arr;
    for (int i = 0; i < elements_number; i++) {
        arr.pushBack(i);
    }

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < insertions_number; i++) {
        switch (type)
        {
        case START:
            arr.pushFront(0);
            break;
        case MIDDLE:
            arr.insert(arr.size()/2, 0);
            break;
        case END:
            arr.pushBack(0);
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

    Array arr;
    for (int i = 0; i < elements_number; i++) {
        arr.pushBack(i);
    }

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < deletion_number; i++) {
        switch (type)
        {
        case START:
            arr.popFront();
            break;
        case MIDDLE:
            arr.pop(arr.size()/2);
            break;
        case END:
            arr.popBack();
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
    
    std::cout << "Insertion in the beginning: " << measureInsertion(1000, 100000, START) << " ms" << std::endl;
    std::cout << "Insertion in the end: " << measureInsertion(1000, 100000, END) << " ms" << std::endl;
    std::cout << "Insertion in the middle: " << measureInsertion(1000, 100000, MIDDLE) << " ms" << std::endl;

    std::cout << "Deletion in the beginning: " <<measureDeletion(100000, 100000, START) << " ms" << std::endl;
    std::cout << "Deletion in the end: " << measureDeletion(100000, 100000, END) << " ms" << std::endl;
    std::cout << "Deletion in the middle: " << measureDeletion(100000, 100000, MIDDLE) << " ms" << std::endl;
}