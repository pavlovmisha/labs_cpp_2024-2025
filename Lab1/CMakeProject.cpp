#include <iostream>
#include <string> 
#include <fstream> 
#include <vector>
#include <iterator>
#include <sstream>
#include <cstdlib>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    //открытие файла и подготовка необходимых переменных
    int length;
    std::string filename;

    std::cout << "Введите длину строки: ";
    std::cin >> length;

    std::cout << "Введите имя файла: ";
    std::cin >> filename;
    //filename = "input.txt";

    std::ifstream input;
    input.open(filename);

    if (!input) {
        std::cerr << "Errors with reading file " << filename << endl;
        return 1;
    }

    std::string readString;
    std::vector<std::string> current_string;
    std::string result;
    int spaces;

    //Начиныаем считывание строк
    while (std::getline(input, readString)) {

        //Делим текущую строку на слова
        std::istringstream iss(readString);
        std::vector<std::string> words((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

        for (const auto& word : words) {
            //Прорабатываем случай того, что слово слишком большое 
            if (word.size() > length) std::cerr << "Word " << word << " is too long to be placed on a line" << endl;

            //Cлово меньше строки, но в эту строку уже не влазит
            else if (result.size() + word.size() + 1 > length) {
                spaces = (length - (int)result.size()) / 2;
                for (int i = 0; i < spaces; i++) {
                    std::cout << " ";
                }
                std::cout << result << endl;
                result.clear();
                result += word;
            }
            //Если слово вмещается, добавляем проблем перед ним и само слово в текущую строку
            else {
                result += " ";
                result += word;
            }
        }


    }

    //Вывод последней незаполненной строки
    if (result.size() > 0) {
        spaces = (length - (int)result.size()) / 2;
        for (int i = 0; i < spaces; i++) {
            std::cout << " ";
        }
        std::cout << result << endl;
    }


    input.close();

    return 0;
}