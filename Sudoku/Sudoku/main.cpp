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

// �������num�Ƿ��������ָ��λ��
bool isValid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    // ������Ƿ����ظ�
    for (int i = 0; i < SIZE; ++i) {
        if (board[row][i] == num) {
            return false;
        }
    }

    // ������Ƿ����ظ�
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][col] == num) {
            return false;
        }
    }

    // ���С�����Ƿ����ظ�
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

// �����������
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

FILE* answer;
char grid[9][9] = { 0 };
bool isPlace(int count)
{
    int row = count / 9;
    int col = count % 9;
    int j;
    for (j = 0; j < 9; j++)     //ͬһ��
    {
        if (grid[row][j] == grid[row][col] && j != col)
            return false;
    }

    for (j = 0; j < 9; j++)     //ͬһ��
    {
        if (grid[j][col] == grid[row][col] && j != row)
            return false;
    }

    int baseRow = row / 3 * 3;
    int baseCol = col / 3 * 3;
    for (j = baseRow; j < baseRow + 3; j++)   //ͬһ��
    {
        for (int k = baseCol; k < baseCol + 3; k++)
        {
            if (grid[j][k] == grid[row][col] && (j != row || k != col))
                return false;
        }
    }
    return true;
}

void backtrace(int count)
{
    if (count == 81)
    {
        for (int i = 0; i < 9; ++i)
        {

            fprintf(answer, "%c %c %c %c %c %c %c %c %c\n", grid[i][0], grid[i][1], grid[i][2], grid[i][3], grid[i][4], grid[i][5], grid[i][6], grid[i][7], grid[i][8]);
        }
        fputs("\n", answer);
        return;
    }
    int row = count / 9;
    int col = count % 9;
    if (grid[row][col] == '0')
    {
        for (int i = 1; i <= 9; i++)
        {
            grid[row][col] = i + '0';
            if (isPlace(count))
            {
                backtrace(count + 1);//������һ��
            }

        }
        grid[row][col] = '0';//����
    }
    else
        backtrace(count + 1);
}
// �����������
void solveSudoku(string filename) {
    ifstream problemfile(filename);
    errno_t err;
    err = fopen_s(&answer, "sudoku.txt", "w+");
    if (problemfile)
    {
        int total = 0;
        string temp[9];
        string str;
        int line = 0;
        bool exc = false;
        while (total < 1000000 && getline(problemfile, str))
        {
            temp[line] = str;  //��problemfile�ж�ȡһ�е�temp��
            line++;
            if (line == 9)   //ÿ����9�н���һ�δ���
            {
                for (int i = 0; i < 9; i++)
                    for (int j = 0; j < 9; j++)
                    {
                        grid[i][j] = temp[i][2 * j];
                        if (temp[i][2 * j] == EMPTY_CHAR)
                        {
                            grid[i][j] = '0';
                        }
                        //�����ո�һ����������װ��grid��
                        if (grid[i][j] < '0' || grid[i][j] > '9')
                        {
                            exc = true;
                            break;
                        }
                    }
                getline(problemfile, str);//����һ����Ŀ��Ŀ���
                line = 0;
                if (exc)
                {
                    exc = false;
                    cout << "Error!" << endl;
                    continue;
                }
                total++;
                // solve sudoku
                long count = 0;
                backtrace(0);
            }
        }
        //resultfile.close();
    }
    else
        cout << "Can't find such file:" << string(filename) << endl;
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

        // ������ҵ�һ��
        std::vector<int> firstRow(SIZE);
        for (int j = 0; j < SIZE; ++j) {
            firstRow[j] = j + 1;
        }
        std::shuffle(firstRow.begin(), firstRow.end(), gen);

        // ����ƫ��ֵ����ʣ����
        int offset[9] = { 0, 6, 6, 5, 6, 6, 5, 6, 6 };
        for (int row = 0; row < SIZE; ++row) {
            std::rotate(firstRow.begin(), firstRow.begin() + offset[row], firstRow.end());
            for (int col = 0; col < SIZE; ++col) {
                board[row][col] = firstRow[col];
            }
        }

        // ��������б任
        std::shuffle(board.begin(), board.begin() + 3, gen);
        std::shuffle(board.begin() + 3, board.begin() + 6, gen);
        std::shuffle(board.begin() + 6, board.end(), gen);

        // ��ӡ�����վֵ��ļ�
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
    // ����һ������
    std::vector<std::vector<int>> gameCopy = board;

    // ʹ�û����㷨�������
    return solveSudoku(gameCopy);
}

