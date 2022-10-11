/**------------------------------------------

	Program 2: MineSweeper

	Course: CS 141, Fall 2022.
	System: MacOS using Clion
	Starter Code Author: Dr. Sara Riazi
	Student Author: ALEXANDER E. CARRILLO
-------------------------------------------*/


#include <iostream> // for cin and cout
#include <string>

using namespace std;


//These variables set the size of the mine field 
//For this program since we don't know how to work with arrays yet, we keep the size of the board fixed. 
//When we learn about the arrays, we can ask the user for the field size.
constexpr int width = 5;
constexpr int height = 8;

//These variables are the possible states for each cell. 
//The negative values are the state of the cells,
// and the positive values determine the number of mines surrouding the cell.

// Note that I am using ALL CAPS for these state names to remind myself (and readers) that these are the names of states
// and I am using them to avoid using numbers such as -1 or -2 in the code that I may forget which number belongs to which state
constexpr int UNFLAGGED_MINE = -1; //The cell is mine that has not been flagged yet
constexpr int FLAGGED_MINE = -2; //The cell has been correctly flagged as a mine
constexpr int INCORRECT_FLAGGED_MINE = -3; //The cell has been incorrectly flagged as a mine
constexpr int UNKNOWN = -4; //The initial state of the cell. 

//The number of mine in the field
int num_mine = 10;

//We need one variable for each cell. The subscript of variables show the position of the cell in the field.
//For example, c00 is the cell on the first row and first column (we start the indices from zero to be consistent with arrays in C++ as you will see them later

int c00, c01, c02, c03, c04,
c10, c11, c12, c13, c14,
c20, c21, c22, c23, c24,
c30, c31, c32, c33, c34,
c40, c41, c42, c43, c44,
c50, c51, c52, c53, c54,
c60, c61, c62, c63, c64,
c70, c71, c72, c73, c74 = UNKNOWN;

//These three variables shows the state of games. You have to change these state during the game such the values reflect the correct state of the game.
int num_incorrect_flagged_mine = NULL; //number of incorrectly flagged cells
int num_correct_flagged_mine = NULL;  //number of correctly flagged cells
bool explode = false; // if the field has exploded or not

//To write a complex program your code needs to be modular. We can achieve the modularity via breaking the code into functions
//Each function is responsible for some functionalities. Here is the list of the functions in our program. 
//Among these functions you have to only implement initField, isMine, reveal and executeCmd. 
//Please don't change the signature of the existing functions. You may add more helper functions as you wish!

string getTag(const int c, const bool explode);
void displayField(const bool explode);
void setCell(const int i, const int j, const int value);
int getCell(const int i, const int j);
void setMine(const int num_of_mine);
void initField(const int num_of_mine);
bool isMine(const int i, const int j);
void reveal(const int i, const int j);
void executeCmd(const string cmd);

//The game has two states: in-progress, explode (game over)
//When we display the field to the user depedning of the game state the output is different.
//The helper getTag function returns the appropriate string (one character) to be shown in each cell depending on the state of the game
//Input:
//	c: the cell variable
//	explode: the game state (false for in-progress, true for game-over)
//Output:
//	one character string to be shown in the cell

string getTag(const int c, const bool explode) {
	string tag;
	switch (c) {
	case UNKNOWN:
		tag = R"(-)";
		break;
	case UNFLAGGED_MINE:
		if (explode)
			tag = R"(*)";
		else
			tag = R"(-)";
		break;
	case FLAGGED_MINE:
		tag = R"(!)";
		num_mine--;
		break;
	case INCORRECT_FLAGGED_MINE:
		if (explode)
			tag = R"(&)";
		else
			tag = R"(!)";
		break;
	default:
		tag = to_string(c);
	}
	return tag;
}

//The displayField function displays the mine field (including the indices of the columns and rows)
//Input:
//	explode: indicates the state of the game (false for in-progress or true for game-over)
//	output: None

