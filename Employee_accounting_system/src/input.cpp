#include "../headers/input.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

using std::cin, std::cout, std::endl, std::string;
void clear() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool get(int& res) {
    cin >> res;
    if (cin.good()){cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');return true;}
    if (cin.eof())throw std::logic_error("Конец ввода");
    if (cin.fail()) {
        clear();
        return false;
    }
    if (cin.bad())throw std::runtime_error("Ошибка работы потока потока");
    return false;
}

void Input(int& res){
    while (!get(res)) { cout << "Это не число!" << std::endl; }
}
void Input(size_t& res) {
    int temp = -1;
    while (!get(temp) || temp < 0) { cout << "Это не положительное число!" << std::endl; }
    res = temp;
}

void Input_str(string& res) {
    if (cin.peek() == '\n') {
        cin.ignore(1, '\n'); // пропустить только один \n
    }

    getline(cin, res);
    if (cin.good()) return;
    if (cin.eof()) throw std::logic_error("Конец ввода");
    clear();
    throw std::runtime_error("проблемы ввода");
}

void Input_strings(string& str) {
    clear();
    str = "";
    string line;
    while (true) {
        std::getline(std::cin, line);
        if (line.empty()) break;
        str += line + '\n';
    }
}

void question(int& answer, std::vector<int> allow) {
    answer = -1;
    Input(answer);
    while (find(allow.begin(), allow.end(), answer) == allow.end()) {
        cout << "Неверный ввод" << endl;
        Input(answer);
    }
}


