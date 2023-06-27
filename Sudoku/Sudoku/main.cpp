#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

const int SIZE = 9;
const int EMPTY = 0;
const char EMPTY_CHAR = '$';

// 检查数字num是否可以填入指定位置
bool isValid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    // 检查行是否有重复
    for (int i = 0; i < SIZE; ++i) {
        if (board[row][i] == num) {
            return false;
        }
    }

    // 检查列是否有重复
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][col] == num) {
            return false;
        }
    }

    // 检查小方格是否有重复
    int boxRow = row - row % 3;
    int boxCol = col - col % 3;
    for (int i = boxRow; i < boxRow + 3; ++i) {
        for (int j = boxCol; j < boxCol + 3; ++j) {
            if (board[i][j] == num) {
                return false;
            }
        }
    }

    return true;
}

// 求解数独问题
bool solveSudoku(std::vector<std::vector<int>>& board) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (board[row][col] == EMPTY) {
                for (int num = 1; num <= SIZE; ++num) {
                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) {
                            return true;
                        }
                        board[row][col] = EMPTY;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// 生成数独终局
void generateSudoku(std::string filename, int count) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < count; ++i) {
        std::vector<std::vector<int>> board(SIZE, std::vector<int>(SIZE, EMPTY));

        // 随机填充第一行
        std::vector<int> nums(SIZE);
        for (int j = 0; j < SIZE; ++j) {
            nums[j] = j + 1;
        }
        std::shuffle(nums.begin(), nums.end(), gen);
        for (int j = 0; j < SIZE; ++j) {
            board[0][j] = nums[j];
        }

        // 求解数独问题
        solveSudoku(board);

        // 打印数独终局到文件
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                if (board[row][col] == EMPTY) {
                    file << EMPTY_CHAR << " ";
                }
                else {
                    file << board[row][col] << " ";
                }
            }
            file << std::endl;
        }
        file << std::endl;
    }

    file.close();
}

// 生成数独游戏
void generateSudokuGames(std::string filename, int gameCount) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < gameCount; ++i) {
        std::vector<std::vector<int>> solution(SIZE, std::vector<int>(SIZE, EMPTY));
        std::vector<std::vector<int>> game(SIZE, std::vector<int>(SIZE, EMPTY));

        // 生成数独终局
        generateSudoku("temp_solution.txt", 1);

        // 读取数独终局
        std::ifstream solutionFile("temp_solution.txt");
        if (!solutionFile.is_open()) {
            std::cout << "Error opening solution file." << std::endl;
            file.close();
            return;
        }

        std::string line;
        int row = 0;
        while (std::getline(solutionFile, line)) {
            std::stringstream ss(line);
            int num;
            int col = 0;
            while (ss >> num) {
                solution[row][col] = num;
                game[row][col] = num;
                col++;
                
            }
            row++;
        }

        solutionFile.close();

        // 生成数独游戏
        int emptyCount = std::uniform_int_distribution<>(30, 50)(gen);  // 随机挖去的空格数量
        //int emptyCount = 5;
        while (emptyCount > 0) {
            int row = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            int col = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            cout << row << " " << col << " " << game[row][col] << endl;
            if (game[row][col] != EMPTY) {
                game[row][col] = EMPTY;
                emptyCount--;
            }
            else {
                continue; // 当前格子已经为空，继续选择新的格子
            }
        }

        // 打印数独游戏到文件
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                if (game[row][col] == EMPTY) {
                    file << EMPTY_CHAR << " ";
                }
                else {
                    file << game[row][col] << " ";
                }
            }
            file << std::endl;
        }
        file << std::endl;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: sudoku.exe -c <count> | -n <gameCount>" << std::endl;
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "-c") {
        int count = std::stoi(argv[2]);
        generateSudoku("sudoku.txt", count);
    }
    else if (mode == "-n") {
        int gameCount = std::stoi(argv[2]);
        generateSudokuGames("sudoku_games.txt", gameCount);
    }
    else {
        std::cout << "Invalid mode." << std::endl;
    }

    return 0;
}
