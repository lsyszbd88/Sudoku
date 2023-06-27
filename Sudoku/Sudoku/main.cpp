#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <sstream>
#include <numeric>


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

        // 随机打乱第一行
        std::vector<int> firstRow(SIZE);
        for (int j = 0; j < SIZE; ++j) {
            firstRow[j] = j + 1;
        }
        std::shuffle(firstRow.begin(), firstRow.end(), gen);

        // 根据偏移值生成剩余行
        int offset[9] = { 0, 6, 6, 5, 6, 6, 5, 6, 6 };
        for (int row = 0; row < SIZE; ++row) {
            std::rotate(firstRow.begin(), firstRow.begin() + offset[row], firstRow.end());
            for (int col = 0; col < SIZE; ++col) {
                board[row][col] = firstRow[col];
            }
        }

        // 随机进行行变换
        std::shuffle(board.begin(), board.begin() + 3, gen);
        std::shuffle(board.begin() + 3, board.begin() + 6, gen);
        std::shuffle(board.begin() + 6, board.end(), gen);

        // 打印数独终局到文件
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                file << board[row][col] << " ";
            }
            file << std::endl;
        }
        file << std::endl;
    }

    file.close();
}

// 生成数独游戏
void generateSudokuGames(std::string filename, int gameCount, int minHoles, int maxHoles, int difficulty) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    int emptyCount;
    if (difficulty == 0) {
        if (minHoles == 0) {
            minHoles = 20;
            maxHoles = 55;
            emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);
        }
        else
            emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);
    }
    else {
        if (minHoles == 0) {
            switch (difficulty)
            {
            case 1:minHoles = 20; maxHoles = 34; break;
            case 2:minHoles = 35; maxHoles = 49; break;
            case 3:minHoles = 50; maxHoles = 55; break;
            }
            emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);
        }
        else {
            cout << minHoles << endl;
            std::cout << "Error: [-m] and [-r] can't appear together." << std::endl;
            return;
        }
    }
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
        
        int emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);  // 随机挖去的空格数量
        while (emptyCount > 0) {
            int row = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            int col = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            if (game[row][col] != EMPTY) {
                game[row][col] = EMPTY;
                emptyCount--;
            }
            else {
                continue; // 当前格子已经为空，继续选择新的格子
            }
        }

        std::string gameFilename = "games" + std::to_string(i) + ".txt";
        std::ofstream file(gameFilename);
        if (!file.is_open()) {
            std::cout << "Error opening file: " << gameFilename << std::endl;
            return;
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

        file.close();
    }

    file.close();
}

int main(int argc, char* argv[]) {
    int sudokuCount = 0;
    int gameCount = 0;
    int minHoles = 0;
    int maxHoles = 0;
    int difficulty = 0;

    bool hasN = false;
    bool hasR = false;
    bool hasM = false;

    for (int i = 1; i < argc; i += 2) {
        std::string arg(argv[i]);
        if (arg == "-c") {
            sudokuCount = std::stoi(argv[i + 1]);
        }
        else if (arg == "-n") {
            gameCount = std::stoi(argv[i + 1]);
            hasN = true;
        }
        else if (arg == "-r") {
            std::string range = argv[i + 1];
            std::stringstream ss(range);
            std::string minHolesStr, maxHolesStr;
            std::getline(ss, minHolesStr, '~');
            std::getline(ss, maxHolesStr);
            minHoles = std::stoi(minHolesStr);
            maxHoles = std::stoi(maxHolesStr);
            hasR = true;
        }
        else if (arg == "-m") {
            difficulty = std::stoi(argv[i + 1]);
            hasM = true;
        }
    }

    if (gameCount > 0) {
        if (!hasN || (hasR && hasM)) {
            std::cout << "Error: Invalid arguments." << std::endl;
            return 0;
        }

        if (hasM) {
            if (difficulty < 1 || difficulty > 3) {
                std::cout << "Error: Invalid difficulty level." << std::endl;
                return 0;
            }
        }
        if (hasR) {
            if (minHoles > maxHoles) {
                std::cout << "Error: Invalid range for holes." << std::endl;
                return 0;
            }
            if (minHoles < 20 || maxHoles>55) {
                std::cout << "Error: Invalid range for holes." << std::endl;
                return 0;
            }
        }

        generateSudokuGames("games.txt", gameCount, minHoles, maxHoles, difficulty);
    }
    else if (sudokuCount > 0) {
        if (hasR || hasM) {
            std::cout << "Error: Invalid arguments." << std::endl;
            return 0;
        }

        generateSudoku("sudokus.txt", sudokuCount);
    }
    else {
        std::cout << "Invalid arguments." << std::endl;
    }

    return 0;
}
