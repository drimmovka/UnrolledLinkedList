#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "ULL.h"

#define NUMBER_DISTRIBUTION  100
#define START                0
#define MIDDLE               1
#define END                  2

bool compare(UnrolledLinkedList& ull, std::vector<int>& vec)
{
    if (ull.size() != vec.size()) {
        return false;
    }
    for (int i = 0; i < ull.size(); i++) {
        if (ull[i] != vec[i]) {
            return false;
        }
    }
    return true;
}

void randomFilling(std::vector<int>& vec)
{
    for (int i = 0; i < vec.size(); i++) {
        vec[i] = rand() % NUMBER_DISTRIBUTION;
    }
}

void checkFindPopMethods(int elements_number, int detections_number)
{
    std::vector<int> input_values(elements_number);
    randomFilling(input_values);
    UnrolledLinkedList ull(input_values);
    
    std::vector<int> detect_values(detections_number);
    randomFilling(detect_values);

    for (int i = 0; i < (int)detect_values.size(); i++) {
        int to_check_index = ull.find(detect_values[i]);
        int real_index = find(input_values.begin(), input_values.end(), detect_values[i]) - input_values.begin();
        
        if (real_index == input_values.size()) {
            real_index = -1;
        }
        
        assert(to_check_index == real_index);

        if (to_check_index != -1) {
            ull.pop(to_check_index);
        }

        if (real_index != -1) {
            input_values.erase(input_values.begin() + real_index);
        }

        assert(compare(ull, input_values));

    }
    std::cout << "Find and pop methods tests passed" << std::endl;
}

void checkInsertMethod(int elements_number, int insertions_number)
{
    std::vector<int> input_values(elements_number);
    randomFilling(input_values);
    UnrolledLinkedList ull(input_values);

    for (int i = 0; i < insertions_number; i++) {
        int index = rand() % (input_values.size()+1);
        int value = rand();

        ull.insert(index, value);
        input_values.insert(input_values.begin() + index, value);

        assert(compare(ull, input_values));
    }

    std::cout << "Insert method tests passed" << std::endl;
}

float measureFinding(int elements_number, int type)
{

    std::vector<int> input_values(elements_number);
    for (int i = 0; i < input_values.size(); i++) {
        input_values[i] = i;
    }
    UnrolledLinkedList ull(input_values);

    auto begin = std::chrono::high_resolution_clock::now();
    switch (type)
    {
    case START:
        ull.find(0);
        break;
    case MIDDLE:
        ull.find(elements_number/2);
        break;
    case END:
        ull.find(elements_number-1);
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
    UnrolledLinkedList ull(input_values);

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < insertions_number; i++) {
        switch (type)
        {
        case START:
            ull.pushFront(0);
            break;
        case MIDDLE:
            ull.insert(ull.size()/2, 0);
            break;
        case END:
            ull.pushBack(0);
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
    UnrolledLinkedList ull(input_values);

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < deletion_number; i++) {
        switch (type)
        {
        case START:
            ull.popFront();
            break;
        case MIDDLE:
            ull.pop(ull.size()/2);
            break;
        case END:
            ull.popBack();
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
    srand(time(0));

    checkFindPopMethods(1000, 1000);
    checkInsertMethod(1000, 1000);

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