/*
NAME : MAYURA PAVAN MANAWADU
INDEX NO : 17_ENG_072
REGISTRATION NO : EN86191

//PLEASE UNCOMMENT THE LINES 422 & 425 OF WRITEOUTPUT FUNCTION IF YOU WANT TO PRINT THE OUTPUT IN THE CONSOLE.
*/



#include <iostream>
#include <cmath>
#include <string>
#include<vector>
#include<sstream>
#include <regex>
#include <chrono>
#include <ctime>
#include<fstream>
#include <iomanip>
#define MAX 1000

using namespace std::chrono;
int sudokuSize;
std::string path;

//----------------------------4. Node structure of four pointers---------------------------------------//

/*
For the Dancing Link Data Structure A Node of 4 pointers (UP,DOWN,Left,Right) is needed.
This is similar to conventional doubly linked list. But connected in all the four directions.
*/

struct Node
{
	Node *left;
	Node *right;
	Node *up;
	Node *down;
	Node *head;
	int size;     //size of column header
	int mapID[3]; //Used in mapping solutions to a sudoku gid - format: (Candidate, Row, Column)
};


//----------------------------5. Sudoku Class which contains the implementation of Dancing Links---------------------------------------//

class Sudoku {


	//----------------------------6. Private Data for the algorithm ---------------------------------------//

private:

	struct Node Head;
	struct Node *HeadNode = &Head; //saves headnode of the dancing link data structure
	struct Node *finalSolution[MAX];
	struct Node *originalSudoku[MAX];
	bool** boolSudokuMatrix;  //boolean value to check the filled and unfilled cells
	bool isSolved = false;

public:


	//----------------------------7. Constructor for Sudoku Class---------------------------------------//


	Sudoku(int** Sudoku)
	{
		//intilizaling a sudoku matrix to generate the sparse matrix.
		boolSudokuMatrix = new bool*[(sudokuSize * sudokuSize * sudokuSize)];
		for (int i = 0; i < (sudokuSize * sudokuSize * sudokuSize); i++) {
			boolSudokuMatrix[i] = new bool[(4 * sudokuSize * sudokuSize)];
		}
		for (int i = 0; i < (sudokuSize * sudokuSize * sudokuSize); i++) {
			for (int j = 0; j < (4 * sudokuSize * sudokuSize); j++) {
				boolSudokuMatrix[i][j] = 0;
			}
		}

		generateSparseMatrix(boolSudokuMatrix);

		generateDacingLinks(boolSudokuMatrix);
		dancingLinksTOMatrix(Sudoku);
		check(0);
		if (!isSolved)
			std::cout << "No Solution!" << std::endl;
		isSolved = false;
	}

	//----------------------------11. Covering colums by removal of nodes and making the necessary adjustments ---------------------------------------//

	void cover(Node *col)
	{
		col->left->right = col->right; //Make the left and right columns point to each other
		col->right->left = col->left; // As a result of "deleting" column from the set of columns

		for (Node *node = col->down; node != col; node = node->down) // For each node contained in column
		{
			for (Node *temp = node->right; temp != node; temp = temp->right) // For each node to the right of current node
			{
				temp->down->up = temp->up;
				temp->up->down = temp->down; // Make the node above and below it, point to each other
				temp->head->size--; // Decrement the count for the columns that have been affected by these node removals
			}
		}
	}

	//----------------------------12. Reversing the cover column operation ---------------------------------------//
	/*
		Since in earlier stage the columns were covered by deleting the column and
		then the affected nodes, here the reverse opeation is conducted.
	*/
	void uncoverColumn(Node *col)
	{

		for (Node *node = col->up; node != col; node = node->up) //uncover from top to up
		{
			for (Node *temp = node->left; temp != node; temp = temp->left) //uncover from left to right
			{
				temp->head->size++; //increment the node count as the node is being read
				temp->down->up = temp; //make the cells up and down nodes points back to the cell
				temp->up->down = temp;
			}
		}
		col->left->right = col;
		col->right->left = col; //uncover the column
	}

