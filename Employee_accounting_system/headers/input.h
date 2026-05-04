#ifndef MYPROJECT_INPUT_H
#define MYPROJECT_INPUT_H
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using std::cin, std::cout, std::endl, std::string;
void Input(int& res);
void Input(size_t& res);
void Input_str(string& res);
void Input_strings(string& str);
void question(int& answer, std::vector<int> allow);
void clear();
#endif