void displayField(const bool explode) {
	cout << R"(   0 1 2 3 4)" << endl << endl;
	cout << R"(0  )" << getTag(c00, explode) << R"( )" << getTag(c01, explode) << R"( )" << getTag(c02, explode) << R"( )" << getTag(c03, explode) << R"( )" << getTag(c04, explode) << endl;
	cout << R"(1  )" << getTag(c10, explode) << R"( )" << getTag(c11, explode) << R"( )" << getTag(c12, explode) << R"( )" << getTag(c13, explode) << R"( )" << getTag(c14, explode) << endl;
	cout << R"(2  )" << getTag(c20, explode) << R"( )" << getTag(c21, explode) << R"( )" << getTag(c22, explode) << R"( )" << getTag(c23, explode) << R"( )" << getTag(c24, explode) << endl;
	cout << R"(3  )" << getTag(c30, explode) << R"( )" << getTag(c31, explode) << R"( )" << getTag(c32, explode) << R"( )" << getTag(c33, explode) << R"( )" << getTag(c34, explode) << endl;
	cout << R"(4  )" << getTag(c40, explode) << R"( )" << getTag(c41, explode) << R"( )" << getTag(c42, explode) << R"( )" << getTag(c43, explode) << R"( )" << getTag(c44, explode) << endl;
	cout << R"(5  )" << getTag(c50, explode) << R"( )" << getTag(c51, explode) << R"( )" << getTag(c52, explode) << R"( )" << getTag(c53, explode) << R"( )" << getTag(c54, explode) << endl;
	cout << R"(6  )" << getTag(c60, explode) << R"( )" << getTag(c61, explode) << R"( )" << getTag(c62, explode) << R"( )" << getTag(c63, explode) << R"( )" << getTag(c64, explode) << endl;
	cout << R"(7  )" << getTag(c70, explode) << R"( )" << getTag(c71, explode) << R"( )" << getTag(c72, explode) << R"( )" << getTag(c73, explode) << R"( )" << getTag(c74, explode) << endl;
}

//The setCell functions assign a value to the cell specified by its position (row and column) in the field, which makes our life easier while we don't know how to work with arrays!
//Input:
//	i: the row index (starting from zero)
//	j: the column index (start from zero)
//Ouput: 
//	None

void setCell(const int i, const int j, const int value) {
	if (i == 0 && j == 0)
		c00 = value;
	else if (i == 0 && j == 1)
		c01 = value;
	else if (i == 0 && j == 2)
		c02 = value;
	else if (i == 0 && j == 3)
		c03 = value;
	else if (i == 0 && j == 4)
		c04 = value;
	else if (i == 1 && j == 0)
		c10 = value;
	else if (i == 1 && j == 1)
		c11 = value;
	else if (i == 1 && j == 2)
		c12 = value;
	else if (i == 1 && j == 3)
		c13 = value;
	else if (i == 1 && j == 4)
		c14 = value;
	else if (i == 2 && j == 0)
		c20 = value;
	else if (i == 2 && j == 1)
		c21 = value;
	else if (i == 2 && j == 2)
		c22 = value;
	else if (i == 2 && j == 3)
		c23 = value;
	else if (i == 2 && j == 4)
		c24 = value;
	else if (i == 3 && j == 0)
		c30 = value;
	else if (i == 3 && j == 1)
		c31 = value;
	else if (i == 3 && j == 2)
		c32 = value;
	else if (i == 3 && j == 3)
		c33 = value;
	else if (i == 3 && j == 4)
		c34 = value;
	else if (i == 4 && j == 0)
		c40 = value;
	else if (i == 4 && j == 1)
		c41 = value;
	else if (i == 4 && j == 2)
		c42 = value;
	else if (i == 4 && j == 3)
		c43 = value;
	else if (i == 4 && j == 4)
		c44 = value;
	else if (i == 5 && j == 0)
		c50 = value;
	else if (i == 5 && j == 1)
		c51 = value;
	else if (i == 5 && j == 2)
		c52 = value;
	else if (i == 5 && j == 3)
		c53 = value;
	else if (i == 5 && j == 4)
		c54 = value;
	else if (i == 6 && j == 0)
		c60 = value;
	else if (i == 6 && j == 1)
		c61 = value;
	else if (i == 6 && j == 2)
		c62 = value;
	else if (i == 6 && j == 3)
		c63 = value;
	else if (i == 6 && j == 4)
		c64 = value;
	else if (i == 7 && j == 0)
		c70 = value;
	else if (i == 7 && j == 1)
		c71 = value;
	else if (i == 7 && j == 2)
		c72 = value;
	else if (i == 7 && j == 3)
		c73 = value;
	else if (i == 7 && j == 4)
		c74 = value;
}