	//----------------------------13.Check whether the operation is complete ---------------------------------------//

	void check(int k)
	{

		if (HeadNode->right == HeadNode) //if the right node of headnode directing to itself
		{

			int **finalGrid = new int*[sudokuSize];
			for (int i = 0; i < sudokuSize; ++i) {
				finalGrid[i] = new int[sudokuSize];

			}
			MapSolutionToFinalGrid(finalGrid);
			writeOutput(finalGrid);
			isSolved = true;
			return;
		}

		//Choose Column Object Deterministically: Choose the column with the smallest Size
		Node *Column = HeadNode->right;
		for (Node *temp = Column->right; temp != HeadNode; temp = temp->right)
			if (temp->size < Column->size)
				Column = temp;

		cover(Column);

		for (Node *temp = Column->down; temp != Column; temp = temp->down)
		{
			finalSolution[k] = temp;
			for (Node *node = temp->right; node != temp; node = node->right)
			{
				cover(node->head);
			}

			check(k + 1);

			temp = finalSolution[k];
			finalSolution[k] = NULL;
			Column = temp->head;
			for (Node *node = temp->left; node != temp; node = node->left)
			{
				uncoverColumn(node->head);
			}
		}

		uncoverColumn(Column);
	}



	//----------------------------8. Generate Sparse Matrix for all possibilities ---------------------------------------//

	/*3 Dimentional : sudokuSize * sudokuSize * sudokuSize is taken because
	exact cover problems have to be applied in 3 stages as Column-wise,
	Row-wise and SubGrid-wise satisfying the constraints. We fill this grid by 1 for
	each possibility*/

	void generateSparseMatrix(bool** boolSudokuMatrix)
	{

		//-------------To allow only one value in any given cell----------------
		int counter = 0, j = 0;
		for (int i = 0; i < (sudokuSize * sudokuSize * sudokuSize); i++)
		{ //iterate over all rows
			boolSudokuMatrix[i][j] = 1;
			counter++;
			if (counter >= sudokuSize)
			{
				j++;
				counter = 0;
			}
		}




		//-------------Allowing only one digit in a given row-----------------------
		int x = 0;
		counter = 1;
		//loop through all rows
		for (j = (sudokuSize * sudokuSize); j < 2 * (sudokuSize * sudokuSize); j++)
		{
			//dividing the linear array in to rows
			for (int i = x; i < counter * (sudokuSize * sudokuSize); i += sudokuSize)
				boolSudokuMatrix[i][j] = 1;

			//if the array element is a corner of a raw, the new row is taken for j
			if ((j + 1) % sudokuSize == 0)
			{
				x = counter * (sudokuSize * sudokuSize);
				counter++;
			}
			else
				x++;
		}



		//---------------Allowing only one digit in a given column------------------
		j = 2 * (sudokuSize * sudokuSize);
		for (int i = 0; i < (sudokuSize * sudokuSize * sudokuSize); i++)
		{
			boolSudokuMatrix[i][j] = 1;
			j++;
			if (j >= 3 * (sudokuSize * sudokuSize)) //next column
				j = 2 * (sudokuSize * sudokuSize);
		}


		//---------------Allowing only one digit in a given sub matrix------------------
		x = 0;
		for (j = 3 * (sudokuSize * sudokuSize); j < (4 * sudokuSize * sudokuSize); j++)
		{

			for (int l = 0; l < int(sqrt((double)sudokuSize)); l++)
			{
				for (int k = 0; k < int(sqrt((double)sudokuSize)); k++)
					boolSudokuMatrix[x + l * sudokuSize + k * (sudokuSize * sudokuSize)][j] = 1;
			}

			int gridLimit = j + 1 - 3 * (sudokuSize * sudokuSize);
			if (gridLimit % (int)(int(sqrt((double)sudokuSize)) * sudokuSize) == 0)
				x += (int(sqrt((double)sudokuSize)) - 1) * (sudokuSize * sudokuSize) + (int(sqrt((double)sudokuSize)) - 1) * sudokuSize + 1;
			//or each new grid
			else if (gridLimit % sudokuSize == 0)
				x += sudokuSize * (int(sqrt((double)sudokuSize)) - 1) + 1;
			else
				x++;
		}
	}

