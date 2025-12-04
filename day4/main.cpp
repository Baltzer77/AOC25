#include <iostream>
#include <vector>
#include <string>

int removeRolls(std::vector<std::string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    int count = 0;

    constexpr int dirr[] = {1, 0, -1, 0, 1, -1, 1, -1};
    constexpr int dirc[] = {0, 1, 0, -1, 1, -1, -1, 1};

    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(grid[r][c] != '@') continue;

            int adjacent_count = 0;
            for(int i = 0; i < 8; i++) {
                int nr = dirr[i] + r;
                int nc = dirc[i] + c;

                if(nr >= 0 && nc >=0 && nr < rows && nc < cols) {
                    if(grid[nr][nc] == '@') {
                        adjacent_count++;
                    }
                }
            }

            if( adjacent_count < 4) {
                count++;
                grid[r][c] = '.';
            }
        }
    }

    // for(int r = 0; r < rows; r++) {
    //     for(int c = 0; c < cols; c++) {
    //         if(grid[r][c] == 'x') {
    //             grid[r][c] = '.';
    //         }
    //     }
    // }

    return count;
}

int main() {
    
    std::string line;
    std::vector<std::string> grid;
    while (std::getline(std::cin, line)) {
        if(line.empty()) {
            break;
        }
        grid.push_back(line);  
    }
    int count = 0, curRolls = 0;
    do {
        curRolls = removeRolls(grid);
        count += curRolls; 
    } while(curRolls != 0);


    std::cout << count <<std::endl;

    return 0;
}