void generateUniqueSudoku(std::string filename, int gameCount) {
    std::ofstream file1(filename);
    if (!file1.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    // �����ѶȺͿո��������Ƽ���ո�����
   
    for (int i = 0; i < gameCount; ++i) {
        int emptyCount = std::uniform_int_distribution<>(20, 55)(gen);
        std::vector<std::vector<int>> solution(SIZE, std::vector<int>(SIZE, EMPTY));
        std::vector<std::vector<int>> game(SIZE, std::vector<int>(SIZE, EMPTY));

        // ���������վ�
        generateSudoku("temp_solution.txt", 1);

        // ��ȡ�����վ�
        std::ifstream solutionFile("temp_solution.txt");
        if (!solutionFile.is_open()) {
            std::cout << "Error opening solution file." << std::endl;
            file1.close();
            return;
        }

        // ���������վ�
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

        // �����ȥ�ո�
        while (emptyCount > 0) {
            int row = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            int col = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            if (game[row][col] != EMPTY) {
                // ��ʱ�Ƴ�����
                int temp = game[row][col];
                game[row][col] = EMPTY;

                // ����Ƿ���Ψһ��
                if (!hasUniqueSolution(game)) {
                    // ��Ψһ�⣬�ָ�����
                    game[row][col] = temp;
                    continue;
                }
                emptyCount--;
            }
            else {
                continue; // ��ǰ�����Ѿ�Ϊ�գ�����ѡ���µĸ���
            }
        }

        std::string gameFilename = "games_u" + std::to_string(i) + ".txt";
        std::ofstream file(gameFilename);
        if (!file.is_open()) {
            std::cout << "Error opening file: " << gameFilename << std::endl;
            return;
        }

        // ��ӡ������Ϸ���ļ�
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


// ����������Ϸ
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
        /*else
            emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);*/
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

        // ���������վ�
        generateSudoku("temp_solution.txt", 1);

        // ��ȡ�����վ�
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

        // ����������Ϸ
        
        int emptyCount = std::uniform_int_distribution<>(minHoles, maxHoles)(gen);  // �����ȥ�Ŀո�����
        while (emptyCount > 0) {
            int row = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            int col = std::uniform_int_distribution<>(0, SIZE - 1)(gen);
            if (game[row][col] != EMPTY) {
                game[row][col] = EMPTY;
                emptyCount--;
            }
            else {
                continue; // ��ǰ�����Ѿ�Ϊ�գ�����ѡ���µĸ���
            }
        }

        std::string gameFilename = "games" + std::to_string(i) + ".txt";
        std::ofstream file(gameFilename);
        if (!file.is_open()) {
            std::cout << "Error opening file: " << gameFilename << std::endl;
            return;
        }

        // ��ӡ������Ϸ���ļ�
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

int main(int argc, char* argv[]) {
    int sudokuCount = 0;
    int gameCount = 0;
    int minHoles = 0;
    int maxHoles = 0;
    int difficulty = 0;
    char* solvefilename=NULL;

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
            solvefilename = argv[i+1];
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
        if (hasN || hasR || hasM || hasU ) {
            std::cout << "Error: Invalid arguments." << std::endl;
            return 0;
        }
        solveSudoku(string(solvefilename));
    }
    else if (gameCount > 0) {
        cout << hasN << hasR << hasM << hasU;
        if (!hasN || (hasR && hasM)||(hasR && hasU)|| (hasU && hasM)) {
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
        std::cout << "Error��Invalid arguments." << std::endl;
    }

    return 0;
}
