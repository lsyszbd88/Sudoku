#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <sstream>
#include <numeric>
#include<iomanip>

using namespace std;

const int SIZE = 9;
const int EMPTY = 0;
const char EMPTY_CHAR = '$';

/*
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
*/
FILE* answer;
bool isPlace(int count, const std::vector<std::vector<int>>& board)
{
    int row = count / 9;
    int col = count % 9;
    int j;
    for (j = 0; j < 9; j++)     //同一行
    {

        if (board[row][j] == board[row][col] && j != col)
            return false;
    }

    for (j = 0; j < 9; j++)     //同一列
    {
        if (board[j][col] == board[row][col] && j != row)
            return false;
    }

    int baseRow = row / 3 * 3;
    int baseCol = col / 3 * 3;
    for (j = baseRow; j < baseRow + 3; j++)   //同一宫
    {
        for (int k = baseCol; k < baseCol + 3; k++)
        {
            if (board[j][k] == board[row][col] && (j != row || k != col))
                return false;
        }
    }
    return true;
}

void backtrace(int count, long& resultcount, std::vector<std::vector<int>>& board)
{
    if (count == 81)
    {
        resultcount++;
        for (int i = 0; i < 9; ++i)
        {

            fprintf(answer, "%d %d %d %d %d %d %d %d %d\n", board[i][0], board[i][1], board[i][2], board[i][3], board[i][4], board[i][5], board[i][6], board[i][7], board[i][8]);
        }
        fputs("\n", answer);
        return;
    }
    int row = count / 9;
    int col = count % 9;
    if (board[row][col] == 0)
    {
        for (int i = 1; i <= 9; i++)
        {
            board[row][col] = i + 0;
            if (isPlace(count, board))
            {
                backtrace(count + 1, resultcount, board);//进入下一层
            }

        }
        board[row][col] = 0;//回溯
    }
    else
        backtrace(count + 1, resultcount, board);
}
// 求解数独问题
int solveSudoku(std::vector<std::vector<int>>& board) {

    // solve sudoku
    long resultcount = 0;
    backtrace(0, resultcount, board);
    //resultfile.close();
    return resultcount;
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

bool hasUniqueSolution(const std::vector<std::vector<int>>& board) {
    // 创建一个副本
    std::vector<std::vector<int>> gameCopy = board;

    // 使用回溯算法求解数独
    if (solveSudoku(gameCopy) == 1)
        return true;
    return false;
}

void generateUniqueSudoku(std::string filename, int gameCount) {
    std::ofstream file1(filename);
    if (!file1.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    // 根据难度和空格数量限制计算空格数量

    for (int i = 0; i < gameCount; ++i) {
        int emptyCount = std::uniform_int_distribution<>(20, 55)(gen);
        std::vector<std::vector<int>> solution(SIZE, std::vector<int>(SIZE, EMPTY));
        std::vector<std::vector<int>> game(SIZE, std::vector<int>(SIZE, EMPTY));

        // 生成数独终局
        generateSudoku("temp_solution.txt", 1);

        // 读取数独终局
        std::ifstream solutionFile("temp_solution.txt");
        if (!solutionFile.is_open()) {
            std::cout << "Error opening solution file." << std::endl;
            file1.close();
            return;
        }

        // 解析数独终局
        std::string line;
        int row1 = 0;
        while (std::getline(solutionFile, line)) {
            std::stringstream ss(line);
            int num;
            int col = 0;
            while (ss >> num) {
                solution[row1][col] = num;
                game[row1][col] = num;
                col++;
            }
            row1++;
        }

        solutionFile.close();

        // 随机挖去空格
        while (emptyCount > 0) {
            int row = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            int col = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            if (game[row][col] != EMPTY) {
                // 暂时移除数字
                int temp = game[row][col];
                game[row][col] = EMPTY;

                // 检查是否有唯一解
                if (!hasUniqueSolution(game)) {
                    // 不唯一解，恢复数字
                    game[row][col] = temp;
                    continue;
                }
                emptyCount--;
            }
            else {
                continue; // 当前格子已经为空，继续选择新的格子
            }
        }

        std::string gameFilename = "games_u" + std::to_string(i) + ".txt";
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

    file1.close();
}


// 生成数独游戏
void generateSudokuGames(std::string filename, int gameCount, int minHoles, int maxHoles, int difficulty) {
    std::ofstream file1(filename);
    if (!file1.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    //int emptyCount;
    if (difficulty == 0) {
        if (minHoles == 0) {
            minHoles = 20;
            maxHoles = 55;
            //emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);
        }
        //else
            //emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);
    }
    else {
        if (minHoles == 0) {
            switch (difficulty)
            {
            case 1:minHoles = 20; maxHoles = 34; break;
            case 2:minHoles = 35; maxHoles = 49; break;
            case 3:minHoles = 50; maxHoles = 55; break;
            }
            //emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);
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
            file1.close();
            return;
        }

        std::string line;
        int row1 = 0;
        while (std::getline(solutionFile, line)) {
            std::stringstream ss(line);
            int num;
            int col = 0;
            while (ss >> num) {
                solution[row1][col] = num;
                game[row1][col] = num;
                col++;

            }
            row1++;
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

    file1.close();
}
bool checkInput(const std::vector<std::vector<int>>& board)
{
    for (int i = 0; i < 9; i++)     //同一行
    {
        for (int j = 0; j < 9; j++)
        {
            for (int t = j + 1; t < 9; t++)
            {
                if (board[i][t] != 0 && board[i][t] == board[i][j])
                {
                    return false;
                }
            }
        }
    }
    for (int i = 0; i < 9; i++)     //同一行
    {
        for (int j = 0; j < 9; j++)
        {
            for (int t = j + 1; t < 9; t++)
            {
                if (board[t][i] != 0 && board[t][i] == board[j][i])
                {
                    return false;
                }
            }
        }
    }
    for (int i = 0; i < 9; i += 3)     //同一行
    {
        for (int j = 0; j < 9; j += 3)
        {
            bool check[10];
            for (int t = 0; t < 10; t++)
            {
                check[t] = false;
            }
            for (int m = i; m < i + 3; m++)
            {
                for (int n = j; n < j + 3; n++)
                {
                    if (check[board[m][n]] && board[m][n] != 0)
                    {
                        return false;
                    }
                    check[board[m][n]] = true;
                }
            }
        }
    }
    return true;
}
void ReadAndSolve(const char* solvefilename) {
    errno_t err;
    FILE* tryopen;
    err = fopen_s(&tryopen, solvefilename, "r");
    if (err != 0)
    {
        printf("The file you want to open doesn't exist\n");
        return;
    }
    ifstream problemfile(solvefilename);
    std::vector<std::vector<int>> game(SIZE, std::vector<int>(SIZE, EMPTY));
    //std::string line;
    if (problemfile)
    {
        int total = 0;
        string str;
        //bool exc = false;
        string s;
        while (total < 1000000 && getline(problemfile, str))
        {
            if (str.empty()) {
                if (!s.empty()) {
                    istringstream iss(s);
                    string token;
                    int row = 0;
                    int col = 0;
                    while (iss >> token) {
                        if (token == "$") {
                            game[row][col] = 0;
                        }
                        else {
                            game[row][col] = std::stoi(token);
                        }
                        col++;
                        if (col == SIZE) {
                            col = 0;
                            row++;
                        }
                        if (row == SIZE) {
                            total++;
                            // solve sudoku
                            bool inputok = checkInput(game);
                            if (inputok)
                            {
                                cout << "input ok";
                                long num = solveSudoku(game);
                                cout << num << "results";
                            }
                            else
                            {
                                cout << "input 重复number";

                            }
                        }
                    }
                    s.clear();
                }
            }
            else {
                s += str + " ";
            }
        }
    }
    else
        cout << "Can't find such file:" << string(solvefilename) << endl;

    return;
}
int main(int argc, char* argv[]) {
    cout << "Welcome to Sudoku Program! You can input the following cmmmand:" << endl;
    cout << setw(8) << "argc" << setw(15) << "argv" << setw(25) << "function" << endl;
    cout << setw(8) << "-c" << setw(15) << "1-1000000" << setw(25) << "生成数独终局" << endl;
    cout << setw(8) << "-s" << setw(15) << "解题路径" << setw(25) << "求解数独游戏" << endl;
    cout << setw(8) << "-n" << setw(15) << "1-10000" << setw(25) << "生成数独游戏的数量" << endl;
    cout << setw(8) << "-m" << setw(15) << "1-3" << setw(25) << "生成数独游戏的难度" << endl;
    cout << setw(8) << "-r" << setw(15) << "20-55" << setw(25) << "指定数独有戏挖空数" << endl;
    cout << setw(8) << "-u" << setw(15) << "" << setw(25) << "生成数独游戏的解唯一" << endl;
    int sudokuCount = 0;
    int gameCount = 0;
    int minHoles = 0;
    int maxHoles = 0;
    int difficulty = 0;
    char* solvefilename = NULL;

    bool hasN = false;
    bool hasR = false;
    bool hasM = false;
    bool hasU = false;
    bool hasS = false;

    for (int i = 1; i < argc; i += 2) {
        std::string arg(argv[i]);
        if (arg == "-c") {
            sudokuCount = std::stoi(argv[i + 1]);
        }
        else if (arg == "-n") {
            gameCount = std::stoi(argv[i + 1]);
            hasN = true;
        }
        else if (arg == "-s")
        {
            solvefilename = argv[i + 1];
            errno_t err;
            FILE* tryopen;
            err = fopen_s(&tryopen, solvefilename, "r");
            if (err != 0)
            {
                printf("The file you want to open doesn't exist\n");
                return 0;
            }
            hasS = true;

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
        else if (arg == "-u") {
            hasU = true;
        }
    }
    if (hasS) {
        if (hasN || hasR || hasM || hasU) {
            std::cout << "Error: Invalid arguments." << std::endl;
            return 0;
        }
        errno_t err1;
        err1 = fopen_s(&answer, "sudoku.txt", "w+");
        if (err1 != 0)
        {
            printf("The file you want to open doesn't exist\n");
            return 0;
        }
        ReadAndSolve(solvefilename);
    }
    else if (gameCount > 0) {
        cout << hasN << hasR << hasM << hasU;
        if (!hasN || (hasR && hasM) || (hasR && hasU) || (hasU && hasM)) {
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
        if (hasU)
            generateUniqueSudoku("games.txt", gameCount);
        else
            generateSudokuGames("games.txt", gameCount, minHoles, maxHoles, difficulty);
    }
    else if (sudokuCount > 0) {
        cout << hasN << hasR << hasM << hasU;
        if (hasN || hasR || hasM || hasU) {
            std::cout << "Error: Invalid arguments." << std::endl;
            return 0;
        }

        generateSudoku("sudokus.txt", sudokuCount);
    }
    else {
        std::cout << "Error：Invalid arguments." << std::endl;
    }

    return 0;
}