//The getCell function return the value of the cell specified by its row and column index in the field.
//Input:
//	i: the row index (starting from 0)
//	j: the column index (starting from 0)
//Ouput:
//	The cell value

int getCell(const int i, const int j) {
	int value{};
	if (i == 0 && j == 0)
		value = c00;
	else if (i == 0 && j == 1)
		value = c01;
	else if (i == 0 && j == 2)
		value = c02;
	else if (i == 0 && j == 3)
		value = c03;
	else if (i == 0 && j == 4)
		value = c04;
	else if (i == 1 && j == 0)
		value = c10;
	else if (i == 1 && j == 1)
		value = c11;
	else if (i == 1 && j == 2)
		value = c12;
	else if (i == 1 && j == 3)
		value = c13;
	else if (i == 1 && j == 4)
		value = c14;
	else if (i == 2 && j == 0)
		value = c20;
	else if (i == 2 && j == 1)
		value = c21;
	else if (i == 2 && j == 2)
		value = c22;
	else if (i == 2 && j == 3)
		value = c23;
	else if (i == 2 && j == 4)
		value = c24;
	else if (i == 3 && j == 0)
		value = c30;
	else if (i == 3 && j == 1)
		value = c31;
	else if (i == 3 && j == 2)
		value = c32;
	else if (i == 3 && j == 3)
		value = c33;
	else if (i == 3 && j == 4)
		value = c34;
	else if (i == 4 && j == 0)
		value = c40;
	else if (i == 4 && j == 1)
		value = c41;
	else if (i == 4 && j == 2)
		value = c42;
	else if (i == 4 && j == 3)
		value = c43;
	else if (i == 4 && j == 4)
		value = c44;
	else if (i == 5 && j == 0)
		value = c50;
	else if (i == 5 && j == 1)
		value = c51;
	else if (i == 5 && j == 2)
		value = c52;
	else if (i == 5 && j == 3)
		value = c53;
	else if (i == 5 && j == 4)
		value = c54;
	else if (i == 6 && j == 0)
		value = c60;
	else if (i == 6 && j == 1)
		value = c61;
	else if (i == 6 && j == 2)
		value = c62;
	else if (i == 6 && j == 3)
		value = c63;
	else if (i == 6 && j == 4)
		value = c64;
	else if (i == 7 && j == 0)
		value = c70;
	else if (i == 7 && j == 1)
		value = c71;
	else if (i == 7 && j == 2)
		value = c72;
	else if (i == 7 && j == 3)
		value = c73;
	else if (i == 7 && j == 4)
		value = c74;
	return value;
}

//The function setMine randomly assigns a predefined number of mines to the field
//Input:
//	num_of_mine: The number of mines to be assigned to the field
//Output:
//	None
//void setMine(int num_of_mine) {
//	int i, j;
//	for (int k = 0; k < num_of_mine; k++) {
//		i = rand() % 8;
//		j = rand() % 5;
//		if (getCell(i, j) == 0)
//			setCell(i, j, 9);
//		else
//			k--;
//	}
//}

void setMine(const int num_of_mine) {
	int current_mine = 0; // The number of assigned mines so far.
	while (current_mine < num_of_mine) {
		//We have to randomly generate the row index and column index for a cell and put a mine in that cell
		//We use the rand function to generate the indices.
		//The rand function randomly generate a number between 0 and RAND_MAX
		//You can read more about it here: https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/ 	
		//By using % operator, we can make sure that we generate random number between 0 and height (excluding height). 
		//We use the generate random value is the row index of the cell
		const int i = rand() % height;

		//We do the same for the column index
		const int j = rand() % width;

		//If we have already selected the cell we should continue with another random cell
		//Otherwise we set the cell to be a mine 
		if (getCell(i, j) != UNFLAGGED_MINE) {
			setCell(i, j, UNFLAGGED_MINE);
			current_mine++;
		}
	}
}

