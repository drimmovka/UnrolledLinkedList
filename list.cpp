#include <list>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

#define START                0
#define MIDDLE               1
#define END                  2

float measureFinding(int elements_number, int type)
{
    std::list<int> lst;
    for (int i = 0; i < elements_number; i++) {
        lst.push_back(i);
    }

    auto begin = std::chrono::high_resolution_clock::now();
    switch (type)
    {
    case START:
        find(lst.begin(), lst.end(), 0);
        break;
    case MIDDLE:
        find(lst.begin(), lst.end(), elements_number/2);
        break;
    case END:
        find(lst.begin(), lst.end(), elements_number-1);
        break;
    default:
        return -1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    return duration.count()/1000.0;
}

int main()
{
    std::cout << "Finding in the beginning: " << measureFinding(1000000, START) << " ms" << std::endl;
    std::cout << "Finding in the end: " << measureFinding(1000000, END) << " ms" << std::endl;
    std::cout << "Finding in the middle: " << measureFinding(1000000, MIDDLE) << " ms" << std::endl;
    
    // std::cout << "Insertion in the beginning: " << measureInsertion(1000, 1000000, START) << " ms" << std::endl;
    // std::cout << "Insertion in the end: " << measureInsertion(1000, 1000000, END) << " ms" << std::endl;
    // std::cout << "Insertion in the middle: " << measureInsertion(1000, 1000000, MIDDLE) << " ms" << std::endl;

    // std::cout << "Deletion in the beginning: " <<measureDeletion(100000, 1000000, START) << " ms" << std::endl;
    // std::cout << "Deletion in the end: " << measureDeletion(100000, 1000000, END) << " ms" << std::endl;
    // std::cout << "Deletion in the middle: " << measureDeletion(100000, 1000000, MIDDLE) << " ms" << std::endl;
}