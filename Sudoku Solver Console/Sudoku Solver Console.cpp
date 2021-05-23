// Sudoku Solver Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <stdlib.h>
#include <list>
using namespace std;
const bool debug = false;;

class Sudoku {
	//Varriables
private:
	//The size of the board
	static const int8_t size = 9;

	//The base state of the board (this will be only used for storing the base state)
	int board[size][size] = {
		{0,9,7,0,0,0,0,0,8},
		{3,1,0,5,0,0,4,0,2},
		{0,0,0,9,0,0,0,1,3},
		{0,0,3,0,0,0,0,2,0},
		{7,8,0,0,0,0,0,3,9},
		{0,0,5,0,0,0,0,0,0},
		{6,0,0,0,0,5,2,8,0},
		{8,0,0,0,0,0,3,7,5},
		{0,0,1,0,0,0,0,0,4}
	};

	//This 2d array will be used to solve the sudoku
	int solvingArray[9][9] = {};

	//Constructor
public:
	Sudoku() {
	}

	//Private Methods
private:
	//Writes out the content of the 2d array which will be a possible board
	void WriteOut(int arrToWrite[9][9]) {
		cout << "The current board" << endl;
		for (size_t i = 0; i < size; i++)
		{
			if (i % 3 == 0)
				cout << "--------------------------" << endl;
			for (size_t j = 0; j < size; j++)
			{
				if (j % 3 == 0)
					cout << " |";
				if (arrToWrite[i][j] == 0)
					cout << "  ";
				else
					cout << " " + to_string(arrToWrite[i][j]);
			}
			cout << " |" << endl;
		}
		cout << "--------------------------" << endl;
	}

	//-----------------------------------------------------------------------
	//Copy the board (base) contents from backwards till it reaches the number which was specified in the numberOfLines parameter
	void CopyToSolvingArray(int numberOfLines) {
		for (int i = size - 1; i >= numberOfLines; i--)
		{
			for (int j = 0; j < size; j++)
			{
				solvingArray[i][j] = board[i][j];
			}
		}
	}

	//----------------------------------------------------------------
	//Gets the possible numbers for the spot according to the current solvingArray
	list<int> GetPossibleNumbers(int row, int col)
	{
		//if the cell is not empty
		if (solvingArray[row][col] != 0)
			throw new exception("The cell is not empty");

		list<int> possNumbers = {};
		bool valid = true;
		//Horizontal test
		for (size_t number = 1; number <= 9; number++)
		{
			for (size_t i = 0; i < size; i++)
			{
				if (solvingArray[row][i] == number)
				{
					valid = false;
					break;
				}
			}
			if (valid)
				possNumbers.push_back(number);
			else
				possNumbers.remove(number);
			valid = true;
		}
		//Vertical test
		for (size_t number = 1; number <= 9; number++)
		{
			for (size_t i = 0; i < size; i++)
			{
				if (solvingArray[i][col] == number)
				{
					valid = false;
					break;
				}
			}
			if (!valid)
				possNumbers.remove(number);
			valid = true;
		}
		//Square test
		for (size_t number = 1; number <= 9; number++)
		{
			for (size_t i = row / 3 * 3; i < row / 3 * 3 + 3; i++)
			{
				for (size_t j = col / 3 * 3; j < col / 3 * 3 + 3; j++)
				{
					if (solvingArray[i][j] == number)
					{
						valid = false;
						break;
					}
				}
			}
			if (!valid)
				possNumbers.remove(number);
			valid = true;
		}
		return possNumbers;
	}

	//*************************************************************
	//Public Methods
public:
	/// <summary>
	/// Writes out the board to the console
	/// </summary>
	void WriteOutBoard() {
		this->WriteOut(board);
	}

	/// <summary>
	/// Solves the sudoku
	/// </summary>
	/// <returns>0 on success 1 when it is unsolveable</returns>
	int Solve() {
		int failStack = 0;
		int rowNumber = 0;
		int cycle = 0;
		bool failed = false;
		CopyToSolvingArray(0);
		do
		{
			failed = false;
			//Go through the row
			for (size_t col = 0; col < size; col++)
			{
				//If the row is not empty
				if (solvingArray[rowNumber][col] != 0)
					continue;
				list<int> possNumbers = GetPossibleNumbers(rowNumber, col);
				//If the possNumbers is empty it means that this is not possible
				if (possNumbers.empty()) {
					failStack++;
					failed = true;
					break;
				}
				solvingArray[rowNumber][col] = *next(possNumbers.begin(), rand() % possNumbers.size());
			}

			//If the current try failed then according to the number of failstacks go back by an x amount of rows
			if (failed) {
				//Reset the whole solving
				if (failStack % 100 == 0)
					rowNumber = 0;
				//Go back 3 rows
				else if (failStack % 20 == 0 && rowNumber > 2)
					rowNumber = rowNumber - 3;
				//Go back one row
				else if (failStack % 5 == 0 && rowNumber > 0)
					rowNumber--;

				CopyToSolvingArray(rowNumber);
			}
			//increment the rowNumber if the try was a success
			else
				rowNumber++;
			//Debug code don't concern yourself with it
			if (debug)
				WriteOut(solvingArray);

			cycle++;

			if (failStack > 20000)
				return 1;
		} while (rowNumber < size);
		cout << "Got it in " + to_string(cycle) + " cycles" << endl;
		WriteOut(solvingArray);
		return 0;
	}
};

//**********************************************************************************
//Entry point of the code
int main()
{
	//Give the random a seed
	srand(time(NULL) * 15);

	Sudoku* game = new Sudoku();
	game->WriteOutBoard();
	game->Solve();
}