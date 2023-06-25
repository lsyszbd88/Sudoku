#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <sstream>

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

// 读取数独问题并求解
void solveSudokuFromFile(std::string inputFilename, std::string outputFilename) {
    std::ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        std::cout << "Error opening input file: " << inputFilename << std::endl;
        return;
    }

    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        std::cout << "Error creating output file: " << outputFilename << std::endl;
        return;
    }

    std::string line;
    std::vector<std::vector<int>> board(SIZE, std::vector<int>(SIZE, EMPTY));
    std::vector<std::vector<std::vector<int>>> solutions;

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        int num;
        int col = 0;
        while (ss >> num) {
            if (num == EMPTY_CHAR) {
                board[0][col] = EMPTY;
            }
            else {
                board[0][col] = num;
            }
            col++;
        }

        // 求解数独问题
        solveSudoku(board);

        // 保存解决方案
        solutions.push_back(board);

        // 清空数独板
        board = std::vector<std::vector<int>>(SIZE, std::vector<int>(SIZE, EMPTY));
    }

    // 输出所有解决方案到文件
    for (const auto& solution : solutions) {
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                if (solution[row][col] == EMPTY) {
                    outputFile << EMPTY_CHAR << " ";
                }
                else {
                    outputFile << solution[row][col] << " ";
                }
            }
            outputFile << std::endl;
        }
        outputFile << std::endl;
    }

    inputFile.close();
    outputFile.close();
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: sudoku.exe -c <count> | -s <inputFile> <outputFile>" << std::endl;
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "-c") {
        int count = std::stoi(argv[2]);
        if (count < 1 || count > 1000000) {
            std::cout << "Invalid count. Count must be between 1 and 1000000." << std::endl;
            return 0;
        }

        std::string filename = "sudoku_puzzles.txt";
        generateSudoku(filename, count);
        std::cout << "Sudoku puzzles generated and saved to: " << filename << std::endl;
    }
    else if (mode == "-s") {
        if (argc < 4) {
            std::cout << "Usage: sudoku.exe -s <inputFile> <outputFile>" << std::endl;
            return 0;
        }

        std::string inputFilename = argv[2];
        std::string outputFilename = argv[3];
        solveSudokuFromFile(inputFilename, outputFilename);
        std::cout << "Sudoku problems solved and solution saved to: " << outputFilename << std::endl;
    }
    else {
        std::cout << "Invalid mode. Available modes: -c, -s" << std::endl;
    }

    return 0;
}
