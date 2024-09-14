#include "ULL.h"
#include <iostream>
#include <string>
#include <vector>


void inputValues(std::vector<int>& values)
{
    std::string input;
    std::getline(std::cin, input);

    int j = 0;
    for (int i = 0; i < (int) input.size(); i++) {
        if (input[i] == ' ') {
            values.push_back( stoi(input.substr(j, i-j)) );
            j = i + 1;
        }
    }
    values.push_back( stoi(input.substr(j, input.size()-j)) );
}

int main()
{
    std::vector<int> values;
    inputValues(values);

    UnrolledLinkedList ull(values);
    ull.print();

    return 0;
}