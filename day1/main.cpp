#include <iostream>
#include <string>

int main() {

    int pos = 50;
    int zeroAmount = 0;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        int dir = (line[0] == 'R') ? 1 : -1;
        int length = std::stoi(line.substr(1));
        int fullCycles = length / 100;

        // handle edge case where starting at zero and moving left
        if(pos == 0 && dir == -1) {
            zeroAmount--;
        }

        // add full rotations 
        zeroAmount += fullCycles;
        length -= fullCycles * 100;

        // move the remaining length
        pos += dir * length;
        if(pos >= 100 || pos <= 0) {    
            zeroAmount++;
            pos = (pos + 100) % 100;
        }
    }
    std::cout << zeroAmount << std::endl;

    return 0;
}