//The initField function initilizes the field with UNKOWN cells and then random 
//The initField function initilizes the field with UNKOWN cells and then randomly sets some of the cells to be mine (using the setMin function)
//Input:
//	num_of_mine: The number of mines in the field
//Output:
//	None

void initField(const int num_of_mine) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			setCell(i, j, UNKNOWN);
		}
	}
	setMine(num_of_mine);
}

//The isMine function checks whether a cell is a mine or not
//In our game design a cell is a mine if its state is FLAGGED_MINE or UNFLAGGED_MINE 
//Input: 
//	i: The row index of the cell (starting from 0)
//	j: The column index of the cell (starting from 0)
//Output:
//	Returns true if the cell has a mine (UNFLAGGED_MINE or FLAGGED_MINE)

bool isMine(const int i, const int j) {
	return getCell(i, j) == UNFLAGGED_MINE || getCell(i, j) == FLAGGED_MINE;
}


//The reveal function set an appropriate values to each cell based on the player move.
//The function is called when the player decides that a cell is not a mine and wants to reveal it.
//Based on the game description, if the cell is not a mine, the user will see the number of mines in the surronding cells as the value of the selected cell 
//In a more advanced version, if there is no mine in the surrounding cells the program reveals all surrounding cells as well.
//To implement the advanced version you need to use recursive functions. We will see the recursive functions toward the end of the semester. However, you can read about the recursive function and implement the advanced version for 10 bonus points on your own.(https://www.geeksforgeeks.org/recursive-functions/)
//Input
//	i: the row index
//	j: the column index
//Output
//	None

void reveal(const int i, const int j) {
	if (isMine(i, j)) {
		setCell(i, j, FLAGGED_MINE);
	}
	else {
		int num_of_mine = 0;
		if (i > 0 && j > 0 && isMine(i - 1, j - 1))
			num_of_mine++;
		if (i > 0 && isMine(i - 1, j))
			num_of_mine++;
		if (i > 0 && j < width - 1 && isMine(i - 1, j + 1))
			num_of_mine++;
		if (j > 0 && isMine(i, j - 1))
			num_of_mine++;
		if (j < width - 1 && isMine(i, j + 1))
			num_of_mine++;
		if (i < height - 1 && j > 0 && isMine(i + 1, j - 1))
			num_of_mine++;
		if (i < height - 1 && isMine(i + 1, j))
			num_of_mine++;
		if (i < height - 1 && j < width - 1 && isMine(i + 1, j + 1))
			num_of_mine++;
		setCell(i, j, num_of_mine);
	}
}

//The executeCmd function takes a command and change the state of game and the cells accordingly.
//the executeCmd may change the global variables
//Input:
//	cmd: the command string. For example, f12, r02, or u12
//Output:
//	None

void executeCmd(const string cmd) {
	if (cmd[0] == 'f') {
		int i = cmd[1] - '0';
		int j = cmd[2] - '0';
		if (getCell(i, j) == UNKNOWN)
			setCell(i, j, FLAGGED_MINE);
		else if (getCell(i, j) == FLAGGED_MINE)
			setCell(i, j, UNKNOWN);
	}
	else if (cmd[0] == 'r') {
		const int i = cmd[1] - '0';
		const int j = cmd[2] - '0';
		reveal(i, j);
	}
	else if (cmd[0] == 'u') {
		const int i = cmd[1] - '0';
		const int j = cmd[2] - '0';
		if (getCell(i, j) == FLAGGED_MINE)
			setCell(i, j, UNFLAGGED_MINE);
	}
}

//The main function of the program
int main() {
	srand(0);//using the same seed generates the same random sequence! In general, to have different games at each run we need to use a seed that is different for that run, for example, we can select the seed as function of time.
	initField(num_mine);
	string cmd;

	displayField(false);
	while (!explode && (num_correct_flagged_mine < num_mine || num_incorrect_flagged_mine > 0)) {

		cout << R"(Enter cmd:)" << endl;
		cin >> cmd;

		executeCmd(cmd);

		displayField(explode);
		cout << R"(Number of mine left: )" << num_mine - (num_correct_flagged_mine + num_incorrect_flagged_mine) << endl;
	}
	if (!explode) {
		cout << R"(:))" << endl;

	}
	else {
		cout << R"(:()" << endl;
	}
	return 0;
}