	//----------------------------9. Generating Dancing Links Data Structure from Sparse Matrix of all possibilities ---------------------------------------//

	void generateDacingLinks(bool** boolSudokuMatrix)
	{
		//initiliIng header node of the dancing links and pointers to neighbouring nodes
		Node *header = new Node;
		header->left = header;
		header->right = header;
		header->down = header;
		header->up = header;
		header->size = -1;
		header->head = header;
		Node *temp = header;

		//Column nodes which corresponds to OPTIONS for ITEMS in the dancing links data structure
		//multiplied by 4 to support four adjascent nodes
		for (int i = 0; i < (4 * sudokuSize * sudokuSize); i++)
		{
			Node *tempNode = new Node;
			tempNode->size = 0;
			tempNode->up = tempNode;
			tempNode->down = tempNode;
			tempNode->head = tempNode;
			tempNode->right = header;
			tempNode->left = temp;
			temp->right = tempNode;
			temp = tempNode;
		}

		//Used in mapping solutions to a sudoku gid - format: (Candidate, Row, Column)
		int candidate_row_column[3] = { 0, 1, 1 };

		//By comparing sparse matrix a node is added to each one present in the sparse matrix
		for (int i = 0; i < (sudokuSize * sudokuSize * sudokuSize); i++)
		{
			Node *rootNode = header->right;
			Node *previous = NULL;

			//setting up candicate, row and colum id based on the spase matrix
			if (i != 0 && i % (sudokuSize * sudokuSize) == 0)
			{
				candidate_row_column[0] -= sudokuSize - 1;
				candidate_row_column[1]++;
				candidate_row_column[2] -= sudokuSize - 1;
			}
			else if (i != 0 && i % sudokuSize == 0)
			{
				candidate_row_column[0] -= sudokuSize - 1;
				candidate_row_column[2]++;
			}
			else
			{
				candidate_row_column[0]++;
			}

			//iterate though all elements in boolean sudoku matrix
			for (int j = 0; j < (4 * sudokuSize * sudokuSize); j++, rootNode = rootNode->right)
			{
				if (boolSudokuMatrix[i][j]) //if there is a one in sparse matrix
				{
					Node *tempNode = new Node;
					//mapping dancing links from sparse matrix
					tempNode->mapID[0] = candidate_row_column[0];
					tempNode->mapID[1] = candidate_row_column[1];
					tempNode->mapID[2] = candidate_row_column[2];

					if (previous == NULL)
					{
						previous = tempNode;
						previous->right = tempNode;
					}

					//inserting nodes
					tempNode->left = previous;
					tempNode->right = previous->right;
					tempNode->right->left = tempNode;
					previous->right = tempNode;
					tempNode->head = rootNode;
					tempNode->down = rootNode;
					tempNode->up = rootNode->up;
					rootNode->up->down = tempNode;
					rootNode->size++;
					rootNode->up = tempNode;

					if (rootNode->down == rootNode)
						rootNode->down = tempNode;

					previous = tempNode;
				}
			}
		}

		HeadNode = header; //setting headnode.
	}

	//----------------------------10. Converting Dancing Links to Orginal Matrix Again ---------------------------------------//

	void dancingLinksTOMatrix(int** Puzzle)
	{

		int index = 0;
		for (int i = 0; i < sudokuSize; i++)
			for (int j = 0; j < sudokuSize; j++)
				//if there is a value
				if (Puzzle[i][j] > 0)
				{
					Node *Column = NULL;

					Node *temp = NULL;

					//looping though right of HeadNode
					for (Column = HeadNode->right; Column != HeadNode; Column = Column->right)
					{
						for (temp = Column->down; temp != Column; temp = temp->down)
							if (temp->mapID[0] == Puzzle[i][j] && (temp->mapID[1] - 1) == i && (temp->mapID[2] - 1) == j)
								goto breakLoop;
					}

				breakLoop:
					//removal of nodes in the columns
					cover(Column);
					originalSudoku[index] = temp;

					index++;

					for (Node *node = temp->right; node != temp; node = node->right)
					{
						cover(node->head);
					}
				}
	}

