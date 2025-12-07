#include <iostream>
#include <vector>
#include <inttypes.h>
#include <string> 
#include <array>

#define LINE_COUNT 4

int64_t readNum(const std::array<std::string, LINE_COUNT>& lines, size_t start_index) {
    int64_t num = -1;
    for(size_t line_index = 0; line_index < LINE_COUNT; line_index++) {
        char c = lines[line_index][start_index];
        if(c == ' ') {
            continue;
        }
        if(num == -1) {
            num = 0;
        }
        num *= 10;
        num += (c - '0');
    }
    return num;
}

int main () {
    std::array<std::string, LINE_COUNT> lines;
    for(size_t i = 0; i < LINE_COUNT; i++) {
        std::getline(std::cin, lines[i]);
    }
    std::string operators;
    std::getline(std::cin, operators);

    uint64_t sum = 0; 
    for(size_t string_index = 0; string_index < operators.size(); string_index++) {
        if(operators[string_index] == ' ') {
            continue;
        }
        char op = operators[string_index];

        int64_t num = -1;
        int64_t result = 0;

        do {
            num = readNum(lines, string_index);
            if (num == -1) {
                break;
            }
            
            // std::cout << "Read num: " << num << " with op: " << op << std::endl;
            if(op == '+') {
                result += num;
            } else if(op == '*') {
                if (result == 0) {
                    result = 1;
                }
                result *= (num == -1 ? 1 : num);
            }
            string_index++;
        } while (string_index < operators.size() && num != -1);

        // std::cout << result << std::endl;

        sum += result;
    }

    std::cout << sum << std::endl;


}