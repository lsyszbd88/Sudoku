#include"stdafx.h"
#include"sudoku.h"
#include<fstream>



bool Sudoku::isPlace(int count)
{
	int row = count / 9;
	int col = count % 9;
	int j;
	for (j = 0; j < 9; j++)     //同一行
	{
		if (grid[row][j] == grid[row][col] && j != col)
			return false;
	}
	
	for (j = 0; j < 9; j++)     //同一列
	{
		if (grid[j][col] == grid[row][col] && j != row)
			return false;
	}
	
	int baseRow = row / 3 * 3;
	int baseCol = col / 3 * 3;
	for (j = baseRow; j < baseRow + 3; j++)   //同一宫
	{
		for (int k = baseCol; k < baseCol + 3; k++)
		{
			if (grid[j][k] == grid[row][col] && (j != row || k != col))
				return false;
		}
	}
	return true;
}

void Sudoku::backtrace(int count)
{
	if (count == 81)
	{
		for (int i = 0; i < 9; ++i)
		{
			
			fprintf(resultfile2, "%c %c %c %c %c %c %c %c %c\n", grid[i][0], grid[i][1], grid[i][2], grid[i][3], grid[i][4], grid[i][5], grid[i][6], grid[i][7], grid[i][8]);
		}
		fputs("\n", resultfile2);
		return;
	}
	int row = count / 9;   //position对应的x坐标
	int col = count % 9;  //position对应的y坐标
	if (grid[row][col] == '0')
	{
		for (int i = 1; i <= 9; i++)//尝试对这一点进行1~9的赋值
		{
			grid[row][col] = i + '0';
			if (isPlace(count))//可以放
			{
               backtrace(count + 1);//进入下一层
			}
				
		}
		grid[row][col] = '0';//回溯
	}
	else
	backtrace(count + 1);
}

void Sudoku::solveSudoku(string path)
{
	ifstream problemfile(path);

	//fopen_s用法:，须定义另外一个变量errno_t err，然后err = fopen_s(&fp, filename, "w")。
	errno_t err;
	err = fopen_s(&resultfile2, "sudoku.txt", "w+");

	if (problemfile)
	{
		int total = 0;
		string temp[9];
		string str;
		int line = 0;
		bool exc = false;     // 识别在problemfile有没有错误输入
		while (total < 1000000 && getline(problemfile, str))
		{
			temp[line] = str;  //从problemfile中读取一行到temp中
			line++;
			if (line == 9)   //每读入9行进行一次处理
			{
				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
					{
						grid[i][j] = temp[i][2 * j];  //跳过空格将一个数独问题装入grid中
						if (grid[i][j] < '0' || grid[i][j] > '9')
						{
							exc = true;
							break;
						}
					}
				getline(problemfile, str);//读入一个题目后的空行
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
		cout << "Can't find such file:" << string(path) << endl;
}