	//----------------------------13.Mapping the answer obtained in to the final grid. ---------------------------------------//


	void MapSolutionToFinalGrid(int** Sudoku)
	{

		for (int i = 0; finalSolution[i] != NULL; i++)
		{
			Sudoku[finalSolution[i]->mapID[1] - 1][finalSolution[i]->mapID[2] - 1] = finalSolution[i]->mapID[0];
		}
		for (int i = 0; originalSudoku[i] != NULL; i++)
		{
			Sudoku[originalSudoku[i]->mapID[1] - 1][originalSudoku[i]->mapID[2] - 1] = originalSudoku[i]->mapID[0];
		}
	}

	//----------------------------14.Writing the answer to output text file ---------------------------------------//

	void writeOutput(int** Sudoku)
	{

		std::regex regexp(".txt");

		std::string outputpath = std::regex_replace(path, regexp, "_output.txt");

		std::ofstream MyFile(outputpath);
		if (!isSolved) {
			for (int i = 0; i < sudokuSize; i++) {
				for (int j = 0; j < sudokuSize; j++) {
					MyFile << Sudoku[i][j] << " ";
					//std::cout << std::setw(3)<<Sudoku[i][j] << " ";
				}
				MyFile << "\n";
				//std::cout << "\n";
			}
		}
		else {
			MyFile << "No Solution";
		}

		MyFile.close();

	}

};



std::string readTextFile()
{
	std::string inputPath;
	std::cout << "Enter the path to text file: ";
	std::cin >> inputPath;
	std::ifstream temsudokuInputFile(inputPath);

	while (!temsudokuInputFile)
	{
		std::cout << "Path does not exist! Please insert a correct path: ";
		std::cin >> inputPath;
		temsudokuInputFile.open(inputPath);
	}

	temsudokuInputFile.close();
	return inputPath;
}

int main(int argc, char *argv[])
{
	//----------------------------1. Taking User Inputs---------------------------------------//

	std::ifstream sudokuInputFile; //read input text file

	if (argc == 2) //if parameters are provided in the command line
	{
		path = argv[1];
		sudokuInputFile.open(path); //argc[1] = path to the input text
		if (!sudokuInputFile)
		{
			path = readTextFile();
			sudokuInputFile.open(path);
		}
	}
	else //if no parameters are provided in the command line
	{
		path = readTextFile();
		sudokuInputFile.open(path);
	}


	//----------------------------2. Processing Sudoku Matrix---------------------------------------//

	//obtaining the size of Sudoku Puzle
	int sudokuPuzzleSize = std::distance(std::istream_iterator<double>(sudokuInputFile),
		std::istream_iterator<double>());
	sudokuSize = sqrt(sudokuPuzzleSize);


	//std::cout << "The size of the boolSudokuMatrix is : " << sudokuSize << std::endl;
	int **boolSudokuMatrix = new int *[sudokuSize];
	sudokuInputFile.close();

	//initiating a new session to read the file.
	sudokuInputFile.open(path);
	int row = 0;

	//store details of input text file into a 2D array
	std::string line;
	while (getline(sudokuInputFile, line))
	{
		int *arr = new int[sudokuSize];
		int number;
		int i = 0;
		for (std::istringstream numbers_iss(line);
			numbers_iss >> number; ) {
			arr[i] = number;
			i++;
		}
		boolSudokuMatrix[row] = arr;
		row++;
	}
	sudokuInputFile.close();


	//----------------------------3. Solve the Sudoku Puzzle---------------------------------------//
	//Sudoku sudokuObj;
	//auto start = steady_clock::now(); // Start the timer
	Sudoku sudokuObj(boolSudokuMatrix); //caling constructor of the Sudoku Class
	//auto end = steady_clock::now();

	//std::cout << "Execution time : " << duration<double>(end - start).count() << " S" << std::endl;

	return 0;
}


