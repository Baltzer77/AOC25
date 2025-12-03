#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <inttypes.h>

std::string addToSum(const std::string& a, const std::string& b) {
    int i = a.size() - 1;
    int j = b.size() - 1;
    int carry = 0;
    std::vector<char> result;


    while(i >= 0 || j >= 0 || carry) {
        int a_dig = (i >= 0) ? a[i] - '0' : 0;
        int b_dig = (j >= 0) ? b[j] - '0' : 0;
        int current = a_dig + b_dig + carry;

        carry = current/10;
        current = current % 10;

        result.push_back('0' + current);
        i--;
        j--;
    }

    std::reverse(result.begin(), result.end());
    return std::string(result.begin(), result.end());
}


int findDigit(const std::string& line, int start_index, int digit_num) {
    int largest_index = start_index;
    for(size_t i = start_index + 1; i < line.size() - digit_num; ++i) {
        int digit = line[i];
        if(digit > line[largest_index]) {
            largest_index = i;
        }
    }

    return largest_index;
}

int main() {
    constexpr size_t digitAmount = 12;
    std::string sum = "0";
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }    

        int start_index = 0;
        std::string joltage = "";
        
        for(size_t i = 1; i <= digitAmount; ++i) {
            int best_index = findDigit(line, start_index, digitAmount - i);
            joltage += line[best_index];
            start_index = best_index + 1;
        }
        
        sum = addToSum(sum, joltage);
    }

    std::cout << sum << std::endl;
    return 0;
}